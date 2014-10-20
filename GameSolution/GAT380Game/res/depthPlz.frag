#version 400

in vec2 vUv;

uniform int useTexture;
uniform int useTransMap;
uniform int useLighting;
uniform vec4 colorInfluence;

uniform sampler2D tex;
uniform sampler2D trans;

out vec4 color;

void main()
{
	color = texture2D( tex, vUv );
	float col = color.x;
	col = pow( col, 40 );
	color = vec4( 1-col, 1-col, 1-col, 1 );
	//color = vec4( 1, 1, 1, 1 );
}