#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>

enum window_flags
{
    INVISIBLE = 0x1,
    FULLSCREEN = 0x2,
    BORDERLESS = 0x4
};

class Window
{
public:
    Window();
    ~Window();

    int create(std::string window_name, int screen_width, int screen_height, unsigned int current_flags);

    void swap_buffers();

    int get_screen_width() { return _screen_width; }
    int get_screen_height() { return _screen_height; }

private:
    SDL_Window *_window;
    int _screen_width;
    int _screen_height;


};


#endif