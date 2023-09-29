#include "Window.h"
#include "Errors.h"

Window::Window()
{

}

Window::~Window()
{

}

int Window::create(std::string window_name, int screen_width, int screen_height, unsigned int current_flags)
{
    _screen_width = screen_width;
    _screen_height = screen_height;

    Uint32 flags = SDL_WINDOW_OPENGL;

    if(current_flags & INVISIBLE)
    {
        flags |= SDL_WINDOW_HIDDEN;
    }
    if(current_flags & FULLSCREEN)
    {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if(current_flags & BORDERLESS)
    {
        flags |= SDL_WINDOW_BORDERLESS;
    }

    _window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                _screen_width, _screen_height, flags);

    if (!_window)
    {
        fatal_error("Failed to create SDL window");
    }

    // Create Context
    SDL_GLContext context = SDL_GL_CreateContext(_window);
    if (!context)
    {
        fatal_error("Failed to create context");
    }

    // Make Current Context
    SDL_GL_MakeCurrent(_window, context);
    
    glViewport(0, 0, _screen_width, _screen_height);

    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        fatal_error("Error initializing GLEW");
    }

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Turn Vsync on
    SDL_GL_SetSwapInterval(1);

    // Print OpenGL Version
    printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void Window::swap_buffers()
{
    SDL_GL_SwapWindow(_window);
}