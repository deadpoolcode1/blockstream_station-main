SHELL := /bin/bash
CXXFLAGS = -pthread
OUTPUT_CROSS = server_evb.bin
OUTPUT_HOST = server_host.bin
CROSS_OPTIONS = -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/opt/petalinux/2022.2/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi
CROSS_COMPILER ?= arm-xilinx-linux-gnueabi-g++
export CROSS_COMPILER

default: makecross

makecross: server.cpp
	unset LD_LIBRARY_PATH && source /opt/petalinux/2022.2/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi && $(CROSS_COMPILER) $(CXXFLAGS) $(CROSS_OPTIONS) -Og server.cpp -g -o $(OUTPUT_CROSS) -lpthread -lzmq
	@echo "Build for cross-compilation complete"

makepc: server.cpp
	$(CXX) $(CXXFLAGS) -Og server.cpp -g -o $(OUTPUT_HOST) -lpthread -lzmq
	@echo "Build for PC complete"

clean:
	rm -f $(OUTPUT)
	@echo "Clean complete"
