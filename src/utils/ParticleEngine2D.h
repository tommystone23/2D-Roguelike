#ifndef PARTICLE_ENGINE2D_H
#define PARTICLE_ENGINE2D_H
#include "ParticleBatch2D.h"

class ParticleEngine2D
{
public:
    ParticleEngine2D();
    ~ParticleEngine2D();

    void add_particle_batch(ParticleBatch2D *particle_batch);

    void update(float delta_time);
    void draw(SpriteBatch &sprite_batch);

private:
    std::vector<ParticleBatch2D*> _batches;
};


#endif // PARTICLE_ENGINE2D_H