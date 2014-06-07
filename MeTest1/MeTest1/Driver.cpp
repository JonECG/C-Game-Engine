#include <Qt\qapplication.h>
#include "OpenGLWindow.h"

int main( int argc, char * argv[] )
{
	QApplication app( argc, argv );
	OpenGLWindow widg;
	widg.show();

	return app.exec();
}