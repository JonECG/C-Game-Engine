Core::RGB ColorHelper::lerp( const Core::RGB& color1, const Core::RGB& color2, float amount )
{
	return RGB( 
		GetRValue( color1 ) + (int)( GetRValue( color2 ) - GetRValue( color1 ) ) * amount,
		GetGValue( color1 ) + (int)( GetGValue( color2 ) - GetGValue( color1 ) ) * amount,
		GetBValue( color1 ) + (int)( GetBValue( color2 ) - GetBValue( color1 ) ) * amount );
}

Core::RGB ColorHelper::quad( const Core::RGB& color1, const Core::RGB& color2, const Core::RGB& color3, float amount )
{
	return lerp( lerp( color1, color2, amount ), lerp( color2, color3, amount ), amount );
}