#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <zmq.hpp>
#include <string>

void* handleClient(void* clientSocket) {
    int socket = *(int*)clientSocket;
    char buffer[1024];
    int bytesRead;

    while ((bytesRead = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Received command: " << buffer << std::endl;

        // Send command to ZeroMQ server
        std::string messageToSend(buffer);

        zmq::context_t context(1);
        zmq::socket_t zmqSocket(context, ZMQ_REQ);

        std::cout << "Connecting to ZeroMQ server..." << std::endl;
        zmqSocket.connect("ipc:///tmp/test");

        zmq::message_t request(messageToSend.size());
        memcpy(request.data(), messageToSend.data(), messageToSend.size());
        std::cout << "Sending message to ZeroMQ server: " << messageToSend << "..." << std::endl;
        zmqSocket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        zmqSocket.recv(reply, zmq::recv_flags::none);

        std::string received_message(static_cast<char*>(reply.data()), reply.size());
        std::cout << "Received message from ZeroMQ server: " << received_message << std::endl;

        // Send ZeroMQ server's response back over TCP
        send(socket, received_message.c_str(), received_message.size(), 0);
    }

    close(socket);
    return NULL;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Bind socket to port 5555
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5555);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding to port 5555" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Listening on TCP port 5555..." << std::endl;

    pthread_t threadId;
    // Accept incoming connections and handle them using threads
    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        pthread_create(&threadId, NULL, &handleClient, (void*)&clientSocket);
    }

    close(serverSocket);
    return 0;
}
