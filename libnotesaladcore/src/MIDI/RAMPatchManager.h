#include "Patch.h"
#include "PatchManagerBase.h"

template <class Tone>
struct PatchEntry {
    PatchEntry() : bank(0xff), program(0xff), noteNum(0xff), patch() { }
    PatchEntry(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<Tone>& patch)
        : bank(bank), program(program), noteNum(noteNum), patch(patch) { }
    uint8_t bank;
    uint8_t program;
    uint8_t noteNum;
    Patch<Tone> patch;
};

template <class Tone, int numPatches>
class RAMPatchManager : public PatchManagerBase<Tone> {
public:
    RAMPatchManager(PatchManagerBase<Tone>* fallbackPatchManager) : fallbackPatchManager(fallbackPatchManager), patches()
    {
    }

    virtual bool supportsDrums() override
    {
        return true;
    }

    virtual bool getPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<Tone>& outPatch) override
    {
        auto i = this->findPatchEntry(bank, program, noteNum);
        if (i < 0) {
            if (this->fallbackPatchManager) {
                return this->fallbackPatchManager->getPatch(bank, program, noteNum, outPatch);
            } else {
                return false;
            }
        } else {
            outPatch = this->patches[i].patch;
            return true;
        }
    }

    virtual bool setPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<Tone>& patch) override
    {
        // Look for an existing patch to overwrite
        auto i = this->findPatchEntry(bank, program, noteNum);
        // Look for an empty space
        if (i < 0) {
            i = this->findPatchEntry(0xff, 0xff, 0xff);
        }
        if (i < 0) {
            // No space left
            return false;
        }

        this->patches[i] = PatchEntry<Tone>(bank, program, noteNum, patch);

        return true;
    }

    virtual bool deletePatch(uint8_t bank, uint8_t program, uint8_t noteNum) override
    {
        auto i = this->findPatchEntry(bank, program, noteNum);
        if (i < 0) {
            return false;
        } else {
            this->patches[i] = PatchEntry<Tone>();
            return true;
        }
    }

private:
    PatchManagerBase<Tone>* fallbackPatchManager;
    PatchEntry<Tone> patches[numPatches];

    int findPatchEntry(uint8_t bank, uint8_t program, uint8_t noteNum)
    {
        for (int i = 0; i < numPatches; i++) {
            auto& aPatch = this->patches[i];
            if (aPatch.bank == bank && aPatch.program == program && aPatch.noteNum == noteNum) {
                return i;
            }
        }
        return -1;
    }
};
