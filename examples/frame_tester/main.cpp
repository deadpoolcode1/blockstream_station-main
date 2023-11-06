#include "Frame.h"
#include <iostream>
#include <iomanip>

// Function to print encoded frame in hexadecimal
void printEncodedFrame(const std::vector<uint8_t>& frame) {
    std::cout << "Encoded Frame: ";
    for (auto byte : frame) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl << std::dec;
}

// Function to print the frame data after decoding
void printDecodedFrame(const Frame& frame) {
    std::cout << "Decoded Frame: " << std::endl;
    std::cout << "Register Address: " << std::hex << static_cast<int>(frame.getRegisterAddress()) << std::endl;
    std::cout << "Read/Write Command: " << (frame.getReadWriteCommand() ? "Read" : "Write") << std::endl;
    std::cout << "Length Code: " << static_cast<int>(frame.getLengthCode()) << std::endl;
    std::cout << "Chip Address: " << static_cast<int>(frame.getChipAddress()) << std::endl;
    std::cout << "Engine Address: " << std::hex << frame.getEngineAddress() << std::endl;
    std::cout << "Data: ";
    for (auto word : frame.getData()) {
        std::cout << std::hex << word << " ";
    }
    std::cout << std::endl;
    std::cout << "General: " << static_cast<int>(frame.getGeneral()) << std::endl;
    std::cout << "CNT: " << static_cast<int>(frame.getCnt()) << std::endl;
    std::cout << "Completion: " << (frame.getCompletion() ? "True" : "False") << std::endl;
    std::cout << "Completion With Conditional: " << (frame.getCompletionWithConditional() ? "True" : "False") << std::endl;
    std::cout << std::dec; // Reset stream to decimal output
}

int main() {
    // Instantiate a Frame object
    Frame myFrame;

    // Set frame data using example values
    myFrame.setRegisterAddress(0x01);
    myFrame.setReadWriteCommand(true);

    // Corrected the length code to 0x02 as there are 2 data words
    myFrame.setLengthCode(13);

    myFrame.setChipAddress(0x12);
    myFrame.setEngineAddress(0x3FF);
    myFrame.setData({0xAAAA, 0x5555});
    myFrame.setGeneral(0x0F);
    myFrame.setCnt(0x03);
    myFrame.setCompletion(true);
    myFrame.setCompletionWithConditional(false);

    // Encode the frame
    std::vector<uint8_t> encodedFrame = myFrame.encodeFrame();

    // Print encoded frame
    printEncodedFrame(encodedFrame);

    // Decode the frame
    Frame receivedFrame;
    receivedFrame.decodeFrame(encodedFrame);

    // Print decoded frame
    printDecodedFrame(receivedFrame);

    return 0; // Successful execution
}