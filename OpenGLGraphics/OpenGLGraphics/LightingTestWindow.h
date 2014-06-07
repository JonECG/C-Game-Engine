#pragma once
#include <QtGui\qwidget>
#include "AutoCheckBox.h"
#include <QtGui\qcheckbox.h>
#include <QtGui\QVboxlayout>
#include <QtGui\QHboxLayout>
#include "VectorEditWidget.h"
#include "OpenGlWindow.h"

class LightingTestWindow : public QWidget
{
public:
	inline LightingTestWindow()
	{
		QVBoxLayout *mainLayout = new QVBoxLayout();
		setLayout(mainLayout);

		OpenGlWindow * gl = new OpenGlWindow();

		QHBoxLayout * row1 = new QHBoxLayout();
		QHBoxLayout * row2 = new QHBoxLayout();
		QHBoxLayout * row3 = new QHBoxLayout();
		QHBoxLayout * row4 = new QHBoxLayout();

		row1->addWidget(new VectorEditWidget( "Object Color", &gl->objColor, 0, 1 ), 1);
		row1->addWidget(new VectorEditWidget( "Ambient Color", &gl->ambLight, 0, 1 ), 1);
		row2->addWidget(new VectorEditWidget( "Light Position", &gl->diffusePosition, -20, 20 ), 1);
		row2->addWidget(new VectorEditWidget( "Diffuse Color", &gl->diffuseColor, 0, 1 ), 1);
		row3->addWidget(new VectorEditWidget( "Specular Color", &gl->specColor, 0, 1 ), 1);
		row3->addWidget(new VectorEditWidget( "Specular Tightness", &gl->tightness, 1, 100 ), 1);
		row4->addWidget(new AutoCheckBox( "Use Lighting", &gl->useLighting ) );

		mainLayout->addLayout( row1 );
		mainLayout->addLayout( row2 );
		mainLayout->addLayout( row3 );
		mainLayout->addLayout( row4 );

		mainLayout->addWidget( gl, 15 );
	}

};