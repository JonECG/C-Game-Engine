#pragma once

#include "DebugSlider.h"

class AutoDebugSlider : public DebugSlider
{
	Q_OBJECT

	float** target;
	int offset;

	private slots:
		void updateValue();

	public:
		AutoDebugSlider( float** target, int offset = 0,
			float min = -10.0f, float max = 10.0f, 
			bool textOnLeft = false, float granularity = 40.0);
		
};