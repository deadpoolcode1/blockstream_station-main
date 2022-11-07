# the compiler to use
CC      = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CCFLAGS =  
LIBS = -lzmq -pthread --std=c++11
RM      = rm -rf

default: client

client: client.cpp
	$(CC) client.cpp -o client.out $(LIBS)
	@echo "Build complete"
	
clean:
	$(RM) *.dSYM *.out
	@echo "Clean complete"
