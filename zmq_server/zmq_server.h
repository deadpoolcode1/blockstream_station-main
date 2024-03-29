#ifndef ZMQ_SERVER_H
#define ZMQ_SERVER_H

#include <zmq.hpp>
#include <thread>
#include <atomic>
#include "../json/include/nlohmann/json.hpp"

class ZmqServer {
public:
    ZmqServer(zmq::context_t& context);
    void Start();
    void Stop();

private:
    void Run();
    std::string HandleCommand(const nlohmann::json& command_json);
    std::string ExecuteCommand(const std::string& command);
    std::string ReadDiskUsage();
    std::string ReadMemoryUsage();
    std::string ReadI2CRegister(int bus, int address, int reg);
    void WriteI2CRegister(int bus, int address, int reg, int value);
    std::string WrapReply(const std::string& result);
    uint8_t readMemoryAddress(uintptr_t address);
    zmq::context_t& context;
    zmq::socket_t socket;
    std::thread thread;
    std::atomic_bool stop{false};
    static std::string version;
};

#endif  // ZMQ_SERVER_H
