all: main.cpp
	$(CXX) $(CXXFLAGS) -Og main.cpp -g -o ${OUTPUT} -lpthread -mfpu=neon -mfloat-abi=hard 
clean:
	rm -f ${OUTPUT}