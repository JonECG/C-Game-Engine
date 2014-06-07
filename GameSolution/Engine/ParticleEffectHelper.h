#ifndef PARTICLEEFFECTHELPER_H
#define PARTICLEEFFECTHELPER_H

#include "ParticleEffect.h"
#include "Vector2.h"
#include "ExportHeader.h"

struct ENGINE_SHARED ParticleEffectHelper
{
	static void createExplosion( ParticleEffect* effect, const Vector2& vec, int maxVelocity, int count );
	static void emitStream( ParticleEffect* effect, Core::RGB color, const Vector2& pos, const Vector2& ref, int maxVelocity, float chance );
};

#endif