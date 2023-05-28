CXXFLAGS = -pthread

OUTPUT = main

makepc: main.cpp
	$(CXX) $(CXXFLAGS) -Og main.cpp -g -o ${OUTPUT} -lpthread

makecross: main.cpp
	$(CXX) $(CXXFLAGS) -Og main.cpp -g -o ${OUTPUT} -lpthread -mfpu=neon -mfloat-abi=hard 

clean:
	rm -f ${OUTPUT}