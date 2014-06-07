#include "Hill.h"
#include "GLHelper.h"
#include "RandomHelper.h"
#include "glm\gtc\matrix_transform.hpp"

using glm::vec2;
using glm::vec3;

Model Hill::ring = Model(); 

Hill::Hill(void)
{
	position = vec3();
	time = 0;
	wave = 0;
}

void Hill::initialize()
{
	//float radius = 0.1f;
	int iterations = 12;
	int count = iterations * 3;
	Vertex * vertices = new Vertex[ count ];
	
	for( int i = 0; i < iterations; i++ )
	{
		vertices[ 3 * i ].position = glm::vec3( 0,0.2f,0 );
		vertices[ 3 * i ].color = glm::vec3( 1.0f, 1.0f, 0.0f );

		for( int j = 0; j < 2; j++ )
		{
			float angle = ((float)((i + j)%iterations))/(iterations) * 6.2831853f;
			vertices[ 3 * i + j + 1 ].position = glm::vec3( cos( angle ), 0, sin( angle ) );
			vertices[ 3 * i + j + 1 ].color = glm::vec3( 1.0f, 1.0f, 0.0f );
		}

		GLHelper::calcNormal( vertices[ 3 * i ], vertices[ 3 * i + 1 ], vertices[ 3 * i + 2 ] );

		/*vertices[ 0 ].position = glm::vec3( 1,0,1 );
		vertices[ 1 ].position = glm::vec3( 1,0,-1 );
		vertices[ 2 ].position = glm::vec3( -1,0,1 );*/
		/*for( int j = 0; j < 2; j++ )
		{
			
			float angleTwo = ((float)(i + (j+1)%2))/iterations * 6.28;

			vertices[ 3 * (i * 2 + j) ].position = glm::vec3( cos( angleOne ), j * 2 - 1, sin( angleOne ) );
			vertices[ 3 * (i * 2 + j) ].color = glm::vec3( 1.0f, 1.0f, 0.0f );

			vertices[ 3 * (i * 2 + j) + 1 ].position = glm::vec3( cos( angleOne ), j * 2 - 1, sin( angleOne ) );
			vertices[ 3 * (i * 2 + j) + 1 ].color = glm::vec3( 1.0f, 1.0f, 0.0f );

			float nextAngle = ((float)i + j - 1)/iterations;
			vertices[ 3 * (i * 2 + j) + 2 ].position = glm::vec3( cos( angleTwo ), ((j+1)%2) * 2 - 1, sin( angleTwo ) );
			vertices[ 3 * (i * 2 + j) + 2 ].color = glm::vec3( 1.0f, 1.0f, 0.0f );

			GLHelper::calcNormal( vertices[ 3 * (i * 2 + j) ], vertices[ 3 * (i * 2 + j) + 1 ], vertices[ 3 * (i * 2 + j) + 2 ] );
		}*/
	}

	ring = //Model( vertices, count );
	GLHelper::indexVertices( vertices, count, false );
}

void Hill::update( GLfloat dt, Terrain &terrain )
{
	time -= dt;

	float changeTime = 20;

	if ( time <= 0 )
	{
		position = terrain.calcRandomPosition();
		time += changeTime;
	}

	wave += dt;
	if ( wave > 3 )
		wave = 0;
}

void Hill::draw( GLuint shaderProgram )
{
	GLuint loc = glGetUniformLocation(shaderProgram, "world");

	int number = 3;

	for ( int i = 0; i < number; i++ )
	{
		if (loc != -1)
		{
			float off = (wave + 3.0f*i/number);
			if (off > 3)
				off -= 3;
			glm::mat4 transform = glm::translate( glm::mat4(), position + vec3(0,off,0) ) * glm::scale( glm::mat4(), vec3( time/4 ) );//glm::scale( vec3( getScale() ) );

			glUniformMatrix4fv( loc, 1, false, (float*)&transform );
		}
		ring.draw();
	}
}

vec3 Hill::getPosition()
{
	return position;
}