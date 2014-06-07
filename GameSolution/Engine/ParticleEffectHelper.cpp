#include "ParticleEffectHelper.h"
#include "MathHelper.h"
#include "Log.h"

void ParticleEffectHelper::createExplosion( ParticleEffect* effect, const Vector2& position, int velocityMax, int count )
{
	if ( count > effect->getSize() )
	{
		LOG( Severity::WARNING, "Creating an explosion with more particles than max allowed" );
	}

	int fragments = count;
	for( int i = 0; i < fragments; i++ )
	{
		Particle * part = effect->createParticle( 1 + MathHelper::randomFloat() );
		part->setColor( RGB(255,255,0), RGB(255,0,0) );
		//part->setColor( std::rand()*1000 );
		part->setPosition( position );
		part->setVelocity( MathHelper::randomUnitVector() * MathHelper::randomInt( velocityMax/5, velocityMax ) );
		part->setAcceleration( -Vector2(part->getVelocity().x/(2 + MathHelper::randomSignedFloat()/4), part->getVelocity().y/(2 + MathHelper::randomSignedFloat()/4) ) );
		part->setSize( MathHelper::randomInt(13)+2 );
		//part->setVelocity( Vector2::fromAngle((PI*2*i)/fragments, 400));
		//part->setAcceleration( Vector2::fromAngle((PI*2*i)/fragments, -400));
	}
}

void ParticleEffectHelper::emitStream( ParticleEffect* effect, Core::RGB color, const Vector2& position, const Vector2& referenceVector, int velocityMax, float chance )
{
	float adjustedChance = chance;
	int parts = 0;

	while( adjustedChance >= 1 )
	{
		parts++;
		adjustedChance--;
	}
	if ( MathHelper::randomFloat() <= adjustedChance )
	{
		parts++;
	}

	for( int i = 0; i < parts; i++ )
	{
		Particle * part = effect->createParticle( 1 + MathHelper::randomFloat() );
		part->setColor( color );

		part->setPosition( position );
		part->setVelocity( referenceVector.normalized() * velocityMax * 3 / 4 + MathHelper::randomUnitVector() * MathHelper::randomFloat() * velocityMax/4 );
		part->setAcceleration( -Vector2(part->getVelocity().x/(2 + MathHelper::randomSignedFloat()/4), part->getVelocity().y/(2 + MathHelper::randomSignedFloat()/4) ) );
		part->setSize( MathHelper::randomInt(8)+1 );
		//part->setVelocity( Vector2::fromAngle((PI*2*i)/fragments, 400));
		//part->setAcceleration( Vector2::fromAngle((PI*2*i)/fragments, -400));
	}
}