#include "Starfield.h"
#include "MathHelper.h"
#include "ColorHelper.h"

Shape* Starfield::star = nullptr;

Starfield::Starfield(void)
{
	for( int i = 0; i < STARAMOUNT; i++ )
	{
		starPositions[i] = Vector2( float(MathHelper::randomInt(1280)), float(MathHelper::randomInt(960)) );
		starSizes[i] = MathHelper::randomFloat() * 4 + 2;
		starTimes[i] = MathHelper::randomFloat() * 4;
		starColors[i] = ColorHelper::quad( RGB( 255,130,130 ), RGB( 255, 200, 20 ), RGB( 100, 200, 255 ), MathHelper::randomFloat() );
		starDepth[i] = MathHelper::randomFloat()/20;
	}
}

void Starfield::update( float dt )
{
	for( int i = 0; i < STARAMOUNT; i++ )
	{
		starTimes[i] += dt*2;
	}
}

void Starfield::draw( Core::Graphics& g, Matrix4 globalTransform, Ship& ship )
{
	for( int i = 0; i < STARAMOUNT; i++ )
	{
		float ratio = std::sin( starTimes[i] )/3+2.0f/3;
		star->draw( g, globalTransform * Matrix4::translation(starPositions[i] + starDepth[i]*(Vector2(640,480)-ship.getPosition()) )*Matrix2::scale(starSizes[i]*ratio), ColorHelper::lerp(starColors[i],RGB(0,0,0),1-ratio ));
	}
	ship;
}