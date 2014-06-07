#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_uv;
layout(location = 4) in vec3 in_binormal;
layout(location = 5) in vec3 in_tangent;

uniform vec4 amblight;
uniform vec4 diffpos;
uniform vec4 difflight;
uniform vec4 specColor;
uniform float tightness;
uniform vec4 eye;

uniform float time;
uniform vec4 colorInfluence;
uniform mat4 world;
uniform mat4 modelViewProjection;

out vec4 vColor;
out vec4 vPosition;
out vec3 vNormal;
out mat3 vTangentSpace;
out vec2 vUv;

void main()
{
	mat4 wmvp = modelViewProjection*world;
	
	mat3 w3 = mat3(world);
	vNormal = normalize( mat3(world)*in_normal );
	
	vTangentSpace = ( mat3( 
		normalize(w3*in_tangent),
		normalize(w3*in_binormal),
		normalize(w3*in_normal) ) );
	vColor = colorInfluence;

	gl_Position = wmvp*in_position;
	
	vUv = in_uv;
	vPosition = world*in_position;
}