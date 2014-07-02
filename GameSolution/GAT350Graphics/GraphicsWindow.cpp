#include "GraphicsWindow.h"
#include <QtGui\qhboxlayout>
#include "DebugMenus.h"
#include "GraphicsHandle.h"
#include "QtGui\qmenu.h"
#include "QtGui\qmenubar.h"
#include "Qt\qfiledialog.h"
#include "glm\gtx\transform.hpp"
#include <fstream>

GraphicsWindow::GraphicsWindow()
{
	handle = new GraphicsHandle();

	setCentralWidget( handle );	
}