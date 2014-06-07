/*
class Matrix4
*/

Matrix4::Matrix4()
{
	setCell( 0, 0, 1 );
	setCell( 1, 0, 0 );
	setCell( 2, 0, 0 );
	setCell( 3, 0, 0 );

	setCell( 0, 1, 0 );
	setCell( 1, 1, 1 );
	setCell( 2, 1, 0 );
	setCell( 3, 1, 0 );

	setCell( 0, 2, 0 );
	setCell( 1, 2, 0 );
	setCell( 2, 2, 1 );
	setCell( 3, 2, 0 );

	setCell( 0, 3, 0 );
	setCell( 1, 3, 0 );
	setCell( 2, 3, 0 );
	setCell( 3, 3, 1 );
}


Matrix4::Matrix4( const Matrix3& mat )
{
	setCell( 0, 0, mat.getCell(0,0) );
	setCell( 1, 0, mat.getCell(1,0) );
	setCell( 2, 0, mat.getCell(2,0) );
	setCell( 3, 0, 0 );

	setCell( 0, 1, mat.getCell(0,1) );
	setCell( 1, 1, mat.getCell(1,1) );
	setCell( 2, 1, mat.getCell(2,1) );
	setCell( 3, 1, 0 );

	setCell( 0, 2, mat.getCell(0,2) );
	setCell( 1, 2, mat.getCell(1,2) );
	setCell( 2, 2, mat.getCell(2,2) );
	setCell( 3, 2, 0 );

	setCell( 0, 3, 0 );
	setCell( 1, 3, 0 );
	setCell( 2, 3, 0 );
	setCell( 3, 3, 1 );
}

Matrix4::Matrix4( const Matrix2& mat )
{
	setCell( 0, 0, mat.getCell(0,0) );
	setCell( 1, 0, mat.getCell(1,0) );
	setCell( 2, 0, 0 );
	setCell( 3, 0, 0 );

	setCell( 0, 1, mat.getCell(0,1) );
	setCell( 1, 1, mat.getCell(1,1) );
	setCell( 2, 1, 0 );
	setCell( 3, 1, 0 );

	setCell( 0, 2, 0 );
	setCell( 1, 2, 0 );
	setCell( 2, 2, 1 );
	setCell( 3, 2, 0 );

	setCell( 0, 3, 0 );
	setCell( 1, 3, 0 );
	setCell( 2, 3, 0 );
	setCell( 3, 3, 1 );
}

Matrix4 Matrix4::rotation( Vector3 v, float angle )
{
	//http://en.wikipedia.org/wiki/Rotation_matrix
	Matrix4 result;
	float cosT = std::cos(angle);
	float sinT = std::sin(angle);
	result.setCell( 0, 0, cosT + v.x*v.x*(1-cosT) );
	result.setCell( 1, 0, v.x*v.y*(1-cosT) - v.z*sinT );
	result.setCell( 2, 0, v.x*v.z*(1-cosT) + v.y*sinT );

	result.setCell( 0, 1, v.x*v.y*(1-cosT) + v.z*sinT );
	result.setCell( 1, 1, cosT + v.y*v.y*(1-cosT) );
	result.setCell( 2, 1, v.y*v.z*(1-cosT) - v.x*sinT );

	result.setCell( 0, 2, v.x*v.z*(1-cosT) - v.y*sinT );
	result.setCell( 1, 2, v.y*v.z*(1-cosT) + v.x*sinT );
	result.setCell( 2, 2, cosT + v.z*v.z*(1-cosT) );
	return result;
}

Matrix4 Matrix4::translation( float x, float y )
{
	Matrix4 result;
	result.setCell( 3, 0, x );
	result.setCell( 3, 1, y );
	return result;
}

Matrix4 Matrix4::translation( const Vector2& offset )
{
	return translation( offset.x, offset.y );
}

Matrix4 Matrix4::translation( float x, float y, float z )
{
	Matrix4 result;
	result.setCell( 3, 0, x );
	result.setCell( 3, 1, y );
	result.setCell( 3, 2, z );
	return result;
}
Matrix4 Matrix4::translation( const Vector3& offset )
{
	return translation( offset.x, offset.y, offset.z );
}

float Matrix4::getCell( int column, int row ) const
{
	return values[row*4+column];
}
void Matrix4::setCell( int column, int row, float value )
{
	values[row*4+column] = value;
}

Matrix4 Matrix4::transpose() const
{
	Matrix4 result;
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			result.setCell( i, j, getCell( j, i ) );
		}
	}
	return result;
}

Matrix4::operator float*()
{
	return &values[0];
}



Matrix4 operator*( const Matrix4& left, const Matrix4& right )
{
	Matrix4 result;
	result.setCell( 0, 0, left.getCell( 0, 0 )*right.getCell( 0, 0 ) + left.getCell( 1, 0 )*right.getCell( 0, 1 ) + left.getCell( 2, 0 )*right.getCell( 0, 2 ) + left.getCell( 3, 0 )*right.getCell( 0, 3 ) );
	result.setCell( 1, 0, left.getCell( 0, 0 )*right.getCell( 1, 0 ) + left.getCell( 1, 0 )*right.getCell( 1, 1 ) + left.getCell( 2, 0 )*right.getCell( 1, 2 ) + left.getCell( 3, 0 )*right.getCell( 1, 3 ) );
	result.setCell( 2, 0, left.getCell( 0, 0 )*right.getCell( 2, 0 ) + left.getCell( 1, 0 )*right.getCell( 2, 1 ) + left.getCell( 2, 0 )*right.getCell( 2, 2 ) + left.getCell( 3, 0 )*right.getCell( 2, 3 ) );
	result.setCell( 3, 0, left.getCell( 0, 0 )*right.getCell( 3, 0 ) + left.getCell( 1, 0 )*right.getCell( 3, 1 ) + left.getCell( 2, 0 )*right.getCell( 3, 2 ) + left.getCell( 3, 0 )*right.getCell( 3, 3 ) );

	result.setCell( 0, 1, left.getCell( 0, 1 )*right.getCell( 0, 0 ) + left.getCell( 1, 1 )*right.getCell( 0, 1 ) + left.getCell( 2, 1 )*right.getCell( 0, 2 ) + left.getCell( 3, 1 )*right.getCell( 0, 3 ) );
	result.setCell( 1, 1, left.getCell( 0, 1 )*right.getCell( 1, 0 ) + left.getCell( 1, 1 )*right.getCell( 1, 1 ) + left.getCell( 2, 1 )*right.getCell( 1, 2 ) + left.getCell( 3, 1 )*right.getCell( 1, 3 ) );
	result.setCell( 2, 1, left.getCell( 0, 1 )*right.getCell( 2, 0 ) + left.getCell( 1, 1 )*right.getCell( 2, 1 ) + left.getCell( 2, 1 )*right.getCell( 2, 2 ) + left.getCell( 3, 1 )*right.getCell( 2, 3 ) );
	result.setCell( 3, 1, left.getCell( 0, 1 )*right.getCell( 3, 0 ) + left.getCell( 1, 1 )*right.getCell( 3, 1 ) + left.getCell( 2, 1 )*right.getCell( 3, 2 ) + left.getCell( 3, 1 )*right.getCell( 3, 3 ) );

	result.setCell( 0, 2, left.getCell( 0, 2 )*right.getCell( 0, 0 ) + left.getCell( 1, 2 )*right.getCell( 0, 1 ) + left.getCell( 2, 2 )*right.getCell( 0, 2 ) + left.getCell( 3, 2 )*right.getCell( 0, 3 ) );
	result.setCell( 1, 2, left.getCell( 0, 2 )*right.getCell( 1, 0 ) + left.getCell( 1, 2 )*right.getCell( 1, 1 ) + left.getCell( 2, 2 )*right.getCell( 1, 2 ) + left.getCell( 3, 2 )*right.getCell( 1, 3 ) );
	result.setCell( 2, 2, left.getCell( 0, 2 )*right.getCell( 2, 0 ) + left.getCell( 1, 2 )*right.getCell( 2, 1 ) + left.getCell( 2, 2 )*right.getCell( 2, 2 ) + left.getCell( 3, 2 )*right.getCell( 2, 3 ) );
	result.setCell( 3, 2, left.getCell( 0, 2 )*right.getCell( 3, 0 ) + left.getCell( 1, 2 )*right.getCell( 3, 1 ) + left.getCell( 2, 2 )*right.getCell( 3, 2 ) + left.getCell( 3, 2 )*right.getCell( 3, 3 ) );

	result.setCell( 0, 3, left.getCell( 0, 3 )*right.getCell( 0, 0 ) + left.getCell( 1, 3 )*right.getCell( 0, 1 ) + left.getCell( 2, 3 )*right.getCell( 0, 2 ) + left.getCell( 3, 3 )*right.getCell( 0, 3 ) );
	result.setCell( 1, 3, left.getCell( 0, 3 )*right.getCell( 1, 0 ) + left.getCell( 1, 3 )*right.getCell( 1, 1 ) + left.getCell( 2, 3 )*right.getCell( 1, 2 ) + left.getCell( 3, 3 )*right.getCell( 1, 3 ) );
	result.setCell( 2, 3, left.getCell( 0, 3 )*right.getCell( 2, 0 ) + left.getCell( 1, 3 )*right.getCell( 2, 1 ) + left.getCell( 2, 3 )*right.getCell( 2, 2 ) + left.getCell( 3, 3 )*right.getCell( 2, 3 ) );
	result.setCell( 3, 3, left.getCell( 0, 3 )*right.getCell( 3, 0 ) + left.getCell( 1, 3 )*right.getCell( 3, 1 ) + left.getCell( 2, 3 )*right.getCell( 3, 2 ) + left.getCell( 3, 3 )*right.getCell( 3, 3 ) );
	return result;
}
Vector2 operator*( const Matrix4& left, const Vector2& right )
{
	Vector2 result;
	result.x = left.getCell( 0, 0 )*right.x + left.getCell( 1, 0 )*right.y + left.getCell( 2, 0 ) + left.getCell( 3, 0 );
	result.y = left.getCell( 0, 1 )*right.x + left.getCell( 1, 1 )*right.y + left.getCell( 2, 1 ) + left.getCell( 3, 1 );
	return result;
}
Vector3 operator*( const Matrix4& left, const Vector3& right )
{
	Vector3 result;
	result.x = left.getCell( 0, 0 )*right.x + left.getCell( 1, 0 )*right.y + left.getCell( 2, 0 )*right.z + left.getCell( 3, 0 );
	result.y = left.getCell( 0, 1 )*right.x + left.getCell( 1, 1 )*right.y + left.getCell( 2, 1 )*right.z + left.getCell( 3, 1 );
	result.z = left.getCell( 0, 2 )*right.x + left.getCell( 1, 2 )*right.y + left.getCell( 2, 2 )*right.z + left.getCell( 3, 2 );
	return result;
}


Matrix4 Matrix4::inverse() 
{
	Matrix4 result;
	float det = calcDeterminant();
	result.setCell(0,0, (getCell(1,2)*getCell(2,3)*getCell(3,1) - getCell(1,3)*getCell(2,2)*getCell(3,1) + getCell(1,3)*getCell(2,1)*getCell(3,2) - getCell(1,1)*getCell(2,3)*getCell(3,2) - getCell(1,2)*getCell(2,1)*getCell(3,3) + getCell(1,1)*getCell(2,2)*getCell(3,3))/det );
	result.setCell(0,1, (getCell(0,3)*getCell(2,2)*getCell(3,1) - getCell(0,2)*getCell(2,3)*getCell(3,1) - getCell(0,3)*getCell(2,1)*getCell(3,2) + getCell(0,1)*getCell(2,3)*getCell(3,2) + getCell(0,2)*getCell(2,1)*getCell(3,3) - getCell(0,1)*getCell(2,2)*getCell(3,3))/det );
	result.setCell(0,2, (getCell(0,2)*getCell(1,3)*getCell(3,1) - getCell(0,3)*getCell(1,2)*getCell(3,1) + getCell(0,3)*getCell(1,1)*getCell(3,2) - getCell(0,1)*getCell(1,3)*getCell(3,2) - getCell(0,2)*getCell(1,1)*getCell(3,3) + getCell(0,1)*getCell(1,2)*getCell(3,3))/det );
	result.setCell(0,3, (getCell(0,3)*getCell(1,2)*getCell(2,1) - getCell(0,2)*getCell(1,3)*getCell(2,1) - getCell(0,3)*getCell(1,1)*getCell(2,2) + getCell(0,1)*getCell(1,3)*getCell(2,2) + getCell(0,2)*getCell(1,1)*getCell(2,3) - getCell(0,1)*getCell(1,2)*getCell(2,3))/det );
	result.setCell(1,0, (getCell(1,3)*getCell(2,2)*getCell(3,0) - getCell(1,2)*getCell(2,3)*getCell(3,0) - getCell(1,3)*getCell(2,0)*getCell(3,2) + getCell(1,0)*getCell(2,3)*getCell(3,2) + getCell(1,2)*getCell(2,0)*getCell(3,3) - getCell(1,0)*getCell(2,2)*getCell(3,3))/det );
	result.setCell(1,1, (getCell(0,2)*getCell(2,3)*getCell(3,0) - getCell(0,3)*getCell(2,2)*getCell(3,0) + getCell(0,3)*getCell(2,0)*getCell(3,2) - getCell(0,0)*getCell(2,3)*getCell(3,2) - getCell(0,2)*getCell(2,0)*getCell(3,3) + getCell(0,0)*getCell(2,2)*getCell(3,3))/det );
	result.setCell(1,2, (getCell(0,3)*getCell(1,2)*getCell(3,0) - getCell(0,2)*getCell(1,3)*getCell(3,0) - getCell(0,3)*getCell(1,0)*getCell(3,2) + getCell(0,0)*getCell(1,3)*getCell(3,2) + getCell(0,2)*getCell(1,0)*getCell(3,3) - getCell(0,0)*getCell(1,2)*getCell(3,3))/det );
	result.setCell(1,3, (getCell(0,2)*getCell(1,3)*getCell(2,0) - getCell(0,3)*getCell(1,2)*getCell(2,0) + getCell(0,3)*getCell(1,0)*getCell(2,2) - getCell(0,0)*getCell(1,3)*getCell(2,2) - getCell(0,2)*getCell(1,0)*getCell(2,3) + getCell(0,0)*getCell(1,2)*getCell(2,3))/det );
	result.setCell(2,0, (getCell(1,1)*getCell(2,3)*getCell(3,0) - getCell(1,3)*getCell(2,1)*getCell(3,0) + getCell(1,3)*getCell(2,0)*getCell(3,1) - getCell(1,0)*getCell(2,3)*getCell(3,1) - getCell(1,1)*getCell(2,0)*getCell(3,3) + getCell(1,0)*getCell(2,1)*getCell(3,3))/det );
	result.setCell(2,1, (getCell(0,3)*getCell(2,1)*getCell(3,0) - getCell(0,1)*getCell(2,3)*getCell(3,0) - getCell(0,3)*getCell(2,0)*getCell(3,1) + getCell(0,0)*getCell(2,3)*getCell(3,1) + getCell(0,1)*getCell(2,0)*getCell(3,3) - getCell(0,0)*getCell(2,1)*getCell(3,3))/det );
	result.setCell(2,2, (getCell(0,1)*getCell(1,3)*getCell(3,0) - getCell(0,3)*getCell(1,1)*getCell(3,0) + getCell(0,3)*getCell(1,0)*getCell(3,1) - getCell(0,0)*getCell(1,3)*getCell(3,1) - getCell(0,1)*getCell(1,0)*getCell(3,3) + getCell(0,0)*getCell(1,1)*getCell(3,3))/det );
	result.setCell(2,3, (getCell(0,3)*getCell(1,1)*getCell(2,0) - getCell(0,1)*getCell(1,3)*getCell(2,0) - getCell(0,3)*getCell(1,0)*getCell(2,1) + getCell(0,0)*getCell(1,3)*getCell(2,1) + getCell(0,1)*getCell(1,0)*getCell(2,3) - getCell(0,0)*getCell(1,1)*getCell(2,3))/det );
	result.setCell(3,0, (getCell(1,2)*getCell(2,1)*getCell(3,0) - getCell(1,1)*getCell(2,2)*getCell(3,0) - getCell(1,2)*getCell(2,0)*getCell(3,1) + getCell(1,0)*getCell(2,2)*getCell(3,1) + getCell(1,1)*getCell(2,0)*getCell(3,2) - getCell(1,0)*getCell(2,1)*getCell(3,2))/det );
	result.setCell(3,1, (getCell(0,1)*getCell(2,2)*getCell(3,0) - getCell(0,2)*getCell(2,1)*getCell(3,0) + getCell(0,2)*getCell(2,0)*getCell(3,1) - getCell(0,0)*getCell(2,2)*getCell(3,1) - getCell(0,1)*getCell(2,0)*getCell(3,2) + getCell(0,0)*getCell(2,1)*getCell(3,2))/det );
	result.setCell(3,2, (getCell(0,2)*getCell(1,1)*getCell(3,0) - getCell(0,1)*getCell(1,2)*getCell(3,0) - getCell(0,2)*getCell(1,0)*getCell(3,1) + getCell(0,0)*getCell(1,2)*getCell(3,1) + getCell(0,1)*getCell(1,0)*getCell(3,2) - getCell(0,0)*getCell(1,1)*getCell(3,2))/det );
	result.setCell(3,3, (getCell(0,1)*getCell(1,2)*getCell(2,0) - getCell(0,2)*getCell(1,1)*getCell(2,0) + getCell(0,2)*getCell(1,0)*getCell(2,1) - getCell(0,0)*getCell(1,2)*getCell(2,1) - getCell(0,1)*getCell(1,0)*getCell(2,2) + getCell(0,0)*getCell(1,1)*getCell(2,2))/det );
	return result;
}



float Matrix4::calcDeterminant()  
{
	float value;
	value =
	getCell(0,3)*getCell(1,2)*getCell(2,1)*getCell(3,0) - getCell(0,2)*getCell(1,3)*getCell(2,1)*getCell(3,0) - getCell(0,3)*getCell(1,1)*getCell(2,2)*getCell(3,0) + getCell(0,1)*getCell(1,3)*getCell(2,2)*getCell(3,0)+
	getCell(0,2)*getCell(1,1)*getCell(2,3)*getCell(3,0) - getCell(0,1)*getCell(1,2)*getCell(2,3)*getCell(3,0) - getCell(0,3)*getCell(1,2)*getCell(2,0)*getCell(3,1) + getCell(0,2)*getCell(1,3)*getCell(2,0)*getCell(3,1)+
	getCell(0,3)*getCell(1,0)*getCell(2,2)*getCell(3,1) - getCell(0,0)*getCell(1,3)*getCell(2,2)*getCell(3,1) - getCell(0,2)*getCell(1,0)*getCell(2,3)*getCell(3,1) + getCell(0,0)*getCell(1,2)*getCell(2,3)*getCell(3,1)+
	getCell(0,3)*getCell(1,1)*getCell(2,0)*getCell(3,2) - getCell(0,1)*getCell(1,3)*getCell(2,0)*getCell(3,2) - getCell(0,3)*getCell(1,0)*getCell(2,1)*getCell(3,2) + getCell(0,0)*getCell(1,3)*getCell(2,1)*getCell(3,2)+
	getCell(0,1)*getCell(1,0)*getCell(2,3)*getCell(3,2) - getCell(0,0)*getCell(1,1)*getCell(2,3)*getCell(3,2) - getCell(0,2)*getCell(1,1)*getCell(2,0)*getCell(3,3) + getCell(0,1)*getCell(1,2)*getCell(2,0)*getCell(3,3)+
	getCell(0,2)*getCell(1,0)*getCell(2,1)*getCell(3,3) - getCell(0,0)*getCell(1,2)*getCell(2,1)*getCell(3,3) - getCell(0,1)*getCell(1,0)*getCell(2,2)*getCell(3,3) + getCell(0,0)*getCell(1,1)*getCell(2,2)*getCell(3,3);
	return value;
}