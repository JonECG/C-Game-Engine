void Shape::setPoint( int index, Vector2 point )
{
	//ASSERT( index < count && index >= 0, "Shape point range check" );
	points[index] = point;
}

Vector2 Shape::getPoint( int index ) const
{
	//ASSERT( index < count && index >= 0, "Shape point range check" );
	return points[index];
}

int Shape::getCount() const
{
	return count;
}

void Shape::draw( Core::Graphics& g, const Vector2& position, const Core::RGB& color ) const
{
	g.SetColor( color );
	draw( g, position );
}

void Shape::draw( Core::Graphics& g, const Matrix3& matrix, const Core::RGB& color ) const
{
	g.SetColor( color );
	draw( g, matrix );
}

void Shape::draw( Core::Graphics& g, const Matrix4& matrix, const Core::RGB& color ) const
{
	g.SetColor( color );
	draw( g, matrix );
}

void Shape::scale( float scale )
{
	for( int i = 0; i < count; i++ )
	{
		points[i]*=scale;
	}
}

void Shape::flipX()
{
	for( int i = 0; i < count; i++ )
	{
		points[i].x*=-1;
	}
}

void Shape::flipY()
{
	for( int i = 0; i < count; i++ )
	{
		points[i].y*=-1;
	}
}