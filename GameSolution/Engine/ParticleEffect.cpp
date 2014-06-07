#include "ParticleEffect.h"
#include "MathHelper.h"

ParticleEffect::ParticleEffect()
{
	head = 0;
	this->size = PARTICLE_LIMIT;
	current = new Particle*[size];
	for( int i = 0; i < size; i++ )
	{
		current[i] = new Particle();
	}
}

Particle * ParticleEffect::createParticle( float life )
{
	Particle * renew = static_cast<Particle*>(current[(head)%PARTICLE_LIMIT]);

	head = (head + 1)%PARTICLE_LIMIT;

	renew->setLife( life );
	renew->setSize( 1 );

	return renew;
}