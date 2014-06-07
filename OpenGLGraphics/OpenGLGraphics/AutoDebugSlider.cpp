#include "AutoDebugSlider.h"
#include <iostream>

void AutoDebugSlider::updateValue()
{
	//std::cout << "update" << std::endl;
	(*target)[offset] = value();
}

AutoDebugSlider::AutoDebugSlider( float** target, int offset,
			float min, float max, 
			bool textOnLeft, float granularity) : DebugSlider( min, max, textOnLeft, granularity )
{
	this->target = target;
	this->offset = offset;

	this->setValue( (*target)[offset] );

	connect(this, SIGNAL(valueChanged(float)), 
		this, SLOT(updateValue()));
}