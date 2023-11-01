#ifndef MAINGAME_H
#define MAINGAME_H

#include "utils/Window.h"
#include "utils/Camera2D.h"
#include "utils/ScreenList.h"
#include "gameplayscreen.h"
#include "utils/InputManager.h"

class MainGame
{
public:
    MainGame();
    ~MainGame();

    void run();

    // getters
    Window* get_window() { return &_window; }
    Camera2D* get_camera() { return &_camera; }
    InputManager* get_input_manager() { return &_input_manager; }
    float get_fps() { return _fps; }

private:
    bool _is_running;
    Window _window;
    Camera2D _camera;
    ScreenList *_screens;
    IGameScreen *_current_screen;

    InputManager _input_manager;

    float _fps;

    void init();
    void game_loop();
    void user_input();
};

#endif // MAINGAME_H