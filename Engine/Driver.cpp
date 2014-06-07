#include "GameWidget.h"
#include <Qt\qapplication.h>

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	GameWidget widg;
	widg.show();
	widg.resize( 640, 640 );

	return app.exec();
}