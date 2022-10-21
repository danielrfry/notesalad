#include "SysEx.h"

#include <cstring>

const uint8_t SysEx::MFR_ID[MFR_ID_LEN] = { 0x00, 0x22, 0x53 };

SysEx::Header::Header()
{
    // SysEx start
    this->sysExMessage = 0xf0;
    this->cmd = 0;
    // Manufacturer ID
    memcpy(&this->manufacturerID[0], &SysEx::MFR_ID[0], MFR_ID_LEN);
}

bool SysEx::Header::checkManufacturerID()
{
    return memcmp(&this->manufacturerID[0], &SysEx::MFR_ID[0], MFR_ID_LEN);
}

SysEx::IdentifyMsg::IdentifyMsg() : SysExMsg()
{
    header.cmd = SYSEX_CMD_IDENTIFY;
}

SysEx::IdentifyReplyMsg::IdentifyReplyMsg(uint8_t deviceID) : SysExMsg()
{
    header.cmd = SYSEX_CMD_IDENTIFY_REPLY;
    content.deviceID = deviceID;
}

SysEx::ReadParamMsg::ReadParamMsg()
{
    header.cmd = SYSEX_CMD_READPARAM;
}

uint16_t SysEx::ReadParamMsg::getParam()
{
    return ((content.paramMSB & 0x7f) << 7) | (content.paramLSB & 0x7f);
}

SysEx::ReadParamReplyMsg::ReadParamReplyMsg(uint8_t channel, uint16_t param, uint16_t value) : SysExMsg()
{
    header.cmd = SYSEX_CMD_READPARAM_REPLY;

    content.channel = channel;
    content.paramLSB = param & 0x7f;
    content.paramMSB = (param >> 7) & 0x7f;
    content.valueLSB = value & 0x7f;
    content.valueMSB = (value >> 7) & 0x7f;
}

SysEx::WriteGlobalParamMsg::WriteGlobalParamMsg() : SysExMsg()
{
    header.cmd = SYSEX_CMD_WRITEGLOBALPARAM;
}

uint16_t SysEx::WriteGlobalParamMsg::getParam()
{
    return ((content.paramMSB & 0x7f) << 7) | (content.paramLSB & 0x7f);
}

uint16_t SysEx::WriteGlobalParamMsg::getValue()
{
    return ((content.valueMSB & 0x7f) << 7) | (content.valueLSB & 0x7f);
}

SysEx::StorePatchMsg::StorePatchMsg() : SysExMsg()
{
    header.cmd = SYSEX_CMD_STOREPATCH;
}

SysEx::PatchAddress SysEx::PatchAddress::forMelodicPatch(uint8_t bank, uint8_t program)
{
    PatchAddress addr;
    addr.type = SYSEX_PATCHADDR_TYPE_MELODICPROG;
    addr.melodicPatch.bank = bank;
    addr.melodicPatch.program = program;
    return addr;
}

SysEx::PatchAddress SysEx::PatchAddress::forDrumPatch(uint8_t bank, uint8_t program, uint8_t noteNum)
{
    PatchAddress addr;
    addr.type = SYSEX_PATCHADDR_TYPE_DRUMPROG;
    addr.drumPatch.bank = bank;
    addr.drumPatch.program = program;
    addr.drumPatch.noteNum = noteNum;
    return addr;
}

SysEx::PatchAddress SysEx::PatchAddress::forChannel(uint8_t channel)
{
    PatchAddress addr;
    addr.type = SYSEX_PATCHADDR_TYPE_CHANNEL;
    addr.channel = channel;
    return addr;
}

SysEx::ReadPatchMsg::ReadPatchMsg(PatchAddress srcAddress)
{
    header.cmd = SYSEX_CMD_READPATCH;
    content.srcAddress = srcAddress;
}

SysEx::WritePatchReplyMsg::WritePatchReplyMsg(uint8_t resultCode)
{
    header.cmd = SYSEX_CMD_WRITEPATCH_REPLY;
    content.resultCode = resultCode;
}

SysEx::SetDrumModeMsg::SetDrumModeMsg(uint8_t channel, bool drumMode)
{
    header.cmd = SYSEX_CMD_SETDRUMMODE;
    content.drumModeAndChannel = (channel & 0x0f) | (drumMode ? 0x40 : 0);
}

uint8_t SysEx::SetDrumModeMsg::getChannel()
{
    return content.drumModeAndChannel & 0x0f;
}

bool SysEx::SetDrumModeMsg::getDrumMode()
{
    return (content.drumModeAndChannel & 0x40) != 0;
}