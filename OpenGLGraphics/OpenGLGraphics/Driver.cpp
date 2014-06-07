#include <Qt\qapplication.h>
#include "LightingTestWindow.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	LightingTestWindow widg;
	widg.show();
	widg.resize( 1280, 640 );

	return app.exec();
}