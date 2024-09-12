#include "Audio.h"

#include "EngineInterface/EngineSingleton.h"

///////////////////////////////////////////////////////////////
// Sound object effect to use with the mixer channel. 
// :: This can load WAVE, AIFF, RIFF, OGG, and VOC files.
///////////////////////////////////////////////////////////////
Audio::Audio()
	: m_pSound(nullptr)
	, m_activeSoundChannel(-1)
{
}

Audio::~Audio()
{
	if (m_pSound != nullptr)
	{
		Mix_FreeChunk(m_pSound);
		m_pSound = nullptr;
	}
}

bool Audio::Load(const StringId& filename)
{
	m_pSound = Mix_LoadWAV(filename);
	if (m_pSound == nullptr)
	{
		//auto& lua = EngineSingleton::Get()->GetLuaState();
		//lua.DoString("Failed to load sound. Error: ");
		//lua.DoString(SDL_GetError());
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////
// Will play the sound as long as it's not already playing. 
///////////////////////////////////////////////////////////////
void Audio::PlaySound()
{
	if (m_activeSoundChannel == -1)
	{
		m_activeSoundChannel = Mix_PlayChannel(-1, m_pSound, 0);
	}
	else if (Mix_Playing(m_activeSoundChannel) == 0)
	{
		m_activeSoundChannel = -1;
	}
}

///////////////////////////////////////////////////////////////
// Use this when you want overlapping sounds. This will just use the first free channel. 
///////////////////////////////////////////////////////////////
void Audio::PlaySoundOverride()
{
	Mix_PlayChannel(-1, m_pSound, 0);
}

void Audio::StopSound()
{
	Mix_HaltChannel(m_activeSoundChannel);
	m_activeSoundChannel = -1;
	
}
