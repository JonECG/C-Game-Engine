#include <Qt\qapplication.h>

#include "Lab01GuiBase.h"
#include "Lab02Gui.h"
#include "Lab03Gui.h"
#include "Lab04Gui.h"
#include "Lab05Gui.h"
#include "Lab06Gui.h"
#include "Lab07Gui.h"
#include "Lab08Gui.h"
#include "Lab09Gui.h"
#include "Lab09Gui.h"
#include "Lab10Gui.h"
#include "Lab11Gui.h"
#include "Lab12Gui.h"
#include "Lab13Gui.h"
#include "Lab14Gui.h"
#include "Lab15Gui.h"

#include "DebugMenus.h"

int main(int argc, char * argv[] )
{
	QApplication app( argc, argv );

	DebugMenus::menu = new DebugMenus();
	DebugMenus::menu -> setVisible( true );

	//Lab01GuiBase * currentLab = new Lab01GuiBase();
	//Lab02Gui * currentLab = new Lab02Gui();
	//Lab03Gui * currentLab = new Lab03Gui();
	//Lab04Gui * currentLab = new Lab04Gui();
	//Lab05Gui * currentLab = new Lab05Gui();
	//Lab06Gui * currentLab = new Lab06Gui();
	//Lab07Gui * currentLab = new Lab07Gui();
	//Lab08Gui * currentLab = new Lab08Gui();
	//Lab09Gui * currentLab = new Lab09Gui();
	//Lab10Gui * currentLab = new Lab10Gui();
	//Lab11Gui * currentLab = new Lab11Gui();
	//Lab12Gui * currentLab = new Lab12Gui();
	//Lab13Gui * currentLab = new Lab13Gui();
	//Lab14Gui * currentLab = new Lab14Gui();
	Lab15Gui * currentLab = new Lab15Gui();

	currentLab->initialize();
	currentLab->show();

	return app.exec();
}