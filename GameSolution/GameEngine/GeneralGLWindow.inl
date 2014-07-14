GeneralGlWindow::BufferInfo::BufferInfo()
{
	freeptr = 0;
}

void GeneralGlWindow::BufferInfo::initialize()
{
	glGenBuffers(1,&id);

	glBindBuffer( GL_ARRAY_BUFFER, id );
	glBufferData( GL_ARRAY_BUFFER, SIZE, NULL, GL_STATIC_DRAW );
}

GeneralGlWindow::GeometryInfo::GeometryInfo( unsigned int vertexArrayID, unsigned int bufferId, unsigned int numIndices, unsigned int numVertices, unsigned int indexingMode, unsigned int bufferOffset ) 
{
	this->vertexArrayID = vertexArrayID;
	this->bufferId = bufferId;
	this->numVertices = numVertices;
	this->numIndices = numIndices;
	this->indexingMode = indexingMode;
	this->bufferOffset = bufferOffset;
	indices = nullptr;
	vertices = nullptr;
}

ushort* GeneralGlWindow::GeometryInfo::getIndices() 
{
	if( indices == nullptr )
	{
		indices = new ushort[numIndices];
		glBindVertexArray( vertexArrayID );
		glGetBufferSubData( GL_ARRAY_BUFFER, bufferOffset, sizeof( ushort ) * numIndices, indices );
	}
	return indices;
}

Neumont::Vertex* GeneralGlWindow::GeometryInfo::getVertices() 
{
	if( vertices == nullptr )
	{
		vertices = new Neumont::Vertex[numVertices];
		glBindVertexArray( vertexArrayID );
		glGetBufferSubData( GL_ELEMENT_ARRAY_BUFFER, bufferOffset + numIndices*sizeof( ushort ), sizeof( Neumont::Vertex ) * numVertices, vertices );
	}
	return vertices;
}

GeneralGlWindow::TextureInfo::TextureInfo( unsigned int texture )
{
	textureID = texture;
}

GeneralGlWindow::UniformInfo::UniformInfo( int locationIn, const float* dataIn, ParameterType typeIn )
{
	location = locationIn;
	data = dataIn;
	type = typeIn;
}

GeneralGlWindow::ShaderInfo::ShaderInfo()
{
	numUniformParameters = 0;
}

GeneralGlWindow::ShaderInfo::ShaderInfo( unsigned int program )
{
	shaderProgramID = program;
	numUniformParameters = 0;
}

GeneralGlWindow::Renderable::Renderable()
{
	numUniformParameters = 0;
	visible = true;
}

GeneralGlWindow::Renderable::Renderable( GeometryInfo* whatGeometry, glm::mat4 whereMatrix, ShaderInfo* howShader, TextureInfo* texture, TextureInfo* trans, TextureInfo* occlusion ) 
{ 
	numUniformParameters = 0;
	this->whatGeometry = whatGeometry;
	where = whereMatrix;
	this->howShader = howShader;
	this->texture = texture;
	this->trans = trans;
	this->occlusion = occlusion;
	visible = true;
}

template<class TVert>
GeneralGlWindow::GeometryInfo* GeneralGlWindow::addGeometry( const TVert* verts, uint numVerts, ushort* indices, uint numIndices, GLuint indexingMode)
{
	int vertsize = numVerts * sizeof( TVert );
	return addGeometry( (void*)verts, numVerts, vertsize, indices, numIndices, indexingMode );
}

template<class TVert>
GeneralGlWindow::GeometryInfo* GeneralGlWindow::addGeometry( const TVert* verts, uint numVerts, uint* indices, uint numIndices, GLuint indexingMode)
{
	int vertsize = numVerts * sizeof( TVert );
	return addGeometry( (void*)verts, numVerts, vertsize, indices, numIndices, indexingMode );
}