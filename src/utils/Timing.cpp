#include "Timing.h"
#include <SDL2/SDL.h>

FPSLimiter::FPSLimiter()
{
}

FPSLimiter::~FPSLimiter()
{
}

void FPSLimiter::init(float max_fps)
{
    set_max_fps(max_fps);
}

void FPSLimiter::set_max_fps(float max_fps)
{
    _max_fps = max_fps;
}

void FPSLimiter::calc_fps()
{
    static const int NUM_SAMPLES = 10;
    static float frame_times[NUM_SAMPLES];
    static int cur_frame = 0;

    static float prev_ticks = SDL_GetTicks();
    float cur_ticks = SDL_GetTicks();

    _frame_time = cur_ticks - prev_ticks;
    frame_times[cur_frame % NUM_SAMPLES] = _frame_time;
    prev_ticks = cur_ticks;
    cur_frame++;

    int count;

    if(cur_frame < NUM_SAMPLES)
    {
        count = cur_frame;
    }
    else 
    {
        count = NUM_SAMPLES;
    }

    float frame_time_average = 0;

    for(int i = 0; i < count; i++)
    {
        frame_time_average += frame_times[i];
    }

    frame_time_average /= count;

    if(frame_time_average > 0)
    {
        _fps = 1000.0f / frame_time_average;
    }
    else 
    {
        _fps = 60.0f;
    }
}

void FPSLimiter::begin()
{
    _start_ticks = SDL_GetTicks();
}

float FPSLimiter::end()
{
    calc_fps();

    float frame_ticks = SDL_GetTicks() - _start_ticks;
    // Limit the FPS to max FPS
    if(1000.0f / _max_fps > frame_ticks)
    {
        SDL_Delay(1000.0f / _max_fps - frame_ticks);
    }

    return _fps;
}
