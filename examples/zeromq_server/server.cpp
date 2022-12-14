//
//  test server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

int main () {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    //socket.bind ("tcp://*:5555");
    socket.bind ("ipc:///tmp/test");
    std::cout << "Server started "  << std::endl;
    char buffer[256];
    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        zmq_recv(socket, buffer, sizeof(buffer), 0);
        std::string temp(buffer);
        //  Do some 'work'
        std::cout << "Server Receive " + temp << std::endl;
        //sleep (1);

        //  Send reply back to client
        //zmq::message_t reply (5);
	    char reply[] = "World";
        std::string temp2(reply);
        std::cout << "Sending reply: " + temp2 << std::endl;
        //memcpy ((void *) reply.data (), "World", 5);
        //socket.send (reply);
	    zmq_send(socket, strdup(reply), strlen(reply), 0);
    }
    return 0;
}
