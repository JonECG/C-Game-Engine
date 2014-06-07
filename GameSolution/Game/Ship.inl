bool Ship::canShoot()
{
	return (reload >= FIRE_RATE());
}

Bullet Ship::shoot()
{
	reload = 0;
	Vector2 direction = Vector2( turretMatrix.getCell( 0, 0 ), turretMatrix.getCell( 0, 1 ) );
	return Bullet( position + direction * TURRET_LENGTH, direction * BULLET_SPEED + velocity/2, true );
}

void Ship::update( float dt )
{
	prevPosition = position;
	reload += dt;
	if ( reload > FIRE_RATE() )
		reload = FIRE_RATE();
	velocity += acceleration * dt;
	velocity *= 1 - (2.0f*dt);
	position += velocity * dt;
}

void Ship::drawTurret( Core::Graphics& g, Matrix4& globalTransform )
{
	Matrix4 mat = globalTransform * Matrix4::translation( position ) * turretMatrix;
	turretShape->drawGlow( g, mat, RGB( 40, 130, 40 ) );
}

void Ship::draw( Core::Graphics& g, Matrix4& globalTransform )
{
	if ( velocity.lengthSquared() != 0 )
	{
		rotation = velocity.getAngle();
	}
	Matrix4 mat = globalTransform * Matrix4::translation( position ) * Matrix3::rotation( rotation );
	shipShape->drawGlow(g, mat, RGB(0,255,0));
	drawTurret( g, globalTransform );
}

void Ship::setVelocity( const Vector2& vec )
{
	velocity.x = vec.x;
	velocity.y = vec.y;
}

void Ship::setAcceleration( const Vector2& vec )
{
	acceleration.x = vec.x;
	acceleration.y = vec.y;
}

void Ship::setPosition( const Vector2& vec )
{
	position.x = vec.x;
	position.y = vec.y;
}

Vector2 Ship::getTurret()
{
	return turret;
}

void Ship::setTurret( const Vector2& vec )
{
	Vector2 n = vec.normalized();
	Vector2 p = n.perpCCW();
	turretMatrix = Matrix2( n, p );
}

Vector2 Ship::getVelocity()
{
	return velocity;
}

Vector2 Ship::getAcceleration()
{
	return acceleration;
}

Vector2 Ship::getPosition()
{
	return position;
}

float Ship::getRotation()
{
	return rotation;
}

void Ship::setRotation( float rot )
{
	rotation = rot;
}
	
Shape* Ship::getShape()
{
	return shipShape;
}

float Ship::getX()
{
	return position.x;
}

float Ship::getY()
{
	return position.y;
}

void Ship::goToPrevious()
{
	position = prevPosition;
}