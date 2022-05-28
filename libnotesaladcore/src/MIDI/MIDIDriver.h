#pragma once

#include "IMIDIDriver.h"
#include "INoteManager.h"
#include "IToneGenerator.h"
#include "IVoiceAllocator.h"
#include "MIDICommon.h"
#include "MonoNoteManager.h"
#include "Patch.h"
#include "PatchSerialization.h"
#include "PolyNoteManager.h"
#include "SysEx.h"
#include "TimeSource.h"
#include "ToneController.h"

#include <cstdint>
#include <functional>

typedef struct ChannelStatus {
    // Currently selected (N)RPN number
    uint16_t paramNum;
    // Current (N)RPN data
    uint16_t paramData;
    uint8_t polyMode;
} ChannelStatus;

template <class Tone, uint8_t voices>
class MIDIDriver : public IMIDIDriver {
public:
    MIDIDriver(IVoiceAllocator<Tone>* voiceAllocator, IToneGenerator<Tone>* toneGenerator,
        PatchManagerBase<Tone>* patchManager, TimeSource* timeSource, uint8_t deviceID, const ParamInfoList* chipParamsInfo)
        : IMIDIDriver(), deviceID(deviceID), voiceAllocator(voiceAllocator), toneGenerator(toneGenerator), patchManager(patchManager), timeSource(timeSource),
          toneController(voiceAllocator, toneGenerator, patchManager, timeSource),
          polyNoteManager(voiceAllocator, &toneController),
          monoNoteManager(voiceAllocator, &toneController, timeSource),
          chipParamsInfo(chipParamsInfo)
    {
        this->resetState();
    }
    virtual void reset(bool hardReset)
    {
        this->resetState();
        this->voiceAllocator->reset();
        this->toneController.resetState();
        this->polyNoteManager.resetState();
        this->monoNoteManager.resetState();
        this->toneGenerator->reset(hardReset);
    }
    virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        if (velocity == 0) {
            this->noteOff(channel, note, velocity);
        } else {
            this->updatePolyMode(channel);
            this->getNoteManager(channel)->noteOn(channel, note, velocity);
        }
    }
    virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        this->getNoteManager(channel)->noteOff(channel, note, velocity);
    }
    virtual void programChange(uint8_t channel, uint8_t program)
    {
        this->toneController.setProgram(channel, program);
    }
    virtual void controlChange(uint8_t channel, uint8_t control, uint8_t value)
    {
        if (control == 6) {
            // (N)RPN data MSB
            this->setParamData(true, channel, value);
        } else if (control == 7) {
            // Volume
            this->toneController.setVolume(channel, value);
        } else if (control == 10) {
            // Pan
            this->toneController.setPan(channel, value);
        } else if (control == 32) {
            // Bank select LSB
            this->toneController.setBank(channel, value);
        } else if (control == 38) {
            // (N)RPN data LSB
            this->setParamData(false, channel, value);
        } else if (control == 64) {
            // Sustain
            this->getNoteManager(channel)->setSustain(channel, value != 0);
        } else if (control == 98) {
            // NRPN select LSB
            this->setParamNum(false, false, channel, value);
        } else if (control == 99) {
            // NRPN select MSB
            this->setParamNum(true, false, channel, value);
        } else if (control == 100) {
            // RPN select LSB
            this->setParamNum(false, true, channel, value);
        } else if (control == 101) {
            // RPN select MSB
            this->setParamNum(true, true, channel, value);
        } else if (control == 123) {
            // All notes off
            if (value == 0) {
                this->getNoteManager(channel)->allNotesOff(channel);
            }
        } else {
            this->toneController.setController(channel, control, value & 0x7f);
        }
    }
    virtual void sysEx(uint8_t* data, unsigned int length)
    {
        if (length < sizeof(SysEx::Header))
            return;

        auto msg = (SysEx::Header*)data;

        switch (msg->cmd) {
        case SYSEX_CMD_IDENTIFY:
            this->sysExIdentify(data, length);
            break;
        case SYSEX_CMD_READPARAM:
            this->sysExReadParam(data, length);
            break;
        case SYSEX_CMD_WRITEGLOBALPARAM:
            this->sysExWriteGlobalParam(data, length);
            break;
        case SYSEX_CMD_STOREPATCH:
            this->sysExStorePatch(data, length);
            break;
        case SYSEX_CMD_READPATCH:
            this->sysExReadPatch(data, length);
            break;
        case SYSEX_CMD_WRITEPATCH:
            this->sysExWritePatch(data, length);
            break;
        }
    }
    virtual void pitchWheel(uint8_t channel, uint16_t value)
    {
        this->toneController.setPitchWheel(channel, value);
    }
    virtual void update()
    {
        this->monoNoteManager.update();
        this->toneController.update();
    }
    virtual bool writePatch(uint8_t srcChannel, uint8_t destBank, uint8_t destProgram, uint8_t destNoteNum = 255)
    {
        auto patch = this->toneController.getChannelPatch(srcChannel);
        return this->patchManager->setPatch(destBank, destProgram, destNoteNum, *patch);
    }
    Patch<Tone>* getChannelPatch(uint8_t channel)
    {
        return this->toneController.getChannelPatch(channel);
    }

private:
    uint8_t deviceID;
    IVoiceAllocator<Tone>* voiceAllocator;
    IToneGenerator<Tone>* toneGenerator;
    PatchManagerBase<Tone>* patchManager;
    TimeSource* timeSource;
    ToneController<Tone, voices> toneController;
    PolyNoteManager<Tone, voices> polyNoteManager;
    MonoNoteManager<Tone, voices> monoNoteManager;
    ChannelStatus channelStatuses[16];
    const ParamInfoList* chipParamsInfo;
    void resetState()
    {
        for (uint8_t c = 0; c < 16; c++) {
            this->channelStatuses[c] = ChannelStatus();
        }
    }
    INoteManager* getNoteManager(uint8_t channel)
    {
        switch (this->channelStatuses[channel].polyMode) {
        case POLYMODE_MONO:
        case POLYMODE_MONO_EXT:
        case POLYMODE_LEGATO:
        case POLYMODE_LEGATO_EXT:
            return &this->monoNoteManager;
        default:
            return &this->polyNoteManager;
        }
    }
    void updatePolyMode(uint8_t channel)
    {
        auto newPolyMode = this->toneController.getChannelPolyMode(channel);
        auto oldPolyMode = this->channelStatuses[channel].polyMode;
        if (oldPolyMode != newPolyMode) {
            this->getNoteManager(channel)->allNotesOff(channel);
            this->channelStatuses[channel].polyMode = newPolyMode;
        }
    }
    void setParamNum(bool msb, bool isRPN, uint8_t channel, uint8_t value)
    {
        auto& channelStatus = this->channelStatuses[channel];
        auto& paramNumber = channelStatus.paramNum;
        if (msb) {
            paramNumber = (isRPN ? 0x4000 : 0) | (paramNumber & ~0x7f80) | (value << 7);
        } else {
            paramNumber = (isRPN ? 0x4000 : 0) | (paramNumber & ~0x407f) | (value & 0x7f);
        }
        channelStatus.paramData = 0;
    }
    void setParamData(bool msb, uint8_t channel, uint8_t value)
    {
        auto& channelStatus = this->channelStatuses[channel];
        auto& paramData = channelStatus.paramData;
        if (msb) {
            paramData = (paramData & ~0x3f80) | (value << 7);
        } else {
            paramData = (paramData & ~0x407f) | (value & 0x7f);
        }
        bool isRPN = (channelStatus.paramNum & 0x4000) != 0;
        if (isRPN) {
            this->setRPNValue(channel, channelStatus.paramNum & 0x3fff, paramData, msb);
        } else {
            this->setNRPNValue(channel, channelStatus.paramNum & 0x3fff, paramData, msb);
        }
    }
    void setRPNValue(uint8_t channel, uint16_t paramNum, uint16_t value, bool settingMSB)
    {
        if (paramNum == 0) {
            this->toneController.setPitchWheelRange(channel, value);
        }
    }
    void setNRPNValue(uint8_t channel, uint16_t paramNum, uint16_t value, bool settingMSB)
    {
        if (!settingMSB) {
            this->toneController.setParam(channel, paramNum, value);
        }
    }
    void sendMIDI(void* msgData, uint32_t length)
    {
        if (this->onSendMIDI != nullptr) {
            this->onSendMIDI(msgData, length);
        }
    }
    void sysExIdentify(uint8_t* msgData, uint32_t length)
    {
        auto reply = SysEx::IdentifyReplyMsg(deviceID);
        this->sendMIDI(&reply, sizeof(reply));
    }
    void sysExReadParam(uint8_t* msgData, uint32_t length)
    {
        if (length == sizeof(SysEx::ReadParamMsg)) {
            auto msg = (SysEx::ReadParamMsg*)msgData;
            auto paramID = msg->getParam();
            auto value = this->toneController.getParam(msg->content.channel, paramID);
            auto reply = SysEx::ReadParamReplyMsg(msg->content.channel, paramID, value);
            this->sendMIDI(&reply, sizeof(reply));
        }
    }
    void sysExWriteGlobalParam(uint8_t* msgData, uint32_t length)
    {
        if (length == sizeof(SysEx::WriteGlobalParamMsg)) {
            auto msg = (SysEx::WriteGlobalParamMsg*)msgData;
            auto paramID = msg->getParam();
            auto value = msg->getValue();
            this->toneGenerator->setGlobalParam(paramID, value);
        }
    }
    void sysExStorePatch(uint8_t* msgData, uint32_t length)
    {
        if (length == sizeof(SysEx::StorePatchMsg)) {
            auto msg = (SysEx::StorePatchMsg*)msgData;
            bool isDrumPatch = (msg->content.flags & SYSEX_STOREPATCH_FLAG_DRUM) != 0;
            uint8_t destNoteNum = isDrumPatch ? msg->content.destNoteNum : 255;
            this->writePatch(msg->content.srcChannel, msg->content.destBank, msg->content.destProgram, destNoteNum);
        }
    }
    bool getPatchAtAddress(SysEx::PatchAddress addr, Patch<Tone>& outPatch)
    {
        if (addr.type == SYSEX_PATCHADDR_TYPE_CHANNEL) {
            if (addr.channel > 15)
                return false;
            outPatch = *this->toneController.getChannelPatch(addr.channel);
            return true;
        } else if (addr.type == SYSEX_PATCHADDR_TYPE_DRUMPROG) {
            return this->patchManager->getPatch(addr.drumPatch.bank, addr.drumPatch.program, addr.drumPatch.noteNum, outPatch);
        } else if (addr.type == SYSEX_PATCHADDR_TYPE_MELODICPROG) {
            return this->patchManager->getPatch(addr.melodicPatch.bank, addr.melodicPatch.program, 0xff, outPatch);
        }
        return false;
    }
    void sysExReadPatch(uint8_t* msgData, uint32_t length)
    {
        if (length < sizeof(SysEx::ReadPatchMsg)) {
            return;
        }

        auto msg = (SysEx::ReadPatchMsg*)msgData;

        Patch<Tone> patch;
        if (this->getPatchAtAddress(msg->content.srcAddress, patch)) {
            this->sendReadPatchReply(patch);
        } else {
            this->sendReadPatchReply(SYSEX_READPATCH_RESULTCODE_NOPATCH);
        }
    }
    void sendReadPatchReply(uint8_t resultCode)
    {
        struct PACKED {
            SysEx::ReadPatchReplyMsgHeader header;
            uint8_t endMarker = 0xf7;
        } reply;
        reply.header.header.cmd = SYSEX_CMD_READPATCH_REPLY;
        reply.header.resultCode = resultCode;

        this->sendMIDI(&reply, sizeof(reply));
    }
    void sendReadPatchReply(Patch<Tone>& patch)
    {
        ArrayWriter<512> replyBuffer;

        SysEx::ReadPatchReplyMsgHeader header;
        header.header.cmd = SYSEX_CMD_READPATCH_REPLY;
        header.resultCode = SYSEX_READPATCH_RESULTCODE_OK;

        if (!replyBuffer.IWriter::write(header)) {
            this->sendReadPatchReply(SYSEX_READPATCH_RESULTCODE_BUFFEROVERFLOW);
            return;
        }

        if (!replyBuffer.IWriter::write(deviceID)) {
            this->sendReadPatchReply(SYSEX_READPATCH_RESULTCODE_BUFFEROVERFLOW);
            return;
        }

        Encoder7Bit encoder(&replyBuffer);
        if (!PatchSerialization::serialize(*chipParamsInfo, patch, &encoder)) {
            this->sendReadPatchReply(SYSEX_READPATCH_RESULTCODE_BUFFEROVERFLOW);
            return;
        }
        if (!encoder.flush()) {
            this->sendReadPatchReply(SYSEX_READPATCH_RESULTCODE_BUFFEROVERFLOW);
            return;
        }

        if (!replyBuffer.IWriter::write(0xf7)) {
            this->sendReadPatchReply(SYSEX_READPATCH_RESULTCODE_BUFFEROVERFLOW);
            return;
        }

        this->sendMIDI(&replyBuffer.buffer[0], replyBuffer.getPosition());
    }
    void sysExWritePatch(uint8_t* msgData, uint32_t length)
    {
        if (length < sizeof(SysEx::WritePatchMsgHeader)) {
            return;
        }

        auto header = (SysEx::WritePatchMsgHeader*)msgData;
        if (header->deviceID != deviceID) {
            this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_INCORRECTDEVICEID);
            return;
        }

        BufferReader reader(msgData + sizeof(SysEx::WritePatchMsgHeader), length - sizeof(SysEx::WritePatchMsgHeader) - 1);
        Decoder7Bit decoder(&reader);
        Patch<Tone> patch;
        if (!PatchSerialization::deserialize(patch, &decoder)) {
            this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_INVALIDPATCHDATA);
            return;
        }

        if (header->destAddress.type == SYSEX_PATCHADDR_TYPE_CHANNEL) {
            auto channel = header->destAddress.channel;
            if (channel > 15) {
                this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_INVALIDADDRESS);
                return;
            }
            this->toneController.setPatch(channel, patch);
        } else if (header->destAddress.type == SYSEX_PATCHADDR_TYPE_MELODICPROG) {
            auto bank = header->destAddress.melodicPatch.bank;
            auto program = header->destAddress.melodicPatch.program;
            if (!this->patchManager->setPatch(bank, program, 255, patch)) {
                this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_NOSPACE);
                return;
            }
        } else if (header->destAddress.type == SYSEX_PATCHADDR_TYPE_DRUMPROG) {
            auto bank = header->destAddress.drumPatch.bank;
            auto program = header->destAddress.drumPatch.program;
            auto noteNum = header->destAddress.drumPatch.noteNum;
            if (!this->patchManager->setPatch(bank, program, noteNum, patch)) {
                this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_NOSPACE);
                return;
            }
        } else {
            this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_INVALIDADDRESS);
            return;
        }

        this->sendWritePatchReply(SYSEX_WRITEPATCH_RESULTCODE_OK);
    }
    void sendWritePatchReply(uint8_t resultCode)
    {
        auto reply = SysEx::WritePatchReplyMsg(resultCode);
        this->sendMIDI(&reply, sizeof(reply));
    }
};
