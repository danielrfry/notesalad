#pragma once

#include <cstdint>

#include "../Utils.h"

#define MFR_ID_LEN 3

#define SYSEX_CMD_IDENTIFY 0x01
#define SYSEX_CMD_IDENTIFY_REPLY 0x02
#define SYSEX_CMD_READPARAM 0x03
#define SYSEX_CMD_READPARAM_REPLY 0x04
#define SYSEX_CMD_WRITEGLOBALPARAM 0x05
#define SYSEX_CMD_STOREPATCH 0x06
#define SYSEX_CMD_READPATCH 0x07
#define SYSEX_CMD_READPATCH_REPLY 0x08
#define SYSEX_CMD_WRITEPATCH 0x09
#define SYSEX_CMD_WRITEPATCH_REPLY 0x0a
#define SYSEX_CMD_SETDRUMMODE 0x0b

#define SYSEX_DEV_ID_OPL2 0x01
#define SYSEX_DEV_ID_OPL3 0x02
#define SYSEX_DEV_ID_OPM 0x03
#define SYSEX_DEV_ID_SD1 0x04

#define SYSEX_STOREPATCH_FLAG_DRUM 0x01

#define SYSEX_PATCHADDR_TYPE_CHANNEL 0x00
#define SYSEX_PATCHADDR_TYPE_MELODICPROG 0x01
#define SYSEX_PATCHADDR_TYPE_DRUMPROG 0x02

#define SYSEX_READPATCH_RESULTCODE_OK 0x00
#define SYSEX_READPATCH_RESULTCODE_NOPATCH 0x01
#define SYSEX_READPATCH_RESULTCODE_BUFFEROVERFLOW 0x02

#define SYSEX_WRITEPATCH_RESULTCODE_OK 0x00
#define SYSEX_WRITEPATCH_RESULTCODE_INCORRECTDEVICEID 0x01
#define SYSEX_WRITEPATCH_RESULTCODE_INVALIDPATCHDATA 0x02
#define SYSEX_WRITEPATCH_RESULTCODE_INVALIDADDRESS 0x03
#define SYSEX_WRITEPATCH_RESULTCODE_NOSPACE 0x04

namespace SysEx {

extern const uint8_t MFR_ID[MFR_ID_LEN];

struct PACKED Header {
    uint8_t sysExMessage;
    uint8_t manufacturerID[MFR_ID_LEN];
    uint8_t cmd;
    Header();
    bool checkManufacturerID();
};

template <typename Content>
struct PACKED SysExMsg {
    Header header;
    Content content;
    uint8_t endMarker = 0xf7;
};

struct PACKED EmptyContent {
};

// ----------------------------------------------------------------------------

struct PACKED IdentifyMsg : public SysExMsg<EmptyContent> {
    IdentifyMsg();
};

// ----------------------------------------------------------------------------

struct PACKED IdentifyReplyContent {
    uint8_t deviceID;
};

struct PACKED IdentifyReplyMsg : SysExMsg<IdentifyReplyContent> {
    IdentifyReplyMsg(uint8_t deviceID);
};

// ----------------------------------------------------------------------------

struct PACKED ReadParamContent {
    uint8_t channel;
    uint8_t paramLSB;
    uint8_t paramMSB;
};

struct PACKED ReadParamMsg : SysExMsg<ReadParamContent> {
    ReadParamMsg();
    uint16_t getParam();
};

// ----------------------------------------------------------------------------

struct PACKED ReadParamReplyContent {
    uint8_t channel;
    uint8_t paramLSB;
    uint8_t paramMSB;
    uint8_t valueLSB;
    uint8_t valueMSB;
};

struct PACKED ReadParamReplyMsg : SysExMsg<ReadParamReplyContent> {
    ReadParamReplyMsg(uint8_t channel, uint16_t param, uint16_t value);
};

// ----------------------------------------------------------------------------

struct PACKED WriteGlobalParamContent {
    uint8_t paramLSB;
    uint8_t paramMSB;
    uint8_t valueLSB;
    uint8_t valueMSB;
    uint8_t endMarker;
};

struct PACKED WriteGlobalParamMsg : SysExMsg<WriteGlobalParamContent> {
    WriteGlobalParamMsg();
    uint16_t getParam();
    uint16_t getValue();
};

// ----------------------------------------------------------------------------

struct PACKED StorePatchContent {
    uint8_t srcChannel;
    uint8_t flags;
    uint8_t destBank;
    uint8_t destProgram;
    uint8_t destNoteNum;
};

struct PACKED StorePatchMsg : SysExMsg<StorePatchContent> {
    StorePatchMsg();
};

// ----------------------------------------------------------------------------

struct PACKED PatchAddress {
    uint8_t type;
    union {
        struct {
            uint8_t bank;
            uint8_t program;
        } melodicPatch;
        struct {
            uint8_t bank;
            uint8_t program;
            uint8_t noteNum;
        } drumPatch;
        uint8_t channel;
    };
    static PatchAddress forMelodicPatch(uint8_t bank, uint8_t program);
    static PatchAddress forDrumPatch(uint8_t bank, uint8_t program, uint8_t noteNum);
    static PatchAddress forChannel(uint8_t channel);
};

struct PACKED ReadPatchContent {
    PatchAddress srcAddress;
};

struct PACKED ReadPatchMsg : SysExMsg<ReadPatchContent> {
    ReadPatchMsg(PatchAddress srcAddress);
};

// ----------------------------------------------------------------------------

struct PACKED ReadPatchReplyMsgHeader {
    Header header;
    uint8_t resultCode;
};

// ----------------------------------------------------------------------------

struct PACKED WritePatchMsgHeader {
    Header header;
    PatchAddress destAddress;
    uint8_t deviceID;
};

// ----------------------------------------------------------------------------

struct PACKED WritePatchReplyContent {
    uint8_t resultCode;
};

struct PACKED WritePatchReplyMsg : SysExMsg<WritePatchReplyContent> {
    WritePatchReplyMsg(uint8_t resultCode);
};

// ----------------------------------------------------------------------------

struct PACKED SetDrumModeContent {
    uint8_t drumModeAndChannel;
};

struct PACKED SetDrumModeMsg : SysExMsg<SetDrumModeContent> {
    SetDrumModeMsg(uint8_t channel, bool drumMode);
    uint8_t getChannel();
    bool getDrumMode();
};

}
