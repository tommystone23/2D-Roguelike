#include "rendersystem.h"
#include "../components.h"

RenderSystem::RenderSystem(MainGame *game, ECS *ecs)
    : System(ecs),
    _batch(NULL),
    _program(NULL),
    _sprite_font(NULL),
    _game(game)
{
}

void RenderSystem::run_system(float delta_time)
{
    _game->get_camera()->update();
    std::vector<component_id> ids;
    ids.push_back(_ecs->get_component_id<Transform>());
    ids.push_back(_ecs->get_component_id<Render>());

    std::vector<component_grouping_t*> groupings;
    if(!_ecs->get_groupings_by_type(ids, &groupings))
        return;

    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    if(!_batch) {
        _batch = new SpriteBatch();
        _batch->init();
    }
    if(!_program) {
        _program = new GLSLProgram();
        _program->compile_shaders("shaders/texture_shader.vert", 
                                    "shaders/texture_shader.frag");
        _program->link_shaders();
    }
    if(!_sprite_font) {
        _sprite_font = new SpriteFont();
        _sprite_font->init("fonts/OpenSans-Regular.ttf", 32);
    }

    glClearDepth(1.0);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _program->use();

    glActiveTexture(GL_TEXTURE0);
    GLint texture_uniform = _program->get_uniform_location("texture0");
    // Pass uniform 0 for texture 0
    glUniform1i(texture_uniform, 0);

    glm::mat4 projection = _game->get_camera()->get_camera_matrix();
    GLint projection_location = _program->get_uniform_location("projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);

    _batch->begin();
    render_hud(_batch);
    for(component_grouping_t *grouping : groupings) {
        batch_data(_batch, grouping);
    }
    _batch->end();
    _batch->render_batch();

    _program->unuse();
    _game->get_window()->swap_buffers();
}

void RenderSystem::batch_data(SpriteBatch *batch, component_grouping_t *grouping)
{
    unsigned char *render_buffer = get_render_buffer(grouping);
    if(!render_buffer)
        return;
    
    component_header_t *render_header = (component_header_t*)render_buffer;
    render_buffer += sizeof(component_header_t);

    unsigned char *transform_buffer = get_transform_buffer(grouping);
    if(!transform_buffer) 
        return;

    component_header_t *transform_header = (component_header_t*)transform_buffer;
    transform_buffer += sizeof(component_header_t);

    int bytes_read = 0;
    while(bytes_read < render_header->bytes_written) {
        Render *render = (Render*)render_buffer;
        Transform *transform = (Transform*)transform_buffer;

        glm::vec4 dest_rect(transform->x, transform->y,
                            transform->w, transform->h);
        glm::vec4 uv_rect(render->x, render->y,
                            render->w, render->h);

        Color color = { 255, 255, 255, 255 };

        batch->draw(dest_rect, uv_rect, render->id, 0.0f, color);

        bytes_read += sizeof(Render);
        render_buffer += sizeof(Render);
        transform_buffer += sizeof(Transform);
    }
}

unsigned char* RenderSystem::get_render_buffer(component_grouping_t *grouping)
{
    component_id id = _ecs->get_component_id<Render>();
    for(unsigned char *buf : grouping->component_data) {
        component_header_t *header = (component_header_t*)buf;
        // Get type
        if(header->type == id)
            return buf;
    }

    return NULL;
}

unsigned char* RenderSystem::get_transform_buffer(component_grouping_t *grouping)
{
    component_id id = _ecs->get_component_id<Transform>();
    for(unsigned char *buf : grouping->component_data) {
        component_header_t *header = (component_header_t*)buf;
        // Get type
        if(header->type == id)
            return buf;
    }

    return NULL;
}

void RenderSystem::render_hud(SpriteBatch *batch)
{
    static int BUFFER_SIZE = 100;
    static float fps = 0;
    static int count = 0;
    count++;
    if(count >= 60) { // Update every 60 frames
        fps = _game->get_fps();
        count = 0;
    }
    char fps_string[BUFFER_SIZE];
    snprintf(fps_string, sizeof(fps_string), "FPS: %.2f", fps);

    // Draw relative to camera
    glm::vec2 camera_pos = _game->get_camera()->get_position();
    float camera_scale = _game->get_camera()->get_scale();
    
    // need to shift camera over half of screen width and height and account for scale
    float offset_height = _game->get_window()->get_screen_height() / 2;
    float offset_width = _game->get_window()->get_screen_width() / 2;

    glm::vec2 fps_text_pos(camera_pos.x - (offset_width / camera_scale), 
                    camera_pos.y + (offset_height / camera_scale) - (32 * 1.0f));

    Color color = { 0, 0, 0, 255 };
    _sprite_font->draw(batch, fps_string, fps_text_pos, glm::vec2(1.0f), 0, color);
}
