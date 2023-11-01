#include "movementsystem.h"
#include "../components.h"
#include <glm/glm.hpp>
#include <cmath>

MovementSystem::MovementSystem(ECS *ecs)
    : System(ecs)
{
}

void MovementSystem::run_system(float delta_time)
{
    std::vector<component_id> ids;
    ids.push_back(_ecs->get_component_id<Transform>());
    ids.push_back(_ecs->get_component_id<Velocity>());

    std::vector<component_grouping_t*> groupings;
    if(!_ecs->get_groupings_by_type(ids, &groupings))
        return;

    for(component_grouping_t *grouping : groupings) {
        do_movement(grouping, delta_time);
    }
}

void MovementSystem::do_movement(component_grouping_t *grouping, float delta_time)
{
    unsigned char *transform_buffer = get_transform_buffer(grouping);
    if(!transform_buffer) 
        return;

    component_header_t *transform_header = (component_header_t*)transform_buffer;
    transform_buffer += sizeof(component_header_t);

    unsigned char *velocity_buffer = get_velocity_buffer(grouping);
    if(!velocity_buffer)
        return;
    component_header_t *velocity_header = (component_header_t*)velocity_buffer;
    velocity_buffer += sizeof(component_header_t);

    int bytes_read = 0;
    // Do loop based on end of transform buffer. Either buffer will work
    while(bytes_read < transform_header->bytes_written) {
        Transform *transform = (Transform*)transform_buffer;
        Velocity *velocity = (Velocity*)velocity_buffer;
        glm::vec2 glm_vel(velocity->x, velocity->y);
        glm_vel = glm::normalize(glm_vel);

        transform->x += (glm_vel.x * velocity->speed) * delta_time;
        transform->y += (glm_vel.y * velocity->speed) * delta_time;

        bytes_read += sizeof(Transform);
        transform_buffer += sizeof(Transform);
        velocity_buffer += sizeof(Velocity);
    }
}

unsigned char* MovementSystem::get_transform_buffer(component_grouping_t *grouping)
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

unsigned char* MovementSystem::get_velocity_buffer(component_grouping_t *grouping)
{
    component_id id = _ecs->get_component_id<Velocity>();
    for(unsigned char *buf : grouping->component_data) {
        component_header_t *header = (component_header_t*)buf;
        // Get type
        if(header->type == id)
            return buf;
    }

    return NULL;
}
