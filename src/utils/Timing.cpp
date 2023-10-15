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
        //The number of frames to average
        static const int NUM_SAMPLES = 10;
        //Stores all the frametimes for each frame that we will average
        static float frame_times[NUM_SAMPLES];
        //The current frame we are on
        static int current_frame = 0;
        //the ticks of the previous frame
        static Uint32 prev_ticks = SDL_GetTicks();

        //Ticks for the current frame
        Uint32 current_ticks = SDL_GetTicks();

        //Calculate the number of ticks (ms) for this frame
        _frame_time = (float)(current_ticks - prev_ticks);
        frame_times[current_frame % NUM_SAMPLES] = _frame_time;

        //current ticks is now previous ticks
        prev_ticks = current_ticks;

        //The number of frames to average
        int count;

        current_frame++;
        if (current_frame < NUM_SAMPLES) {
            count = current_frame;
        } else {
            count = NUM_SAMPLES;
        }

        //Average all the frame times
        float frame_time_average = 0;
        for (int i = 0; i < count; i++) {
            frame_time_average += frame_times[i];
        }
        frame_time_average /= count;

        //Calculate FPS
        if (frame_time_average > 0) {
            _fps = 1000.0f / frame_time_average;
        } else {
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
