#include "Qt\qmainwindow.h"
#include "AiGameHandle.h"

#pragma once
class AiGameWindow : public QMainWindow
{
	AiGameHandle *handle;

public:
	AiGameWindow();
};

