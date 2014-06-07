#version 400

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec4 in_normal;
uniform vec4 light;
uniform float time;
uniform vec3 colorInfluence;
uniform mat4 world;
uniform mat4 modelViewProjection;
uniform int drawMode;

out vec3 vColor;
out vec4 vPosition;
out vec4 vWorldPosition;
out vec4 vNormal;

void main()
{
	mat4 wmvp = modelViewProjection*world;
	vColor = in_color + colorInfluence;
	
	mat4 worldNormal = mat4(mat3(world));
	//vColor *= clamp( abs(dot( normalize(world*in_position-light), normalize( worldNormal*in_normal ) ) ), 0, 1 );
	
	vWorldPosition = world*in_position;
	vNormal = worldNormal*in_normal;
	
	vec4 transformedPosition = world*in_position;
	
	//if( drawMode == 3 )
	{
		transformedPosition += vec4(0,sin(time + transformedPosition.x + transformedPosition.z/2) * 0.4,0,0);
	}
	
	vPosition = modelViewProjection*( transformedPosition );//+ vec4(0,tan(time + in_position.x + in_position.z/2) * 0.2,0,0) );
	gl_Position = vPosition;
}