#include "ParticleBatch2D.h"

Particle::Particle()
    : life_time(0.0f)
{

}

Particle::~Particle()
{
}

ParticleBatch2D::ParticleBatch2D(int max_particles, float decay_rate, 
                                GLTexture texture, std::function<void(Particle&, float, float)> update_func /*= default_particle_update*/)
    : _update_func(update_func),
    _decay_rate(decay_rate),
    _particles(NULL),
    _max_particles(max_particles),
    _last_free_particle(0),
    _texture(texture)
{
    _particles = new Particle[_max_particles];
}

ParticleBatch2D::~ParticleBatch2D()
{
    delete[] _particles;
}

void ParticleBatch2D::update(float delta_time)
{
    for(int i = 0; i < _max_particles; i++) {
        if(_particles[i].life_time > 0.0f)
            _update_func(_particles[i], delta_time, _decay_rate);
    }
}

void ParticleBatch2D::draw(SpriteBatch &sprite_batch)
{
    static glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
    for(int i = 0; i < _max_particles; i++) {
        Particle &particle = _particles[i];
        if(particle.life_time > 0.0f) {
            glm::vec4 dest_rect(particle.position.x, particle.position.y, particle.width, particle.width);

            sprite_batch.draw(dest_rect, uv_rect, _texture.id, 0.0f, particle.color);
        }
    }
}

void ParticleBatch2D::add_particle(const glm::vec2 &position, const glm::vec2 &velocity, 
                                    const Color &color, float width)
{
    int particle_index = find_free_particle();

    Particle &particle = _particles[particle_index];
    particle.life_time = 1.0f;
    particle.position = position;
    particle.velocity = velocity;
    particle.color = color;
    particle.width = width;
}

int ParticleBatch2D::find_free_particle()
{
    for(int i = _last_free_particle; i < _max_particles; i++) {
        if(_particles[i].life_time <= 0.0f) {
            _last_free_particle = i;
            return i;
        }
    }
    for(int i = 0; i < _last_free_particle; i++) {
        if(_particles[i].life_time <= 0) {
            _last_free_particle = i;
            return i;
        }
    }
    // No particles are free, overwrite first particle
    return 0;
}
