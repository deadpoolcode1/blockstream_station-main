ZMQ Server Project
This project provides a basic ZeroMQ server application that communicates over IPC and processes incoming JSON messages.

Dependencies
The project depends on:

ZeroMQ
nlohmann::json
You can install these dependencies using your system's package manager or from their respective sources.

Building
The Makefile included in this project supports both cross-compilation for ARM devices and native compilation for your PC.

To build for an ARM device:

bash
Copy code
make makecross
To build for a PC:

bash
Copy code
make makepc
Usage
You can start the server application by running:

bash
Copy code
./main_host.bin
To send a message to the server, you can use the provided client application:

bash
Copy code
./client_host.bin '<json message>'
The server will print out the received message and respond based on the command specified in the JSON message.

Commands
Commands are to be provided to the server in the format of JSON objects:

json
Copy code
{
    "command": "<command_name>"
}
The following commands are currently supported by the server:

get_app_version - Upon receiving this command, the server responds with the current version number of the application.
Here is an example of how to use it:

bash
Copy code
./client_host.bin '{"command":"get_app_version"}'
In response, the server will send back the current version of the application.
