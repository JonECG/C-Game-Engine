#include "EngineTest.h"
#include <Qt\qapplication.h>

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	EngineTest widg;
	widg.show();
	widg.resize( 640, 640 );

	return app.exec();
}