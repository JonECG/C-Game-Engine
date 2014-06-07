#include "AiGameWindow.h"
#include <QtGui\qhboxlayout>
#include "DebugMenus.h"
#include "AiGameHandle.h"
#include "QtGui\qmenu.h"
#include "QtGui\qmenubar.h"
#include "Qt\qfiledialog.h"
#include "glm\gtx\transform.hpp"
#include <fstream>

AiGameWindow::AiGameWindow()
{
	handle = new AiGameHandle();

	setCentralWidget( handle );	
}