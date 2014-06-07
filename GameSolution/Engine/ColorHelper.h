#ifndef COLORHELPER_H
#define COLORHELPER

#include "Core.h"

class ColorHelper
{
public:
	inline static Core::RGB lerp( const Core::RGB& color1, const Core::RGB& color2, float amount );
	inline static Core::RGB quad( const Core::RGB& color1, const Core::RGB& color2, const Core::RGB& color3, float amount );
};

#include "ColorHelper.inl"

#endif