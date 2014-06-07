#include "Particle.h"

Particle::Particle()
{
	position = Vector2();
	velocity = Vector2();
	acceleration = Vector2();
	fade = false;
	life = 0;
	size = 1;
	color = RGB( 255, 255, 255 );
}
