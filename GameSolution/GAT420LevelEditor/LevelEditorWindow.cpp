#include "LevelEditorWindow.h"
#include <QtGui\qhboxlayout>
#include "DebugMenus.h"
#include "LevelEditorHandle.h"
#include "QtGui\qmenu.h"
#include "QtGui\qmenubar.h"
#include "Qt\qfiledialog.h"
#include "glm\gtx\transform.hpp"
#include <fstream>

LevelEditorWindow::LevelEditorWindow()
{
	//QHBoxLayout *mainLayout = new QHBoxLayout();
	//setLayout( mainLayout );

	/*DebugMenus::menu = new DebugMenus();
	DebugMenus::menu->setVisible( false );

	DebugMenus::inject( layout() );*/

	handle = new LevelEditorHandle();

	setCentralWidget( handle );


	QMenu* fileMenu = menuBar()->addMenu("File");

	QAction* action;
	fileMenu->addAction(action = new QAction("Load Wavefront (.obj) File", this));
	action->setShortcut(QKeySequence::New);
	connect(action, SIGNAL(triggered()), this, SLOT(loadObj()));

	fileMenu->addAction(action = new QAction("Save Level Binary", this));
	action->setShortcuts(QKeySequence::Save);
	connect(action, SIGNAL(triggered()), this, SLOT(saveNative()));

	fileMenu->addAction(action = new QAction("Save Level Binary File as..", this));
	action->setShortcuts(QKeySequence::SaveAs);
	connect(action, SIGNAL(triggered()), this, SLOT(saveNativeAs()));

	fileMenu->addAction(action = new QAction("Load Level Binary", this));
	action->setShortcuts(QKeySequence::Open);
	connect(action, SIGNAL(triggered()), this, SLOT(loadNative()));
}

void LevelEditorWindow::loadObj()
{
	QString *stringPointer = new QString;
	*stringPointer = QFileDialog::getOpenFileName(this, "Open OBJ", "./res/", "Wavefront Object Files (*.obj *.wfo)");
	{
		std::string targetObj = stringPointer->toUtf8().constData();
		if(targetObj != "")
		{
			std::string bString = std::string( "..\\..\\ModelOBJtoBinary\\ModelOBJtoBinary\\obj\\Debug\\ModelOBJtoBinary.exe \"" );
			//QString aString = QString( "..\\..\\ModelOBJtoBinary\\ModelOBJtoBinary\\obj\\Debug\\ModelOBJtoBinary.exe " );
			bString.append( targetObj );
			bString.append( "\"" );
			//aString += targetObj;
			//aString += QString( "\"" );

			const char *command = bString.c_str();
			std::cout << command << std::endl;
			system( command );

			std::string pathString = targetObj.substr( 0, targetObj.length() - 3 );
			pathString.append( "mod" );
			const char *path = pathString.c_str();
			std::cout << path << std::endl;
			GeneralGlWindow::GeometryInfo* levelGeo = handle->loadFile( path );
			handle->setUpAttribs( levelGeo );

			handle->levelRenderable = handle->addRenderable( levelGeo, glm::scale( glm::vec3( 0.4f ) ), handle->textShad, handle->levelTexture );
			
			handle->currentRenderable = 0;
			handle->currentConnection = 0;
		}
	}

	delete stringPointer;

	currentPath = nullptr;
}

#define WRITE_IN_REINTERPRET_SIZE( a ) reinterpret_cast<const char*>(&a), sizeof( a )
void LevelEditorWindow::saveFile( const char* path )
{
	std::ofstream write = std::ofstream( path, std::ios::binary | std::ios::out );

	GeneralGlWindow::GeometryInfo* geo = handle->levelRenderable->whatGeometry;
	//Header
	write.write( WRITE_IN_REINTERPRET_SIZE( geo->numVertices ) );
	write.write( WRITE_IN_REINTERPRET_SIZE( geo->numIndices ) );
	write.write( WRITE_IN_REINTERPRET_SIZE( handle->currentRenderable ) );
	write.write( WRITE_IN_REINTERPRET_SIZE( handle->currentConnection ) );

	//Level Data
	write.write( reinterpret_cast<const char*>( geo->getIndices() ), sizeof( *geo->getIndices() )*geo->numIndices );
	write.write( reinterpret_cast<const char*>( geo->getVertices() ), sizeof( *geo->getVertices() )*geo->numVertices );

	//Node Data
	write.write( reinterpret_cast<const char*>( handle->positions ), sizeof( *handle->positions ) * handle->currentRenderable );
	write.write( reinterpret_cast<const char*>( handle->connections ), sizeof( *handle->connections ) * handle->currentConnection*2 );

	int length = strlen( path );
	currentPath = new char[ length + 1 ];

	for( int i = 0; i <= length; i++ )
	{
		currentPath[i] = path[i];
	}

	write.close();
}

void LevelEditorWindow::saveNative()
{
	if( currentPath != nullptr )
		saveFile( currentPath );
	else
		saveNativeAs();
}



void LevelEditorWindow::saveNativeAs()
{
	QString destination = QFileDialog::getSaveFileName(this, "Save Level", "./res/", "Waymapped Levels (*.way)");
	if(destination != "")
	{
		saveFile( destination.toUtf8().constData() );
	}
}

glm::vec4 unselectColor(0,0,1,1);

void LevelEditorWindow::loadNative()
{
	QString destination = QFileDialog::getOpenFileName(this, "Load Level", "./res/", "Waymapped Levels (*.way)");
	if(destination != "")
	{
		std::ifstream file(destination.toUtf8().constData(), std::ios::in|std::ios::binary);

		char header[16];

		file.read( header, 16 );

		int vertexCount = *reinterpret_cast<int*>( header );
		int indexCount = *(reinterpret_cast<int*>( header ) + 1);
		int nodeCount = *(reinterpret_cast<int*>( header ) + 2);
		int connectionCount = *(reinterpret_cast<int*>( header ) + 3);


		int vertsSize = (vertexCount) * sizeof(Neumont::Vertex);
		int indexSize = indexCount * sizeof(ushort);

		char* data = new char[ vertsSize + indexSize ];

		file.read( data, vertsSize + indexSize );

		ushort* indices = reinterpret_cast<ushort*>(data);
		Neumont::Vertex* verts = reinterpret_cast<Neumont::Vertex*>(data+indexSize);
	
	

		GeneralGlWindow::GeometryInfo* result = handle->addGeometry( verts, vertexCount, indices, indexCount, GL_TRIANGLES );
		handle->setUpAttribs( result );
		delete [] data;

		handle->levelRenderable = handle->addRenderable( result, glm::scale( glm::vec3( 0.4f ) ), handle->textShad, handle->levelTexture );

		//I did it in 4 reads for the level EDITOR because I can just read in the data to different prexisting static arrays
		//file.read( reinterpret_cast<char *>(handle->positions), sizeof( *handle->positions ) * nodeCount );
		//file.read( reinterpret_cast<char *>(handle->connections), sizeof( *handle->connections ) * connectionCount * 2 );

		//But for game it would be easy for only 3 reads but it begins to look ugly in the editor
		
		//using 3 reads
		int nodeDataSize = sizeof( *handle->positions ) * nodeCount + sizeof( *handle->connections ) * connectionCount * 2;
		data = new char[ nodeDataSize ];
		file.read( data, nodeDataSize );
		memcpy( reinterpret_cast<char *>(handle->positions), data, sizeof( *handle->positions ) * nodeCount  );
		memcpy( reinterpret_cast<char *>(handle->connections), data + sizeof( *handle->positions ) * nodeCount, sizeof( *handle->connections ) * connectionCount * 2  );
		

		handle->currentRenderable = nodeCount;
		handle->currentConnection = connectionCount;

		for( int i = 0; i < nodeCount; i++ )
		{
			handle->sphereRenderables[ i ] = handle->addRenderable( handle->sphereGeometry, glm::translate( handle->positions[i] ) * glm::scale( glm::vec3( handle->SPHERE_LENGTH ) ), handle->textShad, NULL );
			handle->addUniformParameter( handle->sphereRenderables[ i ], "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)&unselectColor );
		}

		file.close();
	}
	
}