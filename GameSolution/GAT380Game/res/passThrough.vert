#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_uv;

uniform mat4 world;
uniform mat4 mvp;

out vec2 vUv;

void main()
{
	gl_Position = mvp*world*in_position;
	vUv = in_uv;
}