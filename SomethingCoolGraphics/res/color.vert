#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_uv;

uniform mat4 world;
uniform mat4 mvp;
uniform vec4 color;

out vec4 vPosition;
out vec3 vNormal;

void main()
{
	gl_Position = mvp*world*in_position;
	vNormal = normalize( mat3(world)*in_normal );
	vPosition = world*in_position;
}