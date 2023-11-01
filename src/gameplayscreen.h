#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H
#include "utils/IGameScreen.h"
#include "utils/GLTexture.h"
#include "ecs/ecs.h"

class GameplayScreen : public IGameScreen
{
public:
    GameplayScreen(MainGame *game);
    ~GameplayScreen() {}

    virtual int get_next_screen_index() const override;
    virtual int get_previous_screen_index() const override;

    // Called at beginning and end of application
    virtual void build() override;
    virtual void destroy() override;

    // Called when screen enters or exits focus
    virtual void on_entry() override;
    virtual void on_exit() override;

    // Called in main game loop
    virtual void update(float delta_time) override;
    virtual void draw() override;

private:
    ECS *_ecs;
    MainGame *_game;
    System *_render_system;

    GLTexture _texture;
};

#endif // GAMEPLAYSCREEN_H