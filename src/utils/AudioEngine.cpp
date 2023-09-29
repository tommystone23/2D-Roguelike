#include "AudioEngine.h"
#include "Errors.h"

SoundEffect::SoundEffect()
    : _chunk(NULL)
{

}

void SoundEffect::play(int loops)
{
    if(Mix_PlayChannel(-1, _chunk, loops) < 0) {
        // Try again and force through first channel
        if(Mix_PlayChannel(0, _chunk, loops) < 0) {
            fatal_error("Mix_PlayChannel failed:" + std::string(Mix_GetError()));
        }
    }
}

Music::Music()
    : _music(NULL)
{

}

void Music::play(int loops)
{
    if(Mix_PlayMusic(_music, loops) < 0) {
        fatal_error("Mix_PlayMusic failed:" + std::string(Mix_GetError()));
    }
}

void Music::pause()
{
    Mix_PauseMusic();
}

void Music::stop()
{
    Mix_HaltMusic();
}

void Music::resume()
{
    Mix_ResumeMusic();
}

AudioEngine::AudioEngine()
    : _initialized(false)
{
}

AudioEngine::~AudioEngine()
{
    destroy();
}

void AudioEngine::init()
{
    if(_initialized)
        fatal_error("Tried to init audio twice");
        
    // Parameter can be bitwise combination of
    // MIX_INIT_FAC, MIX_INIT_MP3, MIX_INIT_OGG
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0) {
        fatal_error("Mix_Init failed:" + std::string(Mix_GetError()));
    }

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        fatal_error("Mix_Init failed:" + std::string(Mix_GetError()));
    }

    _initialized = true;
}

void AudioEngine::destroy()
{
    if(_initialized) {
        _initialized = false;

        for(auto &it : _sound_effects) {
            Mix_FreeChunk(it.second);
        }
        for(auto &it : _music) {
            Mix_FreeMusic(it.second);
        }

        Mix_CloseAudio();
        Mix_Quit();
    }
}

SoundEffect AudioEngine::load_sound_effect(const std::string &file_path)
{
    // Check for sound effect already loaded
    auto it = _sound_effects.find(file_path);
    SoundEffect effect;

    if(it == _sound_effects.end()) {
        // Need to load effect
        Mix_Chunk *chunk = Mix_LoadWAV(file_path.c_str());
        if(!chunk)
            fatal_error("Mix_LoadWAV failed:" + std::string(Mix_GetError()));

        effect._chunk = chunk;
        _sound_effects[file_path] = chunk;
    }
    else {
        effect._chunk = it->second;
    }

    return effect;
}

Music AudioEngine::load_music(const std::string &file_path)
{
    // Check for music already loaded
    auto it = _music.find(file_path);
    Music music;

    if(it == _music.end()) {
        // Need to load effect
        Mix_Music *music_file = Mix_LoadMUS(file_path.c_str());
        if(!music_file)
            fatal_error("Mix_LoadMUS failed:" + std::string(Mix_GetError()));

        music._music = music_file;
        _music[file_path] = music_file;
    }
    else {
        music._music = it->second;
    }

    return music;
}
