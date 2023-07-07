#include <stdio.h>
#include <unistd.h>
#include <iostream>
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

std::string ZmqServer::HandleCommand(const nlohmann::json& command_json) {
    std::string command = command_json.value("command", "");
    if (command == "get_app_version") {
        return std::string(version);
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
