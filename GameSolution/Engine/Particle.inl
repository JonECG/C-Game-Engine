void Particle::draw( Core::Graphics& g ) const
{
	if( fade )
	{
		g.SetColor( ColorHelper::quad( color, midColor, RGB(0,0,0), 1 - (life/(float)startLife) ) );
	}
	else
	{
		g.SetColor( ColorHelper::lerp( color, RGB(0,0,0), 1 - (life/(float)startLife) ) );
	}

	for( int i = 0; i < size; i++ )
	{
		g.DrawLine( position.x - i, position.y - i + size, position.x + size - i, position.y - i );
	}
}

void Particle::draw( Core::Graphics& g, Matrix4& globalTransform ) const
{
	if( fade )
	{
		g.SetColor( ColorHelper::quad( color, midColor, RGB(0,0,0), 1 - (life/(float)startLife) ) );
	}
	else
	{
		g.SetColor( ColorHelper::lerp( color, RGB(0,0,0), 1 - (life/(float)startLife) ) );
	}

	for( float i = 0; i < size; i++ )
	{
		Vector2 p1 = globalTransform * ( position + Vector2( -i, -i + size ) );
		Vector2 p2 = globalTransform * ( position + Vector2( size-i, -i ) );
		g.DrawLine( p1.x, p1.y, p2.x, p2.y );
		//g.DrawLine( position.x - i, position.y - i + size, position.x + size - i, position.y - i );
	}
}

void Particle::update( float dt )
{
	velocity += acceleration*dt;
	position += velocity*dt;
	life -= dt;
}

void Particle::setPosition( const Vector2& vec )
{
	position = vec;
}
Vector2 Particle::getPosition() const
{
	return position;
}

void Particle::setVelocity( const Vector2& vec )
{
	velocity = vec;
}
Vector2 Particle::getVelocity() const
{
	return velocity;
}

void Particle::setAcceleration( const Vector2& vec )
{
	acceleration = vec;
}
Vector2 Particle::getAcceleration() const
{
	return acceleration;
}

bool Particle::isAlive() const
{
	return (life > 0);
}

void Particle::kill()
{
	life = 0;
}

void Particle::setLife( float life )
{
	this->life = life;
	this->startLife = life;
}

void Particle::setSize( int size )
{
	this->size = size;
}

void Particle::setColor( Core::RGB color )
{
	this->color = color;
	fade = false;
}

void Particle::setColor( Core::RGB color, Core::RGB midColor )
{
	this->color = color;
	this->midColor = midColor;
	fade = true;
}