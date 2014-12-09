#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_uv;

uniform mat4 world;
uniform mat4 mvp;
uniform sampler2D tex;
uniform float timey;

out vec4 vPosition;
out vec4 vColor;
out vec3 vNormal;
out vec2 vUv;
out vec2 vNewUv;
out vec3 vTangent;
out float vHandedness;

out mat4 vWorld;

void main()
{
	vec2 adjUv= in_uv;
	//vec2 adjUv = in_uv + vec2( timey, timey  );
	//adjUv = abs( mod( adjUv, vec2( 2, 2 ) ) - vec2( 1, 1 ) );
	
	float dist = 2 * max( 0, min( 0.5, 0.6-length( adjUv - vec2( 0.5, 0.5 ) ) ) );
	float xMult = 2, yMult = 3;
	float cutOff = 0.35 + sin(timey+adjUv.x*xMult+adjUv.y*yMult)*0.02;
	vec4 usePosition = in_position + vec4(in_normal.xyz,0)* pow( max( cutOff, texture2D( tex, adjUv ).x*dist) , 2 )/25;
	
	gl_Position = mvp*world*usePosition;
	vWorld = world;
	vNormal = normalize( mat3(world)*in_normal );
	vTangent = normalize( mat3(world)*in_color.xyz );
	vColor = in_color;
	vUv = in_uv;
	vNewUv = in_uv;
	vPosition = world*usePosition;
	vHandedness = in_color.w;
}