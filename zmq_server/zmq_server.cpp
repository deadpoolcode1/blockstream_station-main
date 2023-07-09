#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "../json/include/nlohmann/json.hpp"
#include "zmq_server.h"

std::string ZmqServer::version = "1.0.0";

ZmqServer::ZmqServer(zmq::context_t& context)
    : context(context), socket(context, ZMQ_REP)
{
    socket.bind("ipc:///tmp/test");
}

void ZmqServer::Start()
{
    thread = std::thread([this] { this->Run(); });
}

void ZmqServer::Stop()
{
    stop = true;
    if (thread.joinable())
        thread.join();
}

std::string ZmqServer::WrapReply(const std::string& result)
{
    nlohmann::json reply;
    reply["command"] = "ACK";
    reply["result"] = result;
    return reply.dump();
}

std::string ZmqServer::ExecuteCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        return "Error executing command";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    // Remove trailing newline character
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    return WrapReply(result);
}

std::string ZmqServer::ReadDiskUsage() {
    std::string command = "df -h --output=source,size,used,avail,pcent,target";
    return WrapReply(ExecuteCommand(command));
}

std::string ZmqServer::ReadMemoryUsage() {
    std::string command = "free -m";
    return WrapReply(ExecuteCommand(command));
}

__s32 i2c_smbus_access(int file, char read_write, __u8 command, int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;
    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(file, I2C_SMBUS, &args);
}

__s32 i2c_smbus_read_byte_data(int file, __u8 command)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(file, I2C_SMBUS_READ, command, I2C_SMBUS_BYTE_DATA, &data))
        return -1;
    else
        return 0x0FF & data.byte;
}

__s32 i2c_smbus_write_byte_data(int file, __u8 command, __u8 value)
{
    union i2c_smbus_data data;
    data.byte = value;
    if (i2c_smbus_access(file, I2C_SMBUS_WRITE, command, I2C_SMBUS_BYTE_DATA, &data))
        return -1;
    else
        return 0;
}

std::string ZmqServer::ReadI2CRegister(int bus, int address, int reg)
{
    char filename[20];
    sprintf(filename, "/dev/i2c-%d", bus);
    int file = open(filename, O_RDWR);
    if (file < 0)
        return WrapReply("Error");

    if (ioctl(file, I2C_SLAVE_FORCE, address) < 0)
    {
        close(file);
        return WrapReply("Error");
    }

    int data = i2c_smbus_read_byte_data(file, reg);
    char result[3];
    sprintf(result, "%02x", data);
    close(file);

    return WrapReply(std::string(result));
}

void ZmqServer::WriteI2CRegister(int bus, int address, int reg, int value)
{
    char filename[20];
    sprintf(filename, "/dev/i2c-%d", bus);
    int file = open(filename, O_RDWR);
    if (file < 0)
        return;

    if (ioctl(file, I2C_SLAVE_FORCE, address) < 0)
    {
        close(file);
        return;
    }

    if (i2c_smbus_write_byte_data(file, reg, value) < 0)
    {
        close(file);
        return;
    }

    close(file);
}

std::string ZmqServer::HandleCommand(const nlohmann::json& command_json) {
    std::string command = command_json.value("command", "");
    if (command == "get_app_version") {
        return WrapReply(version);
    } else if (command == "read_i2c_register") {
        int bus = command_json["parameters"].value("bus", -1);
        int address = command_json["parameters"].value("address", -1);
        int reg = command_json["parameters"].value("reg", -1);
        if (bus == -1 || address == -1 || reg == -1) {
            return WrapReply("Invalid parameters");
        }
        return ReadI2CRegister(bus, address, reg);
    } else if (command == "write_i2c_register") {
        int bus = command_json["parameters"].value("bus", -1);
        int address = command_json["parameters"].value("address", -1);
        int reg = command_json["parameters"].value("reg", -1);
        int value = command_json["parameters"].value("val", -1);
        if (bus == -1 || address == -1 || reg == -1 || value == -1) {
            return WrapReply("Invalid parameters");
        }
        WriteI2CRegister(bus, address, reg, value);
        return WrapReply("ACK");
    } else if (command == "read_disk_usage") {
        return ReadDiskUsage();
    } else if (command == "read_memory_usage") {
        return ReadMemoryUsage();
    } else {
        return WrapReply("Unknown command");
    }
}

void ZmqServer::Run() {
    while (!stop) {
        zmq::message_t request;

        auto res = socket.recv(request, zmq::recv_flags::none);
        if (!res) {
            std::cerr << "Failed to receive message: " << zmq_strerror(errno) << "\n";
            continue;
        }
        
        std::string received_message(static_cast<char*>(request.data()), request.size());
        std::cout << "Received message: " << received_message << "\n";

        nlohmann::json command_json = nlohmann::json::parse(received_message);
        std::string response = HandleCommand(command_json);
        
        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.data(), response.size());

        socket.send(reply, zmq::send_flags::none);
    }
}
