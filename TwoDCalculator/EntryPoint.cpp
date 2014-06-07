#include "RenderUI.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

glm::vec2 left, right, result, //Basic Vector Equation 
	orig, norm, cw, ccw, //Perpendicular
	vec1, vec2, pro, rej, //Dot Product
	a, b, aMinB, aPort, bPort, lerpResult, //Lerp

	linearResult; //Linear Transformation
	

glm::vec3 affineResults[5]; //Affine Transformation

glm::mat3 transforms[30], current; //2D Matrix Vertices
glm::vec2 *lines; //Shape for 2D Matrix Vertices

glm::mat4 threeTransforms[30], full; //3D Matrix tab

void myBasicVectorEquationCallback(const BasicVectorEquationInfo& data)
{
	left.x = data.x1;
	left.y = data.y1;
	right.x = data.x2;
	right.y = data.y2;
	left=left*data.scalar1;
	right=right*data.scalar2;
	result = (data.add) ? left + right : left - right ;
}

void myPerpendicularDataCallback(const PerpendicularData& data)
{
	orig.x = data.x;
	orig.y = data.y;
	norm = glm::normalize( orig );
	cw.x = orig.y;
	cw.y = -orig.x;

	ccw.x = -orig.y;
	ccw.y = orig.x;
}

void myDotProductDataCallback(const DotProductData& data)
{
	vec1.x = data.v1i;
	vec1.y = data.v1j;
	vec2.x = data.v2i;
	vec2.y = data.v2j;

	if (data.projectOntoLeftVector)
	{
		float proj = glm::dot(vec1,vec2)/glm::length(vec1);
		pro = glm::normalize(vec1) * proj;
		rej = vec2 - glm::normalize(vec1)*proj;
	}
	else
	{
		float proj = glm::dot(vec1,vec2)/glm::length(vec2);
		pro = glm::normalize(vec2) * proj;
		rej = vec1 - glm::normalize(vec2)*proj;
	}
}

void myLerpDataCallback(const LerpData& data)
{
	a.x = data.a_i;
	a.y = data.a_j;
	b.x = data.b_i;
	b.y = data.b_j;
	aMinB = b-a;
	aPort = a*(1-data.beta);
	bPort = b*data.beta;
	lerpResult = a + aMinB * data.beta;
}

void myLinearTransformationCallback(const LinearTransformationData& data)
{
	glm::vec2 one( data.m00, data.m10 );
	glm::vec2 two( data.m01, data.m11 );
	glm::vec2 vec( data.v0, data.v1 );
	linearResult = glm::mat2(one,two)*vec;
}

void myAffineTransformationCallback(const AffineTransformationData& data)
{
	const float* dat = data.data;
	glm::mat3 affine;
	int i = 0;
	affine[0][0] = dat[i++];
	affine[1][0] = dat[i++];
	affine[2][0] = dat[i++];

	affine[0][1] = dat[i++];
	affine[1][1] = dat[i++];
	affine[2][1] = dat[i++];

	affine[0][2] = dat[i++];
	affine[1][2] = dat[i++];
	affine[2][2] = dat[i++];

	for( int j = 0; j < 5; j++ )
	{
		float x = dat[i++];
		float y = dat[i++];
		float z = dat[i++];
		affineResults[j] = affine * glm::vec3( x, y, z );
	}
}

void myMatrixTransformCallback2D(const MatrixTransformData2D& data)
{
	glm::mat4 temp;
	const float PI = 3.14159f;
	temp *= glm::scale( temp, glm::vec3(data.scaleX,data.scaleY,1) );
	temp *= glm::rotate( temp, 180*data.rotate/PI, glm::vec3(0,0,1) );

	glm::mat3 threeMat;
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			threeMat[i][j] = temp[i][j];
		}
	}
	//Translate
	glm::mat3 trans;
	trans[0][2] = data.translateX;
	trans[1][2] = data.translateY;
	threeMat *= trans;
	transforms[data.selectedMatrix] = threeMat;

	current = glm::mat3();
	for( int i = 0; i < 30; i++ )
	{
		current*=transforms[i];
	}
}

void myMatrixTransformCallback3D(const MatrixTransformData3D& data)
{
	glm::mat4 temp;
	const float PI = 3.14159f;
	temp = glm::translate( temp, glm::vec3( data.translateX, data.translateY, data.translateZ ) )
		* glm::rotate( temp, 180*data.rotateX/PI, glm::vec3(1,0,0) ) 
		* glm::rotate( temp, 180*data.rotateY/PI, glm::vec3(0,1,0) ) 
		* glm::rotate( temp, 180*data.rotateZ/PI, glm::vec3(0,0,1) ) 
		* glm::scale( temp, glm::vec3(data.scaleX,data.scaleY,data.scaleZ) );

	threeTransforms[data.selectedMatrix] = temp;

	full = glm::mat4();
	for( int i = 0; i < 30; i++ )
	{
		full*=threeTransforms[i];
	}
}


void setUpLines( glm::vec2 *lines )
{
	const float size = .3f;
	glm::vec2 topLeft( -size, size );
	glm::vec2 topRight( size, size );
	glm::vec2 botLeft( -size, -size );
	glm::vec2 botRight( size, -size );
	glm::vec2 midLeft( -size/2, 0 );
	glm::vec2 midRight( size/2, 0 );

	glm::vec2 builtLines[] = {
		topLeft, topRight,
		topRight, midRight,
		midRight, botRight,
		botRight, botLeft,
		botLeft, midLeft,
		midLeft, topLeft
	};

	for( int i = 0; i < 12; i++ )
	{
		lines[i] = builtLines[i];
	}
}

#define p(a) reinterpret_cast<const float*>(&a)


int main(int argc, char* argv[])
{
	RenderUI renderUI;

	glm::vec2 temp[12];
	lines = temp;

	setUpLines( lines );

	renderUI.setBasicVectorEquationData( myBasicVectorEquationCallback, p(left), p(right), p(result) );
	renderUI.setPerpendicularData( p(orig), p(norm), p(cw), p(ccw), myPerpendicularDataCallback );
	renderUI.setDotProductData( p(vec1), p(vec2), p(pro), p(rej), myDotProductDataCallback );
	renderUI.setLerpData( p(a), p(b), p(aMinB), p(aPort), p(bPort), p(lerpResult), myLerpDataCallback );
	renderUI.setLinearTransformationData( p(linearResult), myLinearTransformationCallback );
	renderUI.setAffineTransformationData( p(*affineResults), myAffineTransformationCallback );
	renderUI.set2DMatrixVerticesTransformData( p(lines[0]), 6, p(*transforms), p(current), myMatrixTransformCallback2D );
	renderUI.set3DMatrixCallback( p(*threeTransforms), p(full), myMatrixTransformCallback3D );

	if( ! renderUI.initialize(argc, argv))
		return -1;
	return renderUI.run();
}