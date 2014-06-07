/*
class Matrix3
*/

Matrix3::Matrix3()
{
	setCell( 0, 0, 1 );
	setCell( 1, 0, 0 );
	setCell( 2, 0, 0 );

	setCell( 0, 1, 0 );
	setCell( 1, 1, 1 );
	setCell( 2, 1, 0 );

	setCell( 0, 2, 0 );
	setCell( 1, 2, 0 );
	setCell( 2, 2, 1 );
}
Matrix3::Matrix3( const Vector2& column1, const Vector2& column2 )
{
	setCell( 0, 0, column1.x );
	setCell( 1, 0, column2.x );
	setCell( 2, 0, 0 );

	setCell( 0, 1, column1.y );
	setCell( 1, 1, column2.y );
	setCell( 2, 1, 0 );

	setCell( 0, 2, 0 );
	setCell( 1, 2, 0 );
	setCell( 2, 2, 1 );
}

Matrix3::Matrix3( const Vector3& column1, const Vector3& column2, const Vector3& column3 )
{
	setCell( 0, 0, column1.x );
	setCell( 1, 0, column2.x );
	setCell( 2, 0, column3.x );

	setCell( 0, 1, column1.y );
	setCell( 1, 1, column2.y );
	setCell( 2, 1, column3.y );

	setCell( 0, 2, column1.z );
	setCell( 1, 2, column2.z );
	setCell( 2, 2, column3.z );
}

Matrix3::Matrix3( const Matrix2& mat )
{
	setCell( 0, 0, mat.getCell(0,0) );
	setCell( 1, 0, mat.getCell(1,0) );
	setCell( 2, 0, 0 );

	setCell( 0, 1, mat.getCell(0,1) );
	setCell( 1, 1, mat.getCell(1,1) );
	setCell( 2, 1, 0 );

	setCell( 0, 2, 0 );
	setCell( 1, 2, 0 );
	setCell( 2, 2, 1 );
}

Matrix3 Matrix3::rotation( float angle )
{
	return Matrix3( Matrix2::rotation(angle) );
}
Matrix3 Matrix3::scale( float scale )
{
	return Matrix3( Matrix2::scale(scale) );
}
Matrix3 Matrix3::scaleX( float scale )
{
	return Matrix3( Matrix2::scaleX(scale) );
}
Matrix3 Matrix3::scaleY( float scale )
{
	return Matrix3( Matrix2::scaleY(scale) );
}
Matrix3 Matrix3::translation( float x, float y )
{
	Matrix3 result;
	result.setCell( 2, 0, x );
	result.setCell( 2, 1, y );
	return result;
}
Matrix3 Matrix3::translation( const Vector2& offset )
{
	return translation( offset.x, offset.y );
}

float Matrix3::getCell( int column, int row ) const
{
	return values[row*3+column];
}
void Matrix3::setCell( int column, int row, float value )
{
	values[row*3+column] = value;
}

Matrix3 Matrix3::transpose() const
{
	Matrix3 result;
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			result.setCell( i, j, getCell( j, i ) );
		}
	}
	return result;
}

Matrix3::operator float*()
{
	return &values[0];
}

void Matrix3::operator+=( const Matrix3& other )
{
	setCell( 0, 0, getCell( 0, 0 ) + other.getCell( 0, 0 ) );
	setCell( 1, 0, getCell( 1, 0 ) + other.getCell( 1, 0 ) );
	setCell( 2, 0, getCell( 2, 0 ) + other.getCell( 2, 0 ) );

	setCell( 0, 1, getCell( 0, 1 ) + other.getCell( 0, 1 ) );
	setCell( 1, 1, getCell( 1, 1 ) + other.getCell( 1, 1 ) );
	setCell( 2, 1, getCell( 2, 1 ) + other.getCell( 2, 1 ) );

	setCell( 0, 2, getCell( 0, 2 ) + other.getCell( 0, 2 ) );
	setCell( 1, 2, getCell( 1, 2 ) + other.getCell( 1, 2 ) );
	setCell( 2, 2, getCell( 2, 2 ) + other.getCell( 2, 2 ) );
}
void Matrix3::operator-=( const Matrix3& other )
{
	setCell( 0, 0, getCell( 0, 0 ) - other.getCell( 0, 0 ) );
	setCell( 1, 0, getCell( 1, 0 ) - other.getCell( 1, 0 ) );
	setCell( 2, 0, getCell( 2, 0 ) - other.getCell( 2, 0 ) );

	setCell( 0, 1, getCell( 0, 1 ) - other.getCell( 0, 1 ) );
	setCell( 1, 1, getCell( 1, 1 ) - other.getCell( 1, 1 ) );
	setCell( 2, 1, getCell( 2, 1 ) - other.getCell( 2, 1 ) );

	setCell( 0, 2, getCell( 0, 2 ) - other.getCell( 0, 2 ) );
	setCell( 1, 2, getCell( 1, 2 ) - other.getCell( 1, 2 ) );
	setCell( 2, 2, getCell( 2, 2 ) - other.getCell( 2, 2 ) );
}
void Matrix3::operator*=( const Matrix3& other )
{
	float temp00=getCell( 0, 0 ), temp10=getCell( 1, 0 ),
		temp01=getCell( 0, 1 ), temp11=getCell( 1, 1 ),
		temp02=getCell( 0, 2 ), temp12=getCell( 1, 2 );
	setCell( 0, 0, temp00*other.getCell( 0, 0 ) + temp10*other.getCell( 0, 1 ) + getCell( 2, 0 )*other.getCell( 0, 2 ) );
	setCell( 1, 0, temp00*other.getCell( 1, 0 ) + temp10*other.getCell( 1, 1 ) + getCell( 2, 0 )*other.getCell( 1, 2 ) );
	setCell( 2, 0, temp00*other.getCell( 2, 0 ) + temp10*other.getCell( 2, 1 ) + getCell( 2, 0 )*other.getCell( 2, 2 ) );

	setCell( 0, 1, temp01*other.getCell( 0, 0 ) + temp11*other.getCell( 0, 1 ) + getCell( 2, 1 )*other.getCell( 0, 2 ) );
	setCell( 1, 1, temp01*other.getCell( 1, 0 ) + temp11*other.getCell( 1, 1 ) + getCell( 2, 1 )*other.getCell( 1, 2 ) );
	setCell( 2, 1, temp01*other.getCell( 2, 0 ) + temp11*other.getCell( 2, 1 ) + getCell( 2, 1 )*other.getCell( 2, 2 ) );

	setCell( 0, 2, temp02*other.getCell( 0, 0 ) + temp12*other.getCell( 0, 1 ) + getCell( 2, 2 )*other.getCell( 0, 2 ) );
	setCell( 1, 2, temp02*other.getCell( 1, 0 ) + temp12*other.getCell( 1, 1 ) + getCell( 2, 2 )*other.getCell( 1, 2 ) );
	setCell( 2, 2, temp02*other.getCell( 2, 0 ) + temp12*other.getCell( 2, 1 ) + getCell( 2, 2 )*other.getCell( 2, 2 ) );
}

Matrix3 operator+( const Matrix3& left, const Matrix3& right )
{
	Matrix3 result;
	result.setCell( 0, 0, left.getCell( 0, 0 ) + right.getCell( 0, 0 ) );
	result.setCell( 1, 0, left.getCell( 1, 0 ) + right.getCell( 1, 0 ) );
	result.setCell( 2, 0, left.getCell( 2, 0 ) + right.getCell( 2, 0 ) );

	result.setCell( 0, 1, left.getCell( 0, 1 ) + right.getCell( 0, 1 ) );
	result.setCell( 1, 1, left.getCell( 1, 1 ) + right.getCell( 1, 1 ) );
	result.setCell( 2, 1, left.getCell( 2, 1 ) + right.getCell( 2, 1 ) );

	result.setCell( 0, 2, left.getCell( 0, 2 ) + right.getCell( 0, 2 ) );
	result.setCell( 1, 2, left.getCell( 1, 2 ) + right.getCell( 1, 2 ) );
	result.setCell( 2, 2, left.getCell( 2, 2 ) + right.getCell( 2, 2 ) );
	return result;
}

Matrix3 operator-( const Matrix3& left, const Matrix3& right )
{
	Matrix3 result;
	result.setCell( 0, 0, left.getCell( 0, 0 ) - right.getCell( 0, 0 ) );
	result.setCell( 1, 0, left.getCell( 1, 0 ) - right.getCell( 1, 0 ) );
	result.setCell( 2, 0, left.getCell( 2, 0 ) - right.getCell( 2, 0 ) );

	result.setCell( 0, 1, left.getCell( 0, 1 ) - right.getCell( 0, 1 ) );
	result.setCell( 1, 1, left.getCell( 1, 1 ) - right.getCell( 1, 1 ) );
	result.setCell( 2, 1, left.getCell( 2, 1 ) - right.getCell( 2, 1 ) );

	result.setCell( 0, 2, left.getCell( 0, 2 ) - right.getCell( 0, 2 ) );
	result.setCell( 1, 2, left.getCell( 1, 2 ) - right.getCell( 1, 2 ) );
	result.setCell( 2, 2, left.getCell( 2, 2 ) - right.getCell( 2, 2 ) );
	return result;
}

Matrix3 operator*( const Matrix3& left, const Matrix3& right )
{
	Matrix3 result;
	result.setCell( 0, 0, left.getCell( 0, 0 )*right.getCell( 0, 0 ) + left.getCell( 1, 0 )*right.getCell( 0, 1 ) + left.getCell( 2, 0 )*right.getCell( 0, 2 ) );
	result.setCell( 1, 0, left.getCell( 0, 0 )*right.getCell( 1, 0 ) + left.getCell( 1, 0 )*right.getCell( 1, 1 ) + left.getCell( 2, 0 )*right.getCell( 1, 2 ) );
	result.setCell( 2, 0, left.getCell( 0, 0 )*right.getCell( 2, 0 ) + left.getCell( 1, 0 )*right.getCell( 2, 1 ) + left.getCell( 2, 0 )*right.getCell( 2, 2 ) );

	result.setCell( 0, 1, left.getCell( 0, 1 )*right.getCell( 0, 0 ) + left.getCell( 1, 1 )*right.getCell( 0, 1 ) + left.getCell( 2, 1 )*right.getCell( 0, 2 ) );
	result.setCell( 1, 1, left.getCell( 0, 1 )*right.getCell( 1, 0 ) + left.getCell( 1, 1 )*right.getCell( 1, 1 ) + left.getCell( 2, 1 )*right.getCell( 1, 2 ) );
	result.setCell( 2, 1, left.getCell( 0, 1 )*right.getCell( 2, 0 ) + left.getCell( 1, 1 )*right.getCell( 2, 1 ) + left.getCell( 2, 1 )*right.getCell( 2, 2 ) );

	result.setCell( 0, 2, left.getCell( 0, 2 )*right.getCell( 0, 0 ) + left.getCell( 1, 2 )*right.getCell( 0, 1 ) + left.getCell( 2, 2 )*right.getCell( 0, 2 ) );
	result.setCell( 1, 2, left.getCell( 0, 2 )*right.getCell( 1, 0 ) + left.getCell( 1, 2 )*right.getCell( 1, 1 ) + left.getCell( 2, 2 )*right.getCell( 1, 2 ) );
	result.setCell( 2, 2, left.getCell( 0, 2 )*right.getCell( 2, 0 ) + left.getCell( 1, 2 )*right.getCell( 2, 1 ) + left.getCell( 2, 2 )*right.getCell( 2, 2 ) );
	return result;
}
Vector2 operator*( const Matrix3& left, const Vector2& right )
{
	Vector2 result;
	result.x = left.getCell( 0, 0 )*right.x + left.getCell( 1, 0 )*right.y + left.getCell( 2, 0 );
	result.y = left.getCell( 0, 1 )*right.x + left.getCell( 1, 1 )*right.y + left.getCell( 2, 1 );
	return result;
}
Vector3 operator*( const Matrix3& left, const Vector3& right )
{
	Vector3 result;
	result.x = left.getCell( 0, 0 )*right.x + left.getCell( 1, 0 )*right.y + left.getCell( 2, 0 )*right.z;
	result.y = left.getCell( 0, 1 )*right.x + left.getCell( 1, 1 )*right.y + left.getCell( 2, 1 )*right.z;
	result.z = left.getCell( 0, 2 )*right.x + left.getCell( 1, 2 )*right.y + left.getCell( 2, 2 )*right.z;
	return result;
}