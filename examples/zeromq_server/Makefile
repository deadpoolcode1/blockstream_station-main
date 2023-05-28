CXXFLAGS = -pthread
OUTPUT = server
CROSS_OPTIONS = -mfpu=neon -mfloat-abi=hard
CROSS_COMPILE = $(shell $(CXX) $(CROSS_OPTIONS) -x c++ -E - </dev/null >/dev/null 2>&1 && echo "yes")

default: makecross

makecross: server.cpp
	./cross_compile.sh $(CROSS_COMPILE) $(CXX) $(CXXFLAGS) -Og server.cpp -g -o ${OUTPUT} -lpthread -lzmq
	@echo "Build for cross-compilation complete"

makepc: server.cpp
	unset LD_LIBRARY_PATH && $(CXX) $(CXXFLAGS) -Og server.cpp -g -o ${OUTPUT} -lpthread -lzmq
	@echo "Build for PC complete"


clean:
	rm -f ${OUTPUT}
	@echo "Clean complete"
