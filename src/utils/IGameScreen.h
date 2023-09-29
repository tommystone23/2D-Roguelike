#ifndef IGAME_SCREEN_H
#define IGAME_SCREEN_H

enum class ScreenState
{
    NONE,
    RUNNING, 
    EXIT_APPLICATION,
    CHANGE_NEXT,
    CHANGE_PREVIOUS
};

class MainGame;

class IGameScreen
{
public:
    IGameScreen() {}
    virtual ~IGameScreen() {}

    virtual int get_next_screen_index() const = 0;
    virtual int get_previous_screen_index() const = 0;

    // Called at beginning and end of application
    virtual void build() = 0;
    virtual void destroy() = 0;

    // Called when screen enters or exits focus
    virtual void on_entry() = 0;
    virtual void on_exit() = 0;

    // Called in main game loop
    virtual void update(float delta_time) = 0;
    virtual void draw() = 0;

    void set_parent_game(MainGame *game) { _game = game; }
    void set_running() { _state = ScreenState::RUNNING; }
    void set_screen_index(int screen_index) { _screen_index = screen_index; }

    int get_screen_index() const { return _screen_index; }
    ScreenState get_screen_state() const { return _state; }

protected:
    MainGame *_game;
    ScreenState _state = ScreenState::NONE;
    int _screen_index;
};

#endif // IGAME_SCREEN_H