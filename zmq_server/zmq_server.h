#ifndef ZMQ_SERVER_H
#define ZMQ_SERVER_H

#include <zmq.hpp>
#include <thread>

class ZmqServer {
public:
    ZmqServer(zmq::context_t& context);
    void Start();
    void Stop();
private:
    void Run();

    zmq::context_t& context;
    zmq::socket_t socket;
    std::thread thread;
    bool stop = false;
};

#endif // ZMQ_SERVER_H
