void Enemy::update( float dt )
{
	velocity += acceleration * dt;
	velocity *= .9f;
	position += velocity * dt;
}

void Enemy::draw( Core::Graphics& g ) const
{
	Matrix4 mat = Matrix4::translation( position ) * Matrix2::rotation( velocity.getAngle() );
	getShape()->drawGlow( g, mat, RGB( 210,0,0 ) );
}

void Enemy::draw( Core::Graphics& g, Matrix4& globalTransform ) const
{
	Matrix4 mat = globalTransform * Matrix4::translation( position ) * Matrix2::rotation( velocity.getAngle() );
	getShape()->draw( g, mat, RGB( 210,0,0 ) );
}

void Enemy::setVelocity( const Vector2& vec )
{
	velocity.x = vec.x;
	velocity.y = vec.y;
}

void Enemy::setAcceleration( const Vector2& vec )
{
	acceleration.x = vec.x;
	acceleration.y = vec.y;
}

void Enemy::setPosition( const Vector2& vec )
{
	position.x = vec.x;
	position.y = vec.y;
}


Vector2 Enemy::getVelocity()
{
	return velocity;
}

Vector2 Enemy::getAcceleration()
{
	return acceleration;
}

Vector2 Enemy::getPosition()
{
	return position;
}


float Enemy::getX()
{
	return position.x;
}

float Enemy::getY()
{
	return position.y;
}

bool Enemy::isAlive() const
{
	return alive;
}
void Enemy::kill()
{
	alive = false;
}