#pragma once
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <iostream>
#include <vector>

class SDLAudio
{
	public:
		SDLAudio();
		~SDLAudio();
		
		void addSoundEffect(const char* path);
		void addAudioTrack(const char* path);
		void playSoundEffect(const int which) const;
		void playAudioTrack();

	private:
		Mix_Music* backgroundMusic;
		std::vector<Mix_Chunk*> mSoundEffectBank;
};

