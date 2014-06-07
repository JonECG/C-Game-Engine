#include "LevelEditorHandle.h"
#include <ShapeGenerator.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Camera.h"
#include <QtGui\QMouseEvent>
#include "DebugShapes.h"
#include "DebugMenus.h"


GeneralGlWindow::Renderable * cubeRenderable;

Camera camera;
float tightness;
glm::vec3 shipPos;
float fps, delta;



bool selectingDestination;

glm::vec4 unselectedColor(0,0,1,1), sourceColor(1,0,0,1), destinationColor(0,1,0,1), connectionColor( 0.6, 0.6, 0.2, 1 ), directionIndicator( 0.9, 0.9, 0.1, 1 );

int currentlySelectedNode = -1;

void LevelEditorHandle::init()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	currentRenderable = 0;
	currentConnection = 0;
	levelRenderable = nullptr;

	textShad = addShaderInfo( "res/texture.vert", "res/texture.frag" );
	colorShad = addShaderInfo( "res/color.vert", "res/color.frag" );

	SPHERE_LENGTH = 0.1f;

	Neumont::ShapeData sphereData = Neumont::ShapeGenerator::makeSphere( 12 );
	sphereGeometry = addGeometry( sphereData.verts, sphereData.numVerts, sphereData.indices, sphereData.numIndices, GL_TRIANGLES );
	setUpAttribs( sphereGeometry );

	Neumont::ShapeData cubeData = Neumont::ShapeGenerator::makeCube();
	GeneralGlWindow::GeometryInfo *cubeGeo = addGeometry( cubeData.verts, cubeData.numVerts, cubeData.indices, cubeData.numIndices, GL_TRIANGLES );
	setUpAttribs( cubeGeo );

	cubeRenderable = addRenderable( cubeGeo, glm::mat4(), textShad, NULL );
	addUniformParameter( cubeRenderable, "colorInfluence",  GeneralGlWindow::ParameterType::PT_VEC4, (float*)&connectionColor );

	tightness = 40.0f;

	sphereRenderables = reinterpret_cast<GeneralGlWindow::Renderable**>( malloc( sizeof(GeneralGlWindow::Renderable*) * 1000 ) );
	positions = new glm::vec3[1000];

	camera = Camera( float(width())/height(), 0.1, 100 );
	camera.setFrom( glm::vec3( 0,8,4) );
	camera.setTo( glm::vec3(0,0,0) );

	levelTexture = addTexture( "res/metal.png" );
	sphereTexture = addTexture( "res/brick.png" );

	addUniformParameter( textShad, "mvp", PT_MAT4, (float*)&camera.mvp );

	
	setUniformParameter( textShad, "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );
	setUniformParameter( textShad, "diffpos", PT_VEC4, (float*)&glm::vec4(3.0f,6.0f,8.0f,1) );
	setUniformParameter( textShad, "difflight", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	setUniformParameter( textShad, "specColor", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	addUniformParameter( textShad, "tightness", PT_FLOAT, (float*)&(tightness) );
	addUniformParameter( textShad, "eye", PT_VEC4, (float*)&camera.from);
}

float angle = 0;

void LevelEditorHandle::paint()
{
	camera.setAspect( float(width())/height() );
	glViewport( 0, 0, width(), height() );
	glClearColor( 0.5, 0.1, 0.1, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if ( levelRenderable != nullptr )
		levelRenderable->draw();

	for( int i = 0; i < currentRenderable; i++ )
	{
		sphereRenderables[i]->draw();
	}

	for( int i = 0; i < currentConnection; i++ )
	{
		glm::vec3 from = positions[ connections[ i*2 ] ];
		glm::vec3 to = positions[ connections[ i*2 + 1 ] ];
		glm::vec3 diff = to-from;

		float totLength = glm::length( diff );
		float xzLength = std::sqrt( diff.x*diff.x + diff.z*diff.z );

		addUniformParameter( cubeRenderable, "colorInfluence",  GeneralGlWindow::ParameterType::PT_VEC4, (float*)&connectionColor );
		cubeRenderable->where = glm::translate( (to + from)/2.0f ) * glm::rotate( -90 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 0.0f, 0.0f, 1.0f ) * glm::scale( glm::length( ( to - from ) / 2.0f ), 0.01f, 0.01f );
		cubeRenderable->draw();

		addUniformParameter( cubeRenderable, "colorInfluence",  GeneralGlWindow::ParameterType::PT_VEC4, (float*)&directionIndicator );
		cubeRenderable->where = glm::translate( (to + from)/2.0f ) * glm::rotate( -90 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 0.0f, 0.0f, 1.0f ) * glm::scale( glm::length( ( to - from ) / 8.0f ), 0.02f, 0.02f ) * glm::translate( glm::vec3(3,0,0) );
		cubeRenderable->draw();
		//TODO: Draw a line from 'from' to 'to'
	}
}

glm::vec2 mousePosition = glm::vec2();
bool mouseTracking = false;

bool LevelEditorHandle::connectionExists( int from, int to )
{
	bool result = false;
	for( int i = 0; i < currentConnection; i++ )
	{
		if( connections[ i*2 ] == from && connections[ i*2 + 1 ] == to )
			result = true;
	}
	return result;
}

void LevelEditorHandle::mousePressEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{

		if( !mouseTracking )
		{
			mousePosition.x = mEvent->x()/float(width());
			mousePosition.y = mEvent->y()/float(height());
		}
		mouseTracking = true; 
	}

	if ( mEvent->button() == Qt::MouseButton::LeftButton && levelRenderable != nullptr )
	{
		//Change ray
		glm::vec3 camDir = glm::normalize(camera.to - camera.from);
		glm::vec3 rayPosition = camera.getFrom();

		glm::vec4 mousePosition = glm::vec4((2.0f * mEvent->x()) / width() - 1.0f, 1.0f - (2.0f * mEvent->y()) / height() ,-1,1);
		glm::vec4 mouseEye = glm::inverse( camera.calcProjection() ) * mousePosition;
		mouseEye.z = -1;
		mouseEye.w = 0;
		glm::vec3 rayDirection = glm::normalize( glm::vec3( glm::inverse( camera.calcModelView() ) * mouseEye ) );


		//Check for clicking nodes
		int nodeSelected = -1;
		for( int i = 0; i < currentRenderable; i++ )
		{
			glm::vec3 toNode = positions[i] - rayPosition;
			float toNodeLength = glm::length( toNode );
			float projectedLength = glm::dot( rayDirection, toNode );
			glm::vec3 projectedVector = projectedLength * rayDirection;
			glm::vec3 rejectedVector = toNode - projectedVector;
			float rejectedLength = glm::length( rejectedVector );


			std::cout << rejectedLength << std::endl;

			if ( rejectedLength < SPHERE_LENGTH )
			{
				if( selectingDestination && currentlySelectedNode != -1 && !connectionExists( currentlySelectedNode, i ) )
				{
					addUniformParameter( sphereRenderables[ i ], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&destinationColor );
					connections[ currentConnection*2 ] = currentlySelectedNode;
					connections[ currentConnection*2 + 1 ] = i;
					currentConnection++;
					nodeSelected = i;
				}
				else
				{
					for( int j = 0; j < currentRenderable; j++ )
					{
						addUniformParameter( sphereRenderables[j], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&unselectedColor );
					}
					nodeSelected = i;
					currentlySelectedNode = i;
					for( int j = 0; j < currentConnection; j++ )
					{
						if ( connections[j*2] == i )
						{
							addUniformParameter( sphereRenderables[connections[j*2 + 1]], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&destinationColor );
						}
					}
					addUniformParameter( sphereRenderables[ i ], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&sourceColor );
				}
			}
		}

		if ( nodeSelected != -1 )
		{
			//Node was clicked
		}
		else
		{
			//Check for level click
			Neumont::Vertex *vertices = levelRenderable->whatGeometry->getVertices();
			ushort *indices = levelRenderable->whatGeometry->getIndices();

			float bestTime = 80000000;
			glm::vec3 bestPosition = glm::vec3();

			for( int i = 0; i <levelRenderable->whatGeometry->numIndices; i += 3 )
			{
				//Each face
				glm::vec3 v1 = glm::vec3( levelRenderable->where * glm::vec4( vertices[indices[i]].position, 1 ) ), v2 = glm::vec3( levelRenderable->where * glm::vec4( vertices[indices[i+1]].position, 1 ) ), v3 = glm::vec3( levelRenderable->where * glm::vec4( vertices[indices[i+2]].position, 1 ) );
				glm::vec3 faceNormal = glm::cross( v2-v1, v3-v1 );

				for( int normalFlip = 0; normalFlip < 2; normalFlip++ )
				{
					float faceD = glm::dot( -v1, faceNormal );

					//Calculate intersection
					float t = -( faceD + glm::dot( faceNormal, rayPosition ) )/glm::dot( faceNormal, rayDirection );

					if ( glm::dot( faceNormal, rayDirection ) != 0 && t > 0 && t < bestTime )
					{
						//Check to see if point is within polygon
						int projectionAxisIndex = 0;
						float maxAxis = std::max( std::max( faceNormal.x, faceNormal.y ), faceNormal.z );
						for( int j = 0; j < 3; j++ )
						{
							if ( faceNormal[j] == maxAxis )
								projectionAxisIndex = j;
						}
						int inda = ( projectionAxisIndex + 1 ) % 3, indb = ( projectionAxisIndex + 2 ) % 3;
						//glm::vec3 u, v;
						glm::vec3 currentPosition = rayPosition + rayDirection*t;

						glm::vec3 u = glm::vec3( currentPosition[inda] - v1[inda], v2[inda] - v1[inda], v3[inda] - v1[inda] );
						glm::vec3 v = glm::vec3( currentPosition[indb] - v1[indb], v2[indb] - v1[indb], v3[indb] - v1[indb] );

						float alpha = (v.x*u.z-u.x*v.z)/(v.y*u.z-u.y*v.z);
						float beta = (v.y*u.x-u.y*v.x)/(v.y*u.z-u.y*v.z);

						if( alpha >= 0 && beta >= 0 && (alpha + beta) <= 1 )
						{
							bestTime = t;
							bestPosition = currentPosition;
						}
					}

					faceNormal *= -1;
				}
			}

			if (bestTime != 80000000)
			{
				//Add sphere
				std::cout<< currentRenderable;
				sphereRenderables[ currentRenderable ] = addRenderable( sphereGeometry, glm::translate( bestPosition ) * glm::scale( glm::vec3( SPHERE_LENGTH ) ), textShad, NULL );
				addUniformParameter( sphereRenderables[ currentRenderable ], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&unselectedColor );
				positions[ currentRenderable ] = bestPosition;
				currentRenderable++;
			}
		}
	}
}

void LevelEditorHandle::mouseReleaseEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{
		mouseTracking = false;
	}
}

void LevelEditorHandle::mouseMoveEvent( QMouseEvent * mEvent )
{
	if ( mouseTracking )
	{
		glm::vec2 currentPosition = glm::vec2( mEvent->x()/float(width()), mEvent->y()/float(height()));

		glm::vec2 delta = currentPosition-mousePosition;

		glm::vec3 camNorm = glm::normalize( camera.getTo()-camera.getFrom() );

		glm::vec3 rotUp = glm::cross( glm::vec3(0,1,0), camNorm );
		glm::vec3 rotRight = glm::cross( rotUp, camNorm );


		camera.setTo( camera.getFrom() + glm::rotate( glm::rotate( camNorm, delta.y*100, rotUp ), delta.x*100, rotRight ) );
		//camera.setTo( camera.getFrom() + glm::rotate( camNorm, delta.y*100, rotUp ) );
		//camera.setTo( glm::vec3( mousePosition.x, mousePosition.y, 0 ) );

		mousePosition.x = currentPosition.x;
		mousePosition.y = currentPosition.y;
	}
}

void LevelEditorHandle::update( float dt )
{
	static glm::vec2 mousePosition = glm::vec2();

	fps = 1/dt;
	delta = dt;

	glm::vec3 camNorm = glm::normalize( camera.getTo()-camera.getFrom() );
	glm::vec3 strafe = glm::normalize( -glm::cross( glm::vec3(0,1,0), camNorm ) );
	glm::vec3 up = glm::normalize( glm::cross( strafe, camNorm ) );

	glm::mat3 camSpace = glm::mat3( strafe, camNorm, up );

	glm::vec3 movement;
	/*if ( GetAsyncKeyState(VK_SHIFT) )
		movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), 0, (GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );
	else*/
		movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), (GetAsyncKeyState(VK_SPACE)?1:0) - (GetAsyncKeyState(VK_SHIFT)?1:0),(GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );

	glm::vec3 worldMove = camSpace*movement * 1.0f * dt;

	camera.setTo( camera.getTo() + worldMove);
	camera.setFrom( camera.getFrom() + worldMove);

	camera.calcModelViewProjection();

	selectingDestination = GetAsyncKeyState( VK_LCONTROL );

	static bool wasPressed = false;
	bool isPressed = GetAsyncKeyState( 'F' );

	if( isPressed && !wasPressed )
	{
		for( int i = 0; i < currentRenderable; i++ )
		{
			for( int j = 0; j < currentRenderable; j++ )
			{
				if( i!=j && !connectionExists( i, j ) )
				{
					glm::vec3 rayPosition = positions[i];
					glm::vec3 rayDirection = glm::normalize( positions[j] - positions[i] );
					float length = glm::length( positions[j] - positions[i] );

					//Check for level click
					Neumont::Vertex *vertices = levelRenderable->whatGeometry->getVertices();
					ushort *indices = levelRenderable->whatGeometry->getIndices();

					bool isIntersected = false;

					for( int i = 0; i <levelRenderable->whatGeometry->numIndices; i += 3 )
					{
						//Each face
						glm::vec3 v1 = glm::vec3( levelRenderable->where * glm::vec4( vertices[indices[i]].position, 1 ) ), v2 = glm::vec3( levelRenderable->where * glm::vec4( vertices[indices[i+1]].position, 1 ) ), v3 = glm::vec3( levelRenderable->where * glm::vec4( vertices[indices[i+2]].position, 1 ) );
						glm::vec3 faceNormal = glm::cross( v2-v1, v3-v1 );

						for( int normalFlip = 0; normalFlip < 2; normalFlip++ )
						{
							float faceD = glm::dot( -v1, faceNormal );

							//Calculate intersection
							float t = -( faceD + glm::dot( faceNormal, rayPosition ) )/glm::dot( faceNormal, rayDirection );

							if ( abs( glm::dot( faceNormal, rayDirection ) ) > 0.01 && t > 0.01 && !isIntersected && t < length )
							{
								//Check to see if point is within polygon
								int projectionAxisIndex = 0;
								float maxAxis = std::max( std::max( faceNormal.x, faceNormal.y ), faceNormal.z );
								for( int j = 0; j < 3; j++ )
								{
									if ( faceNormal[j] == maxAxis )
										projectionAxisIndex = j;
								}
								int inda = ( projectionAxisIndex + 1 ) % 3, indb = ( projectionAxisIndex + 2 ) % 3;
								//glm::vec3 u, v;
								glm::vec3 currentPosition = rayPosition + rayDirection*t;

								glm::vec3 u = glm::vec3( currentPosition[inda] - v1[inda], v2[inda] - v1[inda], v3[inda] - v1[inda] );
								glm::vec3 v = glm::vec3( currentPosition[indb] - v1[indb], v2[indb] - v1[indb], v3[indb] - v1[indb] );

								float alpha = (v.x*u.z-u.x*v.z)/(v.y*u.z-u.y*v.z);
								float beta = (v.y*u.x-u.y*v.x)/(v.y*u.z-u.y*v.z);

								if( alpha >= 0 && beta >= 0 && (alpha + beta) <= 1 )
								{
									isIntersected = true;
								}
							}

							faceNormal *= -1;
						}
					}

					if( !isIntersected )
					{
						connections[ currentConnection*2 ] = i;
						connections[ currentConnection*2 + 1 ] = j;
						currentConnection++;
					}

				}
			}
		}
	}

	wasPressed = isPressed;

	if( GetAsyncKeyState( VK_DELETE ) && currentlySelectedNode != -1 )
	{
		for( int i = 0; i < currentConnection; i++ )
		{
			if( connections[ i*2 ] == currentlySelectedNode || connections[ i*2 + 1 ] == currentlySelectedNode )
			{
				//Remove this connection
				currentConnection--;
				for( int j = i; j < currentConnection; j++ )
				{
					connections[ j*2 ] = connections[ (j+1)*2 ];
					connections[ j*2 + 1 ] = connections[ (j+1)*2 + 1 ];
				}
				i--;
			}
			else
			{
				if( connections[ i*2 ] > currentlySelectedNode )
					connections[ i*2 ]--;
				if( connections[ i*2 + 1 ] > currentlySelectedNode )
					connections[ i*2 + 1 ]--;
			}
		}
		currentRenderable--;
		for( int i = currentlySelectedNode; i < currentRenderable; i++ )
		{
			sphereRenderables[ i ] = sphereRenderables[ i + 1 ];
			positions[ i ] = positions[ i + 1 ];
		}
		currentlySelectedNode = -1;

		for( int j = 0; j < currentRenderable; j++ )
		{
			addUniformParameter( sphereRenderables[j], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&unselectedColor );
		}
	}

	//angle++;
	repaint();
}