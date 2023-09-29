#ifndef PARTICLE_BATCH2D_H
#define PARTICLE_BATCH2D_H
#include <glm/glm.hpp>
#include <functional>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

class Particle
{
public:
    Particle();
    ~Particle();

    glm::vec2 position;
    glm::vec2 velocity;
    Color color;
    float life_time;
    float width;
};

inline void default_particle_update(Particle &particle, float delta_time, float decay_rate)
{
    particle.position += particle.velocity * delta_time;
    particle.life_time -= decay_rate * delta_time;
}

class ParticleBatch2D
{
public:
    ParticleBatch2D(int max_particles, float decay_rate, 
                    GLTexture texture, 
                    std::function<void(Particle&, float, float)> update_func = default_particle_update);
    ~ParticleBatch2D();

    void update(float delta_time);
    void draw(SpriteBatch &sprite_batch);

    void add_particle(const glm::vec2 &position, const glm::vec2 &velocity, 
                        const Color &color, float width);

private:
    int find_free_particle();

    std::function<void(Particle&, float, float)> _update_func;

    float _decay_rate;

    Particle *_particles;

    int _max_particles;
    int _last_free_particle;

    GLTexture _texture;
};

#endif // PARTICLE_BATCH2D_H