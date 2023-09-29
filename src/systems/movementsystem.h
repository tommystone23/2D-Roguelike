#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H
#include "../ecs/ecs.h"
#include "../ecs/system.h"

class MovementSystem : public System
{
public:
    MovementSystem(ECS *ecs);
    ~MovementSystem(){}

    virtual void run_system(float delta_time) override;

private:
    void do_movement(component_grouping_t *grouping, float delta_time);
    unsigned char* get_transform_buffer(component_grouping_t *grouping);
    unsigned char* get_velocity_buffer(component_grouping_t *grouping);
};

#endif // MOVEMENTSYSTEM_H