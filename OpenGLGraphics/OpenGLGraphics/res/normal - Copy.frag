#version 400

in vec4 vNormal;
in vec3 vColor;
in vec4 vPosition;
in vec4 vWorldPosition;

out vec3 color;

uniform mat4 world;
uniform vec4 light;
uniform int drawMode;


void main()
{
	//float mult = min( max( 1-vPosition.z/vPosition.w + 0.5, 0 ), 1 );
	vec3 diff = vec3( vWorldPosition-light );
	float dist = 100.0;
	float mult = clamp( abs(dot( normalize(diff), normalize( vec3( vNormal ) ) ) ), 0, 1 );
	//mult *= clamp( (dist - sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z) )/dist,  0, 1 );
	
	
	if( drawMode == 1 )
	{
		//Depth Buffer
		float colPart = 1 - pow( (vPosition.z/vPosition.w+1)/2, 20 );
		color = vec3(colPart,colPart,colPart);
	}
	else
	if( drawMode == 2 )
	{
		//View of Vertex Normals
		vec3 nNormal = normalize( vec3( vNormal ) );
		color = vec3( abs(nNormal.x), abs(nNormal.y), abs(nNormal.z) );
	}
	else
	if( drawMode == 3 )
	{
		//Cotton Candy
		color = (vec3( vWorldPosition ) + vec3( 15.0, 15.0f, 15.0f ) ) / 30.0f;
	}
	else
	{
		//Normal
		color =vColor*mult;
	}
}