#version 400

uniform int useTexture;
uniform int useTransMap;
uniform int useLighting;
uniform vec4 colorInfluence;

uniform sampler2D tex;
uniform sampler2D trans;

in vec2 vUv;

out vec4 color;

void main()
{
	if( useTexture != 0 )
	{
		color = texture( tex, vUv );
	}
	else
	{
		color = vec4(1,1,1,1);
	}
}