#include "ParticleEngine2D.h"

ParticleEngine2D::ParticleEngine2D()
{
}

ParticleEngine2D::~ParticleEngine2D()
{
    for(ParticleBatch2D *batch : _batches) {
        delete batch;
    }
}

void ParticleEngine2D::add_particle_batch(ParticleBatch2D *particle_batch)
{
    _batches.push_back(particle_batch);
}

void ParticleEngine2D::update(float delta_time)
{
    for(ParticleBatch2D *batch : _batches) {
        batch->update(delta_time);
    }
}

void ParticleEngine2D::draw(SpriteBatch &sprite_batch)
{
    for(ParticleBatch2D *batch : _batches) {
        sprite_batch.begin();

        batch->draw(sprite_batch);

        sprite_batch.end();
        sprite_batch.render_batch();
    }
}
