#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

class SoundEffect
{
public:
    SoundEffect();
    ~SoundEffect() {}

    friend class AudioEngine;
    // Plays the sound effect file
    // default loops = 0 will only play once, -1 for infinite loop
    void play(int loops = 0);

private:
    Mix_Chunk *_chunk;
};

class Music
{
public:
    Music();
    ~Music() {}

    friend class AudioEngine;
    // Plays the music file
    // default loops = 0 will only play once, -1 for infinite loop
    void play(int loops = 0);
    static void pause();
    static void stop();
    static void resume();

private:
    Mix_Music *_music;
};

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    void init();
    void destroy();

    SoundEffect load_sound_effect(const std::string &file_path);
    Music load_music(const std::string &file_path);

private:
    std::map<std::string, Mix_Chunk*> _sound_effects;
    std::map<std::string, Mix_Music*> _music;

    bool _initialized;
};

#endif // AUDIOENGINE_H