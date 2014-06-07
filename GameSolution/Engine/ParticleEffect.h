#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include "ExportHeader.h"
#include "Particle.h"
#include "DebugMemory.h"
#include "OrganicContainer.h"


class ENGINE_SHARED ParticleEffect : public OrganicContainer<Particle>
{
	static const int PARTICLE_LIMIT = 2000;
public:
	ParticleEffect();
	Particle * createParticle( float life );
};

#endif