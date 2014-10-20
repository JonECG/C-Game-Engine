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
	//color = vec4( 1, 1, 1, 1 );
}