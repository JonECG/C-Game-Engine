#pragma once
#include <QtGui\qwidget.h>

#include <QtGui\qlabel.h>
#include <QtGui\QHBoxLayout>
#include "AutoDebugSlider.h"
#include <QtGui\qslider.h>

#include <glm\glm.hpp>

#include <iostream>

class VectorEditWidget : public QWidget
{
	float* data;
	int size;

public:
	inline VectorEditWidget( char * name, float * input, float min, float max, float granularity = 100 )
	{
		data = input;
		size = 1;//sizeof( input ) / sizeof( float );

		setUp( name, min, max, granularity);
	}

	inline VectorEditWidget( char * name, glm::vec3 * input, float min, float max, float granularity = 100 )
	{
		data = (float*)input;
		size = 3;//sizeof( input ) / sizeof( float );

		setUp( name, min, max, granularity);
	}

	inline VectorEditWidget( char * name, glm::vec4 * input, float min, float max, float granularity = 100 )
	{
		data = (float*)input;
		size = 4;//sizeof( input ) / sizeof( float );

		setUp( name, min, max, granularity);
	}

	template <class Vect>
	inline VectorEditWidget( char * name, Vect * input, float min, float max, float granularity = 100 )
	{
		data = (float*)input;
		size = sizeof( Vect ) / sizeof( float );

		setUp( name, min, max, granularity);
	}

	inline void setUp( char * name, float min, float max, float granularity )
	{
		QVBoxLayout *mainLayout = new QVBoxLayout();
		setLayout(mainLayout);

		mainLayout -> addWidget( new QLabel(name) );

		QVBoxLayout *editLayout = new QVBoxLayout();
		for( int i = 0; i < size; i++ )
		{
			AutoDebugSlider * current = new AutoDebugSlider( &data, i, min, max, true, granularity );
			editLayout->addWidget( current );
		}

		mainLayout->addLayout(editLayout);
	}

	inline void setData( float* input )
	{
		data = input;
	}
};