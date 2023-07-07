#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "../json/include/nlohmann/json.hpp"
#include "zmq_server.h"

std::string ZmqServer::version = "1.0.0";

ZmqServer::ZmqServer(zmq::context_t& context) 
    : context(context), socket(context, ZMQ_REP) {
    socket.bind("ipc:///tmp/test");
}

void ZmqServer::Start() {
    thread = std::thread([this] { this->Run(); });
}

void ZmqServer::Stop() {
    stop = true;
    if (thread.joinable()) {
        thread.join();
    }
}
 
std::string ZmqServer::ReadI2CRegister(int bus, const std::string& address, const std::string& reg){
    int addressInt = std::stoi(address, nullptr, 16);
    int regInt = std::stoi(reg, nullptr, 16);
    char filename[20];
    sprintf(filename, "/dev/i2c-%d", bus);
    int file = open(filename, O_RDWR);
    if (file < 0) {
        /* ERROR HANDLING */
    }
    if (ioctl(file, I2C_SLAVE, address) < 0) {
        /* ERROR HANDLING */
    }

    char buf[2] = {0};
    buf[0] = regInt;
    if (write(file, buf, 1) != 1) {
        /* ERROR HANDLING */
    }

    if (read(file, buf, 1) != 1) {
        /* ERROR HANDLING */
    }

    close(file);

    char result[3];
    sprintf(result, "%02x", buf[0]);
    return std::string(result);
}

void ZmqServer::WriteI2CRegister(int bus, const std::string& address, const std::string& reg, const std::string& value) {
    int addressInt = std::stoi(address, nullptr, 16);
    int regInt = std::stoi(reg, nullptr, 16);
    int valueInt = std::stoi(value, nullptr, 16);
    char filename[20];
    sprintf(filename, "/dev/i2c-%d", bus);
    int file = open(filename, O_RDWR);
    if (file < 0) {
        /* ERROR HANDLING */
    }
    if (ioctl(file, I2C_SLAVE, address) < 0) {
        /* ERROR HANDLING */
    }

    char buf[2] = {0};
    buf[0] = regInt;
    buf[1] = valueInt;
    if (write(file, buf, 2) != 2) {
        /* ERROR HANDLING */
    }

    close(file);
}

std::string ZmqServer::HandleCommand(const nlohmann::json& command_json) {
    std::string command = command_json.value("command", "");
    if (command == "get_app_version") {
        return version;
    } else if (command == "read_i2c_register") {
        int bus = command_json["parameters"].value("bus", -1);
        std::string address = command_json["parameters"].value("address", "");
        std::string reg = command_json["parameters"].value("reg", "");
        if (bus == -1 || address.empty() || reg.empty()) {
            return "Invalid parameters";
        }
        return ReadI2CRegister(bus, address, reg);
    } else if (command == "write_i2c_register") {
        int bus = command_json["parameters"].value("bus", -1);
        std::string address = command_json["parameters"].value("address", "");
        std::string reg = command_json["parameters"].value("reg", "");
        std::string value = command_json["parameters"].value("value", "");
        if (bus == -1 || address.empty() || reg.empty() || value.empty()) {
            return "Invalid parameters";
        }
        WriteI2CRegister(bus, address, reg, value);
        return "ACK";
    } else {
        return "Unknown command";
    }
}

void ZmqServer::Run() {
    while (!stop) {
        zmq::message_t request;

        // Use the new recv function
        auto res = socket.recv(request, zmq::recv_flags::none);
        if (!res) {
            // Handle the error, for example:
            std::cerr << "Failed to receive message: " << zmq_strerror(errno) << "\n";
            continue;
        }
        
        // Define received_message in this scope
        std::string received_message(static_cast<char*>(request.data()), request.size());
        std::cout << "Received message: " << received_message << "\n";

        // Parse the message and handle the command
        nlohmann::json command_json = nlohmann::json::parse(received_message);
        std::string response = HandleCommand(command_json);
        
        // Create the reply
        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.data(), response.size());

        // Use the new send function
        socket.send(reply, zmq::send_flags::none);
    }
}
