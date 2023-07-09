# ZMQ Server Project

This project provides a basic ZeroMQ server application that communicates over IPC and processes incoming JSON messages.

## Dependencies

This project depends on:

- ZeroMQ
- nlohmann::json

You can install these dependencies using your system's package manager or from their respective sources.

## Building the Project

The Makefile included in this project supports both cross-compilation for ARM devices and native compilation for your PC.

**To build for an ARM device:**
```bash
make makecross
```
**To build for a PC:**
```bash
make makepc
```
## Usage

You can start the server application by running:
```bash
./main_host.bin
```
To send a message to the server, you can use the provided client application:
```bash
./client_host.bin '<json message>'
```
The server will print out the received message and respond based on the command specified in the JSON message.

## Commands

Commands should be provided to the server in the format of JSON objects:

```json
{
    "command": "<command_name>"
}
```
Currently, the server supports the following command:

- `get_app_version` - Upon receiving this command, the server responds with the current version number of the application.

**For example:**

```bash
./client_host.bin '{"command":"get_app_version"}'
```
The server will then respond with the current version of the application.

- `read_i2c_register` - This command reads the value from a specific register on an I2C device.

**For example:**

```bash
./client_host.bin '{
    "command": "read_i2c_register",
    "parameters": {
        "bus": <bus_number>,
        "address": <device_address>,
        "reg": <register_address>
    }
}'
```
The server responds with the value read from the specified register.

- `write_i2c_register` - This command reads the value from a specific register on an I2C device.

**For example:**

```bash
./client_host.bin '{
    "command": "write_i2c_register",
    "parameters": {
        "bus": <bus_number>,
        "address": <device_address>,
        "reg": <register_address>,
        "reg": value
    }
}'
```
The server responds with the value read from the specified register.