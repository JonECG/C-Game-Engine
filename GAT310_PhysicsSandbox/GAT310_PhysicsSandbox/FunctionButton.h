#pragma once
#include <QtGui\qpushbutton.h>
#include "Qt\qobject.h"
#include "Engine.h"
#include "FunctionButton.h"
#include <functional>

class ENGINE_SHARED FunctionButton : public QPushButton
{
	Q_OBJECT

		std::function<void()> func;

	private slots:
		void runFunction();

	public:
		FunctionButton( const char * name, std::function<void()> func );
};

