#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <cstdint>

class Frame {
public:
    Frame();
    ~Frame();

    // Methods for setting the fields
    void setRegisterAddress(uint8_t address);
    void setReadWriteCommand(bool command);
    void setChipAddress(uint8_t address);
    void setEngineAddress(uint16_t address);
    void setData(const std::vector<uint16_t>& data);
    void setGeneral(uint8_t general);
    void setCnt(uint8_t cnt);
    void setCompletion(bool completion);
    void setCompletionWithConditional(bool cc);
    void setLengthCode(uint8_t length);

    // Getter method declarations
    uint8_t getRegisterAddress() const;
    bool getReadWriteCommand() const;
    uint8_t getLengthCode() const;
    uint8_t getChipAddress() const;
    uint16_t getEngineAddress() const;
    std::vector<uint16_t> getData() const;
    uint8_t getGeneral() const;
    uint8_t getCnt() const;
    bool getCompletion() const;
    bool getCompletionWithConditional() const;

    // Method for encoding the frame
    std::vector<uint8_t> encodeFrame() const;

    // Method for decoding the frame
    void decodeFrame(const std::vector<uint8_t>& frameData);
    uint16_t calculateCRC(const std::vector<uint8_t>& frameData) const;


private:
    uint8_t registerAddress;
    bool readWriteCommand;
    uint8_t lengthCode;
    uint8_t chipAddress;
    uint16_t engineAddress;
    std::vector<uint16_t> data;
    uint8_t general;
    uint8_t cnt;
    bool completion;
    bool completionWithConditional;
    uint16_t crc;

};

#endif // FRAME_H