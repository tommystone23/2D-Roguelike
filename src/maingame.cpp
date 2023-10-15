#include "maingame.h"
#include "utils/utils.h"
#include "gameplayscreen.h"
#include "utils/Timing.h"
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080

MainGame::MainGame()
    : _is_running(true)
{
    _screens = new ScreenList(this);
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
    init();
    game_loop();
}

void MainGame::init()
{
    // Init engine (Inits SDL and SDL attributes)
    Utils::init();
    // Init SDL window and GLEW
    _window.create("Roguelike", DEFAULT_WIDTH, DEFAULT_HEIGHT, 0);
    // Init camera
    _camera.init(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    _camera.set_position(glm::vec2(0.0f));
    _camera.set_scale(1.0f);

    // Add screens
    IGameScreen *game_screen = new GameplayScreen(&_window, &_camera, &_input_manager);
    _screens->add_screen(game_screen);
    _screens->set_screen(game_screen->get_screen_index());
    
    _current_screen = _screens->get_current_screen();
    _current_screen->on_entry();
}

void MainGame::game_loop()
{
    const float DESIRED_FPS = 60.0f;
    const int MAX_PHYSICS_STEPS = 6;
    const float MAX_DELTA_TIME = 1.0f;

    FPSLimiter fps;
    // Set max fps
    fps.init(DESIRED_FPS);

    const float MS_PER_SECOND = 1000;
    const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_FPS;

    float previous_ticks = SDL_GetTicks64();

    Uint64 time_taken;
    while(_is_running)
    {
        float new_ticks = SDL_GetTicks64();
        float frame_time = SDL_GetTicks64() - previous_ticks;
        previous_ticks = new_ticks;

        float total_delta_time  = frame_time / DESIRED_FRAME_TIME;

        fps.begin();

        user_input();

        /*
            Go through each time step and do physics calculations based
            on steps so changes in FPS will not affect collision and other physics.
        */
        int i = 0;
        while(total_delta_time > 0.0f && i++ < MAX_PHYSICS_STEPS) {
            float delta_time = std::min(total_delta_time, MAX_DELTA_TIME);
            _current_screen->update(delta_time);
            total_delta_time -= delta_time;
        }

        _current_screen->draw();

        _fps = fps.end();
    }
}

void MainGame::user_input()
{
    _input_manager.update();
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case(SDL_QUIT):
                _is_running = false;
                break;
            case(SDL_MOUSEMOTION):
                _input_manager.set_mouse_coords(event.motion.x, event.motion.y);
                break;
            case(SDL_KEYDOWN):
                _input_manager.press_key(event.key.keysym.sym);
                break;
            case(SDL_KEYUP):
                _input_manager.release_key(event.key.keysym.sym);
                break;
            case(SDL_MOUSEBUTTONDOWN):
                _input_manager.press_key(event.button.button);
                break;
            case(SDL_MOUSEBUTTONUP):
                _input_manager.release_key(event.button.button);
                break;
        }
    }
}