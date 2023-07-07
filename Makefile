SHELL := /bin/bash
CXXFLAGS = -pthread -Ijson/include
OUTPUT_CROSS = main_evb.bin
OUTPUT_HOST = main_host.bin
CLIENT_CROSS = client_evb.bin
CLIENT_HOST = client_host.bin
CROSS_OPTIONS = -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/opt/petalinux/2022.2/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi
CROSS_COMPILER ?= arm-xilinx-linux-gnueabi-g++
export CROSS_COMPILER

default: makecross makecrossclient

makecross: main.cpp
	unset LD_LIBRARY_PATH && source /opt/petalinux/2022.2/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi && $(CROSS_COMPILER) $(CXXFLAGS) $(CROSS_OPTIONS) -Og main.cpp zmq_server/zmq_server.cpp libs/database.cpp -g -o $(OUTPUT_CROSS) -lpthread -lzmq -lmysqlclient
	@echo "Build for cross-compilation complete"

makecrossclient: zmq_client.cpp
	unset LD_LIBRARY_PATH && source /opt/petalinux/2022.2/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi && $(CROSS_COMPILER) $(CXXFLAGS) $(CROSS_OPTIONS) -Og zmq_client.cpp -g -o $(CLIENT_CROSS) -lpthread -lzmq
	@echo "Build for cross-compilation client complete"

makepc: main.cpp
	$(CXX) $(CXXFLAGS) -Og main.cpp zmq_server/zmq_server.cpp libs/database.cpp -g -o $(OUTPUT_HOST) -lpthread -lzmq -lmysqlclient
	@echo "Build for PC complete"

makepcclient: zmq_client.cpp
	$(CXX) $(CXXFLAGS) -Og zmq_client.cpp -g -o $(CLIENT_HOST) -lpthread -lzmq
	@echo "Build for PC client complete"

clean:
	rm -f $(OUTPUT_CROSS) $(OUTPUT_HOST) $(CLIENT_CROSS) $(CLIENT_HOST)
	@echo "Clean complete"
