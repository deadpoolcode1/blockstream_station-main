#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "zmq_server.h"

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

void ZmqServer::Run() {
    while (!stop) {
        zmq::message_t request;
        
        // Use the new recv function
        auto res = socket.recv(request, zmq::recv_flags::none);
        if (!res) {
            // Handle the error, for example:
            std::cerr << "Failed to receive message: " << zmq_strerror(errno) << "\n";
            continue;
        } else {
            std::string received_message(static_cast<char*>(request.data()), request.size());
            std::cout << "Received message: " << received_message << "\n";
        }     
        // Do some 'work'
        sleep(1);
        
        zmq::message_t reply(5);
        memcpy(reply.data(), "World", 5);

        // Use the new send function
        socket.send(reply, zmq::send_flags::none);
    }
}