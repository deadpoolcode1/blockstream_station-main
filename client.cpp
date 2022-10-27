#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

int main (void)
{
  zmq::context_t context(1);

    //  Socket to talk to server
    printf ("Connecting to hello world server…\n");
    zmq::socket_t sock (context, ZMQ_REQ);
    sock.connect("ipc:///tmp/test");
    char msg[] = "Hell1";
    char buffer[256] = {0x00};
    int request_nbr;
    for (request_nbr = 0; request_nbr != 10000; request_nbr++) {
      zmq_send(sock, strdup(msg), strlen(msg), 0);
      std::cout << "Sending Hello" << std::endl;
      zmq::message_t reply;
      zmq_recv(sock, buffer, sizeof(buffer), 0);
      std::string temp(buffer);
      std::cout << "Receive " + temp << std::endl;
    }
    sock.close();
    return 0;
}
