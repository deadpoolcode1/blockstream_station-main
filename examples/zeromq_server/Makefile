# the compiler to use
CC      = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CCFLAGS =  
LIBS = -lzmq -pthread --std=c++11
RM      = rm -rf

default: server

server: server.cpp
	$(CC) server.cpp -o server $(LIBS)
	@echo "Build complete"
	
clean:
	$(RM) *.dSYM *.out server
	@echo "Clean complete"
