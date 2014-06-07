#include "Sierpinski.h"
#include <iostream>
#include "GLHelper.h"

using glm::vec3;
using glm::vec2;

vec3* Sierpinski::generateSierpinskiTriangle( int iterations )
{
	int numberOfTriangles = 1;
	for( int i = 0; i < iterations; i++ )
	{
		numberOfTriangles += std::pow( 3, i );
	}

	int resultSize =  sizeof( vec3 ) * 2 * 3 * numberOfTriangles;

	vec3* result = (vec3*) malloc( resultSize );

	result[0] = vec3( -0.8f, -0.8f, +0.0f );
	result[1] = vec3( +1.0f, +0.0f, +0.0f );
	result[2] = vec3( +0.8f, -0.8f, +0.0f );
	result[3] = vec3( +0.0f, +1.0f, +0.0f );
	result[4] = vec3( +0.0f, +0.8f, +0.0f );
	result[5] = vec3( +0.0f, +0.0f, +1.0f );

	//std::cout << "Ending up " << 
	sierpinskiTriangleStep( iterations, result, result[0], result[2], result[4], 1 );

	return result;
}

int Sierpinski::sierpinskiTriangleStep( int iterations, vec3* triangles, vec3& p1, vec3& p2, vec3& p3, int index )
{
	if( iterations > 0 )
	{
		int pIndex = index;
		triangles[ 6 * index     ] = (p1 + p2) / 2.0f;
		triangles[ 6 * index + 1 ] = vec3( +1.0f, +1.0f, +0.0f );
		triangles[ 6 * index + 2 ] = (p2 + p3) / 2.0f;
		triangles[ 6 * index + 3 ] = vec3( +0.0f, +1.0f, +1.0f );
		triangles[ 6 * index + 4 ] = (p3 + p1) / 2.0f;
		triangles[ 6 * index + 5 ] = vec3( +1.0f, +0.0f, +1.0f );
		index++;

	
		index = sierpinskiTriangleStep( iterations - 1, triangles, triangles[6 * pIndex    ], triangles[6 * pIndex + 2], p2, index );
		index = sierpinskiTriangleStep( iterations - 1, triangles, triangles[6 * pIndex + 2], triangles[6 * pIndex + 4], p3, index );
		index = sierpinskiTriangleStep( iterations - 1, triangles, triangles[6 * pIndex + 4], triangles[6 * pIndex    ], p1, index );
	}

	return index;
}

Model Sierpinski::generateSierpinskiPyramid( int iterations, vec3& p1, vec3& p2, vec3& p3, vec3& p4 )
{
	/*int numberOfPyramids = 1;
	for( int i = 0; i < iterations; i++ )
	{
		numberOfPyramids += std::pow( 4, i );
	}*/

	int numberOfPyramids = std::pow( 4, iterations );

	int resultSize =  sizeof( vec3 ) * 3 * 3 * 4 * numberOfPyramids;

	vec3* result = (vec3*) malloc( resultSize );

	//std::cout << result;
	//std::cout << "Ending up " << 
	sierpinskiPyramidStep( iterations, result, p1, p2, p3, p4, 0 );// << std::endl;

	return Model( result, resultSize/(sizeof(Vertex)));
}

int Sierpinski::sierpinskiPyramidStep( int iterations, vec3* triangles, vec3& p1, vec3& p2, vec3& p3, vec3& p4, int index )
{
	if( iterations == 1 )
	{
		int pIndex = index;

		vec3 all[] = { p1, p2, p3, p4 };

		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < 3; j++ )
			{
				triangles[9 * index    ] = all[i];
				triangles[9 * index + 1] = vec3( +1.0f, +0.0f, +0.0f );

				for( int k = 1; k <= 2; k++ )
				{
					int next = (i + j + k) % 4;
					if (next == i)
						next = (next + 1) % 4;
					triangles[ 3*k + 9 * index    ] = ( all[i] + all[next] ) / 2.0f;
					triangles[ 3*k + 9 * index + 1] = vec3( +0.0f, (k==1) ? +1.0f : +0.0f, (k==1) ? +0.0f : +1.0f );
				}

				GLHelper::calcNormal( triangles, 9*index );

				index++;
			}
			
			triangles[ 9 * index    ] = ( all[i] + all[(i + 1) % 4] ) / 2.0f;
			triangles[ 9 * index + 1] = vec3( +1.0f, +0.0f, +0.0f );
			triangles[ 9 * index + 3] = ( all[i] + all[(i + 2) % 4] ) / 2.0f;
			triangles[ 9 * index + 4] = vec3( +0.0f, +1.0f, +0.0f );
			triangles[ 9 * index + 6] = ( all[i] + all[(i + 3) % 4] ) / 2.0f;
			triangles[ 9 * index + 7] = vec3( +0.0f, +0.0f, +1.0f );

			GLHelper::calcNormal( triangles, 9*index );

			index++;
		}
	}
	if( iterations > 0 )
	{
		index = sierpinskiPyramidStep( iterations - 1, triangles, p1, (p1+p2)/2.0f, (p1+p3)/2.0f, (p1+p4)/2.0f, index );
		index = sierpinskiPyramidStep( iterations - 1, triangles, (p1+p2)/2.0f, p2, (p3+p2)/2.0f, (p4+p2)/2.0f, index );
		index = sierpinskiPyramidStep( iterations - 1, triangles, (p1+p3)/2.0f, (p2+p3)/2.0f, p3, (p4+p3)/2.0f, index );
		index = sierpinskiPyramidStep( iterations - 1, triangles, (p1+p4)/2.0f, (p2+p4)/2.0f, (p3+p4)/2.0f, p4, index );
	}

	return index;
}
