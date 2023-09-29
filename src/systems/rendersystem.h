#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "../ecs/system.h"
#include "../ecs/ecs.h"
#include "../utils/SpriteBatch.h"
#include "../utils/GLSLProgram.h"
#include "../utils/Camera2D.h"
#include "../utils/Window.h"

class RenderSystem : public System
{
public:
    RenderSystem(Camera2D *camera, Window *window, ECS *ecs);
    virtual ~RenderSystem() { 
            delete _batch;
            delete _program; 
        }

    virtual void run_system(float delta_time) override;

private:
    SpriteBatch *_batch;
    GLSLProgram *_program;
    Camera2D *_camera;
    Window *_window;

    void batch_data(SpriteBatch *batch, component_grouping_t *grouping);

    unsigned char* get_render_buffer(component_grouping_t *grouping);
    unsigned char* get_transform_buffer(component_grouping_t *grouping);
};

#endif // RENDERSYSTEM_H