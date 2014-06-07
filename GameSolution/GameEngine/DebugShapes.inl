
void DebugShapes::cullColor( Neumont::ShapeData *shape )
{
	for( int i = 0; i < shape->numVerts; i++ )
	{
		shape->verts[i].color = glm::vec4( 1, 1, 1, 1 );
	}
}

void DebugShapes::update( float dt )
{
	for( int i = 0; i < numberOfDebugs; i++ )
	{
		shapes[i].visible = (shapes[i].timeLeft == -1 || shapes[i].timeLeft >= 0);

		if (shapes[i].timeLeft != -1 )
			shapes[i].timeLeft -= dt;
	}
}


void DebugShapes::addCube( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color, float lifeTime, bool useDepth)
{
	addShape( cube, shad, transform, color, lifeTime, useDepth );
}


void DebugShapes::addSphere( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color, float lifeTime, bool useDepth)
{
	addShape( sphere, shad, transform, color, lifeTime, useDepth );
}


void DebugShapes::addPoint( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, float lifeTime, bool useDepth)
{
	addShape( crossHair, shad, transform, glm::vec4(1,1,1,1), lifeTime, useDepth );
}


void DebugShapes::addLine( GeneralGlWindow::ShaderInfo *shad, glm::vec3 from, glm::vec3 to, glm::vec4 color, float lifeTime, bool useDepth)
{
	glm::vec3 diff = to-from;
	addShape( line, shad, glm::translate( glm::mat4(), from ) * glm::mat4( glm::vec4( diff.x, diff.y, diff.z, 0 ), glm::vec4( 0, 1, 0, 0 ), glm::vec4( 0, 0, 1, 0 ), glm::vec4(0,0,0,1) ), color, lifeTime, useDepth );
}


void DebugShapes::addVector( GeneralGlWindow::ShaderInfo *shad, glm::vec3 tailPosition, glm::vec3 vec, glm::vec4 color, float lifeTime, bool useDepth)
{
	addShape( vector, shad, glm::translate( glm::mat4(), tailPosition ) * glm::mat4( glm::vec4( vec.x, vec.y, vec.z, 0 ), glm::vec4( 0, 1, 0, 0 ), glm::vec4( 0, 0, 1, 0 ), glm::vec4(0,0,0,1) ), color, lifeTime, useDepth );
}