#include <zmq.hpp>
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Please provide a message to send as the first argument." << std::endl;
        return -1;
    }

    std::string messageToSend(argv[1]);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);

    std::cout << "Connecting to server..." << std::endl;
    socket.connect("ipc:///tmp/test");

    zmq::message_t request(messageToSend.size());
    memcpy(request.data(), messageToSend.data(), messageToSend.size());
    std::cout << "Sending message " << messageToSend << "..." << std::endl;
    socket.send(request, zmq::send_flags::none);

    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);

    std::string received_message(static_cast<char*>(reply.data()), reply.size());
    std::cout << "Received message: " << received_message << std::endl;

    return 0;
}
