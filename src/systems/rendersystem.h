#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "../ecs/system.h"
#include "../ecs/ecs.h"
#include "../utils/SpriteBatch.h"
#include "../utils/GLSLProgram.h"
#include "../utils/Camera2D.h"
#include "../utils/Window.h"
#include "../utils/SpriteFont.h"
#include "../maingame.h"

class RenderSystem : public System
{
public:
    RenderSystem(MainGame *game, ECS *ecs);
    virtual ~RenderSystem() { 
            delete _batch;
            delete _program; 
        }

    virtual void run_system(float delta_time) override;

private:
    SpriteBatch *_batch;
    GLSLProgram *_program;
    SpriteFont *_sprite_font;
    MainGame *_game;

    void batch_data(SpriteBatch *batch, component_grouping_t *grouping);
    void render_hud(SpriteBatch *batch);

    unsigned char* get_render_buffer(component_grouping_t *grouping);
    unsigned char* get_transform_buffer(component_grouping_t *grouping);
};

#endif // RENDERSYSTEM_H