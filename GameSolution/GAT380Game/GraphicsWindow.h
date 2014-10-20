#include "Qt\qmainwindow.h"
#include "GraphicsHandle.h"

#pragma once
class GraphicsWindow : public QMainWindow
{
	GraphicsHandle *handle;

public:
	GraphicsWindow();
};
