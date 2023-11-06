#include "Frame.h"
#include <iostream>

Frame::Frame() : registerAddress(0), readWriteCommand(false), lengthCode(0),
                 chipAddress(0), engineAddress(0), general(0), cnt(0),
                 completion(false), completionWithConditional(false), crc(0) {
    // Constructor implementation
}

Frame::~Frame() {
    // Destructor implementation
}

void Frame::setRegisterAddress(uint8_t address) {
    registerAddress = address;
}

void Frame::setReadWriteCommand(bool command) {
    readWriteCommand = command;
}


void Frame::setChipAddress(uint8_t address) {
    chipAddress = address;
}

void Frame::setEngineAddress(uint16_t address) {
    engineAddress = address;
}

void Frame::setData(const std::vector<uint16_t>& newData) {
    data = newData;
}

void Frame::setGeneral(uint8_t newGeneral) {
    general = newGeneral;
}

void Frame::setCnt(uint8_t newCnt) {
    cnt = newCnt;
}

void Frame::setCompletion(bool newCompletion) {
    completion = newCompletion;
}

void Frame::setCompletionWithConditional(bool newCC) {
    completionWithConditional = newCC;
}

void Frame::setLengthCode(uint8_t length) {
    lengthCode = length;
}

std::vector<uint8_t> Frame::encodeFrame() const {
    std::vector<uint8_t> frameBytes;

    // Add registerAddress to the byte stream
    frameBytes.push_back(registerAddress);

    // Previously, readWriteCommand was added as a full byte, but we're going to combine it with 'general' now.
    // The line adding readWriteCommand independently is removed.

    // Add lengthCode to the byte stream
    frameBytes.push_back(lengthCode);

    // Add chipAddress to the byte stream
    frameBytes.push_back(chipAddress);

    // Engine address might be two bytes
    frameBytes.push_back(static_cast<uint8_t>(engineAddress >> 8)); // high byte
    frameBytes.push_back(static_cast<uint8_t>(engineAddress & 0xFF)); // low byte

    // Assuming 'data' is an array of uint16_t, we need to add both bytes of each uint16_t to the stream
    for (const auto& word : data) {
        frameBytes.push_back(static_cast<uint8_t>(word >> 8)); // high byte
        frameBytes.push_back(static_cast<uint8_t>(word & 0xFF)); // low byte
    }

    // Combine general and readWriteCommand into a single byte.
    // The general takes the most significant 7 bits, readWriteCommand takes the least significant bit.
    frameBytes.push_back(static_cast<uint8_t>((general << 1) & 0xFE) | (readWriteCommand ? 0x01 : 0x00));

    // Add cnt to the byte stream
    frameBytes.push_back(cnt);

    // Add completion and completionWithConditional to the byte stream
    // If they are just bits, you may need to combine them as before, so this remains unchanged.
    uint8_t completionBytes = static_cast<uint8_t>(completion) | (static_cast<uint8_t>(completionWithConditional) << 1);
    frameBytes.push_back(completionBytes);

    // Add CRC to the byte stream (assuming 2 bytes, if it's actually 12 bits it might need special handling)
    uint16_t crcValue = calculateCRC(frameBytes);
    frameBytes.push_back(static_cast<uint8_t>(crcValue >> 8)); // high byte
    frameBytes.push_back(static_cast<uint8_t>(crcValue & 0xFF)); // low byte

    return frameBytes;
}


uint16_t Frame::calculateCRC(const std::vector<uint8_t>& frameData) const {
    const uint16_t polynomial = 0x8005;
    uint16_t crc = 0x0000;  // Start value
    for (auto byte : frameData) {
        crc ^= (byte << 8);  // Merge byte into CRC
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}


void Frame::decodeFrame(const std::vector<uint8_t>& frameData) {
    const size_t headerSize = 6; // Size of the header in bytes
    const size_t footerSize = 4; // Size of the footer in bytes (general, cnt, completion flags, CRC)

    if (frameData.size() < headerSize + footerSize) {
        std::cerr << "Error: Frame data is too short to contain a valid frame header and footer." << std::endl;
        return;
    }

    size_t index = 0;

    // Decode fixed-size fields from the header
    registerAddress = frameData[index++];
    readWriteCommand = frameData[index] & 0x01;
    general = frameData[index++] >> 1;
    lengthCode = frameData[index++];
    chipAddress = frameData[index++];
    engineAddress = static_cast<uint16_t>(frameData[index++] << 8);
    engineAddress |= frameData[index++];


    // Decode the variable length data field
    data.clear();
    for (size_t i = 0; i < 13; ++i) { // Changed the loop condition to iterate based on lengthCode
        if (index + 1 < frameData.size()) {
            uint16_t word = static_cast<uint16_t>(frameData[index++] << 8);
            word |= frameData[index++];
            data.push_back(word);
        } else {
            return;
        }
    }

    // Decode footer fields
    cnt = frameData[index++];

    uint8_t flags = frameData[index++];
    completion = flags & 0x01;
    completionWithConditional = (flags & 0x02) >> 1;
}





uint8_t Frame::getRegisterAddress() const {
    return registerAddress;
}

bool Frame::getReadWriteCommand() const {
    return readWriteCommand;
}

uint8_t Frame::getLengthCode() const {
    return lengthCode;
}

uint8_t Frame::getChipAddress() const {
    return chipAddress;
}

uint16_t Frame::getEngineAddress() const {
    return engineAddress;
}

std::vector<uint16_t> Frame::getData() const {
    return data;
}

uint8_t Frame::getGeneral() const {
    return general;
}

uint8_t Frame::getCnt() const {
    return cnt;
}

bool Frame::getCompletion() const {
    return completion;
}

bool Frame::getCompletionWithConditional() const {
    return completionWithConditional;
}