# The compiler to use
CC      = g++

# Compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
LIBS    = -pthread --std=c++11
RM      = rm -rf

# The build target executable:
TARGET  = frame

# List of source files:
SRCS    = main.cpp Frame.cpp
OBJS    = $(SRCS:.cpp=.o)

# Default rule, to compile everything:
default: $(TARGET)
	@echo "Build complete"

# To create the executable file we need to link the object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# To create object files from cpp source files:
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# To start over from scratch, type 'make clean'. Removes all .o files and the executable file.
clean:
	$(RM) $(OBJS) $(TARGET)
	@echo "Clean complete"

# Define a phony target to avoid problems with files named clean
.PHONY: clean default