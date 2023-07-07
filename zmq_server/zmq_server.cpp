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
        socket.recv(&request);
        // Do some 'work'
        sleep(1);
        zmq::message_t reply(5);
        memcpy(reply.data(), "World", 5);
        socket.send(reply);
    }
}