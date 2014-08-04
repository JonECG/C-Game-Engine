#version 400

in vec3 vNormal;
in vec4 vPosition;

uniform sampler2D tex;

out vec4 color;

void main()
{
	color = vColor;
}