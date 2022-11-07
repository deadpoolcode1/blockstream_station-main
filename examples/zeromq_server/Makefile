# the compiler to use
CC      = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CCFLAGS =  
LIBS = -lzmq -pthread --std=c++11
RM      = rm -rf

default: server

client: client.cpp
	$(CC) server.cpp -o server.out $(LIBS)
	@echo "Build complete"
	
clean:
	$(RM) *.dSYM *.out
	@echo "Clean complete"
