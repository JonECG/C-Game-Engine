#include "Terrain.h"
#include "GLHelper.h"
#include "Qt\qimage.h"

using glm::vec2;
using glm::vec3;

Terrain::Terrain()
{
	//position = vec3();
	//dimensions = vec3();
	//model = Model();
}

void Terrain::smoothHeights( int radius, int iterations )
{
	for( int iteration = 0; iteration < iterations; iteration++ )
	{
		GLfloat * smoothedHeights = new GLfloat[ (xSegments+1) * (ySegments+1) ];

		for( int i = 0; i <= xSegments; i++ )
		{
			for( int j = 0; j <= ySegments; j++ )
			{
				GLfloat heightSum = 0;
				GLint count = 0;

				//std::cout << "Smoothing " << i << " " << j << std::endl;

				for( int k = std::max( 0, i - radius ); k < i + radius && k <= xSegments; k++ )
				{
					for( int l = std::max( 0, j - radius ); l < j + radius && l <= ySegments; l++ )
					{
						//std::cout << "    with" << k << " " << l << std::endl;
						heightSum += heights[ k * (ySegments+1) + l ];
						count += 1;
					}
				}
				smoothedHeights[ i * (ySegments+1) + j ] = heightSum / count;
			}
		}

		delete [] heights;

		heights = smoothedHeights;
	}
}

void Terrain::normalizeHeights()
{
	GLfloat small = 1, large = 0;

	for( int i = 0; i <= xSegments; i++ )
	{
		for( int j = 0; j <= ySegments; j++ )
		{
			small = std::min( small, heights[ i * (ySegments+1) + j ] );
			large = std::max( large, heights[ i * (ySegments+1) + j ] );
		}
	}

	GLfloat diff = large - small;

	if ( diff != 0 )
	{
		for( int i = 0; i <= xSegments; i++ )
		{
			for( int j = 0; j <= ySegments; j++ )
			{
				heights[ i * (ySegments+1) + j ] =  ( heights[ i * (ySegments+1) + j ] - small ) / diff;
			}
		}
	}
}

void Terrain::generateModel()
{
	int tris = xSegments * ySegments * 2;
	Vertex * vertices = new Vertex[ tris * 3 ];

	for( int i = 0; i < xSegments; i++ )
	{
		for( int j = 0; j < ySegments; j++ )
		{
			for( int k = 0; k < 2; k++ )
			{
				int offset = 6 * ( i * xSegments + j ) + 3 * k;
				for( int l = 0; l < 3; l++ )
				{
					int index = (k*2+l)%4;
					int y = index / 2;
					int x = abs( y - index % 2 );

					vertices[offset + l].position = 
						vec3( dimensions.x * (i+x)/xSegments, 
						dimensions.y * heights[ (i+x) * (ySegments+1) + j+y ], 
						dimensions.z * (j+y)/ySegments ) - dimensions/2.0f;

					vertices[offset + l].color = vec3( 0.0f, 0.4f, 1.0f );
					vertices[offset + l].uv = vec2( float(i)/xSegments, float(j)/ySegments );
				}
				GLHelper::calcNormal( vertices[ offset ], vertices[ offset + 1 ], vertices[ offset + 2 ] );
			}
		}
	}

	model = GLHelper::indexVertices( vertices, tris * 3, false );
	//model = Model( vertices, tris * 3 );
}
	
Terrain Terrain::createRandom(vec3 position, vec3 dimensions, GLint xSegments, GLint ySegments )
{
	Terrain result;
	result.position = position;
	result.dimensions = dimensions;
	result.xSegments = xSegments;
	result.ySegments = ySegments;

	result.heights = new GLfloat[ (xSegments+1) * (ySegments+1) ];

	for( int i = 0; i <= xSegments; i++ )
	{
		for( int j = 0; j <= ySegments; j++ )
		{
			result.heights[ i * (ySegments+1) + j ] = RandomHelper::random();
		}
	}

	result.smoothHeights( 4, 5 );
	result.normalizeHeights();

	result.generateModel();

	return result;
}

Terrain Terrain::createFromHeightmap( char * fileName, vec3 position, vec3 dimensions, GLint xSegments, GLint ySegments )
{
	Terrain result;
	result.position = position;
	result.dimensions = dimensions;
	result.xSegments = xSegments;
	result.ySegments = ySegments;

	QImage image = QImage();
	image.load( fileName );

	std::cout << image.width() << std::endl;

	result.heights = new GLfloat[ (xSegments+1) * (ySegments+1) ];

	for( int i = 0; i <= xSegments; i++ )
	{
		for( int j = 0; j <= ySegments; j++ )
		{
			QRgb pix = image.pixel( image.width() * i /( xSegments + 1), image.height() * j / (ySegments+1) );
			result.heights[ i * (ySegments+1) + j ] = qRed( pix ) / 255.0f;
		}
	}

	result.smoothHeights( 4, 5 );

	result.generateModel();

	return result;
}

GLfloat max( GLfloat a, GLfloat b )
{
	return ( a > b ) ? a : b;
}

GLfloat min( GLfloat a, GLfloat b )
{
	return ( a < b ) ? a : b;
}

glm::vec3 Terrain::fitPosition( vec3 queryPosition )
{
	vec3 result;

	result.x = max( position.x - dimensions.x/2.0f, min( queryPosition.x, position.x + dimensions.x/2.0f ) );
	result.z = max( position.z - dimensions.z/2.0f, min( queryPosition.z, position.z + dimensions.z/2.0f ) );

	GLfloat xDiff, zDiff, xPerc, zPerc, from, mid, end;
	xDiff = result.x - position.x + dimensions.x/2.0f;
	zDiff = result.z - position.z + dimensions.z/2.0f;

	int xSeg = (int)(xSegments * xDiff / dimensions.x);
	int zSeg = (int)(ySegments * zDiff / dimensions.z);

	xPerc = (xDiff - xSeg * dimensions.x / xSegments)/(dimensions.x/xSegments);
	zPerc = (zDiff - zSeg * dimensions.z / xSegments)/(dimensions.z/ySegments);

	from = heights[ xSeg * (ySegments+1) + zSeg ];
	end = heights[ ( xSeg + 1 ) * (ySegments+1)  + zSeg + 1 ];

	if ( xPerc > zPerc )
	{
		mid = heights[ ( xSeg + 1 ) * (ySegments+1)  + zSeg ];
		result.y = from + xPerc * (mid-from) + zPerc * (end-mid);
	}
	else
	{
		mid = heights[ xSeg * (ySegments+1)  + zSeg + 1 ];
		result.y = from + zPerc * (mid-from) + xPerc * (end-mid);
	}

	result.y = position.y + dimensions.y * result.y - dimensions.y / 2.0f;

	return result;
}

Terrain::~Terrain()
{
	//delete heights;
}
