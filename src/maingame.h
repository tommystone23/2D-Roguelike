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