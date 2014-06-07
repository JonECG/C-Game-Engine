/*
class Matrix2
*/

Matrix2::Matrix2()
{
	setCell( 0, 0, 1 );
	setCell( 1, 0, 0 );
	setCell( 0, 1, 0 );
	setCell( 1, 1, 1 );
}
Matrix2::Matrix2( const Vector2& column1, const Vector2& column2 )
{
	setCell( 0, 0, column1.x );
	setCell( 1, 0, column2.x );
	setCell( 0, 1, column1.y );
	setCell( 1, 1, column2.y );
}

Matrix2 Matrix2::rotation( float angle )
{
	Matrix2 result;
	result.setCell( 0, 0, std::cos(angle));
	result.setCell( 1, 0, -std::sin(angle));
	result.setCell( 0, 1, std::sin(angle));
	result.setCell( 1, 1, std::cos(angle));
	return result;
}
Matrix2 Matrix2::scale( float scale )
{
	Matrix2 result;
	result.setCell( 0, 0, scale );
	//result.setCell( 1, 0, 0 );
	//result.setCell( 0, 1, 0 );
	result.setCell( 1, 1, scale );
	return result;
}
Matrix2 Matrix2::scaleX( float scale )
{
	Matrix2 result;
	result.setCell( 0, 0, scale );
	//result.setCell( 1, 0, 0 );
	//result.setCell( 0, 1, 0 );
	result.setCell( 1, 1, 1 );
	return result;
}
Matrix2 Matrix2::scaleY( float scale )
{
	Matrix2 result;
	result.setCell( 0, 0, 1 );
	//result.setCell( 1, 0, 0 );
	//result.setCell( 0, 1, 0 );
	result.setCell( 1, 1, scale );
	return result;
}

float Matrix2::getCell( int column, int row ) const
{
	return values[row*2+column];
}
void Matrix2::setCell( int column, int row, float value )
{
	values[row*2+column] = value;
}

Matrix2 Matrix2::transpose() const
{
	Matrix2 result;
	for( int i = 0; i < 2; i++ )
	{
		for( int j = 0; j < 2; j++ )
		{
			result.setCell( i, j, getCell( j, i ) );
		}
	}
	return result;
}

Matrix2::operator float*()
{
	return &values[0];
}

void Matrix2::operator+=( const Matrix2& other )
{
	setCell( 0, 0, getCell( 0, 0 ) + other.getCell( 0, 0 ) );
	setCell( 1, 0, getCell( 1, 0 ) + other.getCell( 1, 0 ) );
	setCell( 0, 1, getCell( 0, 1 ) + other.getCell( 0, 1 ) );
	setCell( 1, 1, getCell( 1, 1 ) + other.getCell( 1, 1 ) );
}
void Matrix2::operator-=( const Matrix2& other )
{
	setCell( 0, 0, getCell( 0, 0 ) - other.getCell( 0, 0 ) );
	setCell( 1, 0, getCell( 1, 0 ) - other.getCell( 1, 0 ) );
	setCell( 0, 1, getCell( 0, 1 ) - other.getCell( 0, 1 ) );
	setCell( 1, 1, getCell( 1, 1 ) - other.getCell( 1, 1 ) );
}
void Matrix2::operator*=( const Matrix2& other )
{
	float temp00 = getCell( 0, 0 ), temp01 = getCell( 0, 1 );
	setCell( 0, 0, temp00*other.getCell( 0, 0 ) + getCell( 1, 0 ) * other.getCell( 0, 1 ) );
	setCell( 1, 0, temp00*other.getCell( 1, 0 ) + getCell( 1, 0 ) * other.getCell( 1, 1 ) );
	setCell( 0, 1, temp01*other.getCell( 0, 0 ) + getCell( 1, 1 ) * other.getCell( 0, 1 ) );
	setCell( 1, 1, temp01*other.getCell( 1, 0 ) + getCell( 1, 1 ) * other.getCell( 1, 1 ) );
}


Matrix2 operator+( const Matrix2& left, const Matrix2& right )
{
	Matrix2 result;
	result.setCell( 0, 0, left.getCell( 0, 0 ) + right.getCell( 0, 0 ) );
	result.setCell( 1, 0, left.getCell( 1, 0 ) + right.getCell( 1, 0 ) );
	result.setCell( 0, 1, left.getCell( 0, 1 ) + right.getCell( 0, 1 ) );
	result.setCell( 1, 1, left.getCell( 1, 1 ) + right.getCell( 1, 1 ) );
	return result;
}
Matrix2 operator-( const Matrix2& left, const Matrix2& right )
{
	Matrix2 result;
	result.setCell( 0, 0, left.getCell( 0, 0 ) - right.getCell( 0, 0 ) );
	result.setCell( 1, 0, left.getCell( 1, 0 ) - right.getCell( 1, 0 ) );
	result.setCell( 0, 1, left.getCell( 0, 1 ) - right.getCell( 0, 1 ) );
	result.setCell( 1, 1, left.getCell( 1, 1 ) - right.getCell( 1, 1 ) );
	return result;
}
Matrix2 operator*( const Matrix2& left, const Matrix2& right )
{
	Matrix2 result;
	result.setCell( 0, 0, left.getCell( 0, 0 )*right.getCell( 0, 0 ) + left.getCell( 1, 0 ) * right.getCell( 0, 1 ) );
	result.setCell( 1, 0, left.getCell( 0, 0 )*right.getCell( 1, 0 ) + left.getCell( 1, 0 ) * right.getCell( 1, 1 ) );
	result.setCell( 0, 1, left.getCell( 0, 1 )*right.getCell( 0, 0 ) + left.getCell( 1, 1 ) * right.getCell( 0, 1 ) );
	result.setCell( 1, 1, left.getCell( 0, 1 )*right.getCell( 1, 0 ) + left.getCell( 1, 1 ) * right.getCell( 1, 1 ) );
	return result;
}
Vector2 operator*( const Matrix2& left, const Vector2& right )
{
	Vector2 result;
	result.x = left.getCell( 0, 0 )*right.x + left.getCell( 1, 0)*right.y;
	result.y = left.getCell( 0, 1 )*right.x + left.getCell( 1, 1)*right.y;
	return result;
}