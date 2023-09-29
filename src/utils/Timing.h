#ifndef TIMING_H
#define TIMING_H
class FPSLimiter
{
public:
    FPSLimiter();
    ~FPSLimiter();

    void init(float max_fps);

    void begin();
    // Returns current fps
    float end();

private:
    float _max_fps;
    float _frame_time;
    float _fps;
    unsigned int _start_ticks;

    void set_max_fps(float max_fps);
    void calc_fps();
};

#endif