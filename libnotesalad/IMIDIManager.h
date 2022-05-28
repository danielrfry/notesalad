#pragma once

#include <cstdint>
#include <functional>

class IMIDIManager {
public:
    virtual void reset() = 0;
    virtual void setReceiveCallback(std::function<void(void*, uint32_t)> callback) = 0;
    virtual void sendMIDI(uint8_t* msgData, uint32_t msgLength) = 0;
    virtual void setTime(uint32_t timeMS) = 0;
    virtual void update() = 0;
    virtual ~IMIDIManager();
};
