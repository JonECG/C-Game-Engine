#version 400
in layout(location=0) vec3 position;
in layout(location=1) vec4 color;

out vec4 passThroughColor;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position.xyz, 1);
	passThroughColor = color;
}
