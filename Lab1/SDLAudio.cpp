#include "SDLAudio.h"

SDLAudio::SDLAudio()
{
    // init sound
    SDL_Init(SDL_INIT_AUDIO); // we don't actually need this step as its in the Display class

    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
        exit(-1);
    }
}

SDLAudio::~SDLAudio()
{
    SDL_Quit();
}

void SDLAudio::addSoundEffect(const char* path)
{
    Mix_Chunk* tmpChunk = Mix_LoadWAV(path);

    if (tmpChunk != nullptr)
    {
        mSoundEffectBank.push_back(tmpChunk);
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    }
}

void SDLAudio::addAudioTrack(const char* path)
{
    backgroundMusic = Mix_LoadMUS(path);

    if (backgroundMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void SDLAudio::playSoundEffect(const int which) const
{
    if (which > mSoundEffectBank.size() - 1)
    {
        std::cout << "Sound out of range.\n";
        return;
    }

    Mix_PlayChannel(-1, mSoundEffectBank[which], 0);

}

void SDLAudio::playAudioTrack()
{
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(backgroundMusic, -1);
    }
}
