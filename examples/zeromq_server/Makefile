CXXFLAGS = -pthread
OUTPUT = server
CROSS_OPTIONS = -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/opt/petalinux/2022.2/sysroots/cortexa9t2hf-neon-xilinx-linux-gnueabi
CROSS_CXX = $(CROSS_COMPILER)

default: makecross

makecross: server.cpp
ifndef CROSS_COMPILER
	$(error CROSS_COMPILER is not set. Please set CROSS_COMPILER to your cross compiler, like: export CROSS_COMPILER=arm-xilinx-linux-gnueabi-g++)
endif
	$(CROSS_CXX) $(CXXFLAGS) $(CROSS_OPTIONS) -Og server.cpp -g -o $(OUTPUT) -lpthread -lzmq
	@echo "Build for cross-compilation complete"

makepc: server.cpp
	$(CXX) $(CXXFLAGS) -Og server.cpp -g -o $(OUTPUT) -lpthread -lzmq
	@echo "Build for PC complete"

clean:
	rm -f $(OUTPUT)
	@echo "Clean complete"
