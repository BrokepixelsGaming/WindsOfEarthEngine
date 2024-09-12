#pragma once
#include "Component.h"
#include "..\3rdParty\include\SDL\SDL_mixer.h"
#include "ResourceSystem/Resource.h"

class Audio :
    public Component, public Resource
{
protected:
    StringId m_type = "Audio";
private:
    // Sound data.
    Mix_Chunk* m_pSound;

    // ID of the currently playing sound Channel.
    // Resets to -01 if no channel is playing. 
    int m_activeSoundChannel;

public:
    Audio();
    ~Audio();

    bool Load(const StringId& filename) override;

    // Handles the Event in the context of this object.
    void PlaySound();
    void PlaySoundOverride();
    void StopSound();
    void SetVolume(Uint8 level) { m_pSound->volume = level; }
};