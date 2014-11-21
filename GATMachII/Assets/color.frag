#version 400

in vec4 vColor;

uniform sampler2D tex;

out vec4 color;

void main()
{
	color = vColor;
}