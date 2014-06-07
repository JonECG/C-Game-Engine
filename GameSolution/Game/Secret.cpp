#include "Secret.h"


Shape* Secret::jamie = nullptr;

Secret::Secret()
{
	state = -1;
}

void Secret::init()
{
	position.x = 640;
	position.y = -320;
	state = CENTER;
	time = 0;
	spin = 0;
	reload = 0;
	life = 75;
}

void Secret::wait()
{
	if (state != CENTER && life > 0)
	{
		state = POLITELYWAITING;
		time = 0;
	}
}

void Secret::update( float dt, Ship& ship, std::vector<Bullet>& bul )
{
	switch( state )
	{
	case CENTER:
		{
		Vector2 difference = (Vector2( 640, 480 ) - position);
		Vector2 spDiff = difference.normalized()*120*dt;
		position += spDiff;
		if( (Vector2( 640, 480 ) - position).lengthSquared() < 20 )
			state = LASER;
		}
		break;
	case LASER:
		spin += dt/2;
		time+=dt;
		if (time > 10)
		{
			state = SHOOT;
			time = 0;
			reload = 0;
		}
		break;
	case SHOOT:
		time+=dt;
		reload+=dt;
		if (reload > 1.5f)
		{
			reload = 0;
			//Shooting at
			Vector2 direction = ((ship.getPosition()+ship.getVelocity()/4)-(Vector2(-83,60) + position)).normalized();
			bul.push_back( Bullet( position+Vector2(-83,60), direction * 800, false ) );
			//Shooting while leading
			direction = (ship.getPosition()-(Vector2(-47,55) + position)).normalized();
			bul.push_back( Bullet( position+Vector2(-47,55), direction * 800, false ) );
		}
		if (time > 10)
		{
			state = LASER;
			time = 0;
		}
		break;
	case POLITELYWAITING:
		time+=dt;
		if (time > 3)
		{
			state = LASER;
			time = 0;
			reload = 0;
		}
		break;
	}
}

bool Secret::collide( Ship& ship )
{
	bool result = false;

	if( state == LASER )
	{
		if( time > 2 )
		{
			result = collideWithEye( position+Vector2(-105,-15), ship ) || collideWithEye( position+Vector2(3,-20), ship );
		}
	}

	return result;
}

bool Secret::collideWithEye( Vector2 pos, Ship& ship )
{
	bool result = false;
	for( int i = 0; i < 4; i++ )
	{
		Vector2 a = (ship.getPosition() - pos);
		Vector2 b = (Vector2::fromAngle( spin+i*3.1415f/2, 1000 ));
		if ( (a-(a.project(b)*b.normalized())).lengthSquared() < 800 )
			 result = true;
	}
	return result;
	
}

void Secret::draw( Core::Graphics& g, Matrix4 globalTransform )
{
	if ( life > 0 )
	jamie->draw( g, globalTransform * Matrix4::translation( position ), RGB( 255, 0, 0 ) );

	if( state == LASER )
	{
		g.SetColor( RGB( 255, 100, 100 ) );
		int size = ( time < 2 ) ? 1 : 10;
		drawEyeLine( position+Vector2(-105,-15), g, globalTransform, size );
		drawEyeLine( position+Vector2(3,-20), g, globalTransform, size );
	}
}

void Secret::disable()
{
	state = -1;
	life = 0;
}

void Secret::drawEyeLine( Vector2 pos, Core::Graphics& g, Matrix4 globalTransform, int thickness )
{
	for( int i = 0; i < 4; i++ )
	{
		Vector2 before = globalTransform*pos;
		Vector2 after = globalTransform*(pos+Vector2::fromAngle( spin+i*3.1415f/2, 1000 ));
		for( int i = 0; i < thickness; i++ )
		{
			g.DrawLine( before.x+i, before.y, after.x+i, after.y );
			g.DrawLine( before.x, before.y-i, after.x, after.y-i );
			g.DrawLine( before.x-i, before.y, after.x-i, after.y );
			g.DrawLine( before.x, before.y+i, after.x, after.y+i );
		}
	}
}

void Secret::hit()
{
	life--;
	if ( life <= 0 )
		state = -1;
}