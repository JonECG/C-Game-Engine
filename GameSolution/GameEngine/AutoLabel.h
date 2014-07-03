#pragma once
#include <QtGui\qwidget>
#include "Engine.h"
#include "Qt\qtimer.h"

class QLabel;

class ENGINE_SHARED AutoLabel : public QWidget
{
	Q_OBJECT

	QLabel* label;
	const void * data;
	bool isFloat;
	QTimer timer;

	private slots:
		void update();

	public:
		AutoLabel( const char * name, const char * const * data );
		AutoLabel( const char * name, float * data );
};

