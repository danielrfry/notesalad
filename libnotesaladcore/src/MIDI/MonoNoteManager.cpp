#include "MonoNoteManager.h"

bool shouldGlideFromLastNote(uint8_t polyMode)
{
    return (polyMode == POLYMODE_MONO_EXT || polyMode == POLYMODE_LEGATO_EXT);
}

bool isLegato(uint8_t polyMode)
{
    return (polyMode == POLYMODE_LEGATO || polyMode == POLYMODE_LEGATO_EXT);
}

bool MonoChannelStatus::isSustainActive()
{
    return this->flags & MONO_FLAG_SUSTAINACTIVE;
}

void MonoChannelStatus::setSustainActive(bool newSustainActive)
{
    if (newSustainActive)
        this->flags |= MONO_FLAG_SUSTAINACTIVE;
    else
        this->flags &= ~MONO_FLAG_SUSTAINACTIVE;
}

bool MonoChannelStatus::isNotePlaying()
{
    return this->flags & MONO_FLAG_NOTE_PLAYING;
}

void MonoChannelStatus::setNotePlaying(bool newNotePlaying)
{
    if (newNotePlaying)
        this->flags |= MONO_FLAG_NOTE_PLAYING;
    else
        this->flags &= ~MONO_FLAG_NOTE_PLAYING;
}
