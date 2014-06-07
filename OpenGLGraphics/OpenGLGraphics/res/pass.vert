#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec4 in_normal;

uniform mat4 world;
uniform mat4 modelViewProjection;

out vec3 vColor;

void main()
{
	mat4 wmvp = modelViewProjection*world;
	vColor = in_color;
	gl_Position = wmvp * in_position;
}