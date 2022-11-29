# the compiler to use
CC      = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CCFLAGS =  
LIBS = -lzmq -pthread --std=c++11
RM      = rm -rf

default: all

all: main

main: main.cpp
	$(CC) main.cpp libs/database.cpp -o main.o $(LIBS)
	@echo "Build complete"
clean:
	$(RM) *.dSYM *.out main
	@echo "Clean complete"
