#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_uv;

uniform mat4 world;
uniform mat4 mvp;

out vec4 vPosition;
out vec4 vColor;
out vec3 vNormal;
out vec2 vUv;
out vec3 vTangent;
out float vHandedness;

void main()
{
	gl_Position = mvp*world*in_position;
	vNormal = normalize( mat3(world)*in_normal );
	vTangent = normalize( mat3(world)*in_color.xyz );
	vColor = in_color;
	vUv = in_uv;
	vPosition = world*in_position;
	vHandedness = in_color.w;
}