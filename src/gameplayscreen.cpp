#include "gameplayscreen.h"
#include "utils/Window.h"
#include "utils/Camera2D.h"
#include "utils/InputManager.h"
#include "components.h"
#include "utils/ImageLoader.h"
#include "systems/movementsystem.h"
#include "systems/rendersystem.h"
#include <ctime>
#include <random>
#include "maingame.h"

const static float ENTITY_WIDTH = 16.0f;
const static float ENTITY_HEIGHT = 28.0f;

GameplayScreen::GameplayScreen(MainGame *game)
    : _game(game)
{
}

int GameplayScreen::get_next_screen_index() const
{
    return 0;
}

int GameplayScreen::get_previous_screen_index() const
{
    return 0;
}

void GameplayScreen::build()
{
    _ecs = new ECS();
    _ecs->init();
}

void GameplayScreen::destroy()
{
    delete _ecs;
}

void GameplayScreen::on_entry()
{
    _texture = ImageLoader::load_image("textures/0x72_DungeonTilesetII_v1.6/0x72_DungeonTilesetII_v1.6.png");
    glm::vec4 uv = ImageLoader::texture_coords_to_uv(glm::vec4(128.0f, 292.0f, 16.0f, 28.0f), 
                                                            _texture.width, 
                                                            _texture.height,
                                                            true);                                                   

    std::mt19937 random_engine(time(nullptr));
    for(int i = 0; i < 10000; i++) {
        std::uniform_real_distribution<float> rand_angle(-1.0f, 1.0f);
        std::uniform_real_distribution<float> rand_speed(0.0f, 1.0f);
        float velocity_x = rand_angle(random_engine);
        float velocity_y = rand_angle(random_engine);
        float speed = rand_speed(random_engine);
        entity_id id = _ecs->create_entity();
        _ecs->add_component<Transform>(id, { 0.0f, 0.0f, ENTITY_WIDTH, ENTITY_HEIGHT });
        _ecs->add_component<Velocity>(id, { velocity_x, velocity_y, speed });
        _ecs->add_component<Render>(id, { _texture.id, uv.x, uv.y, uv.z, uv.w });
        _ecs->commit_components(id);
    }

    System *movement_system = new MovementSystem(_ecs);
    _ecs->register_system(movement_system, 0);
    _render_system = new RenderSystem(_game, _ecs);
    //_ecs->register_system(render_system, 1);
}

void GameplayScreen::on_exit()
{
}

void GameplayScreen::update(float delta_time)
{
    _ecs->run_systems(delta_time);
}

void GameplayScreen::draw()
{
    _render_system->run_system(1.0f);
}
