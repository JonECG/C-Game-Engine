#version 400

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;
in mat3 vTangentSpace;
in vec2 vUv;


uniform vec4 amblight;
uniform vec4 diffpos;
uniform vec4 difflight;
uniform vec4 specColor;
uniform float tightness;
uniform vec4 eye;
uniform float useLighting;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

uniform float time;
uniform vec4 colorInfluence;
uniform mat4 world;
uniform mat4 modelViewProjection;


out vec4 color;

void main()
{
	vec3 normalized = //normalize( vNormal );
		normalize( vTangentSpace * vec3(0,1,0) );
	
	vec3 eyeVector = normalize( vec3( eye - vPosition ) );
	vec3 lightVector = normalize( vec3( diffpos - vPosition ) );
	
	vec4 diffuse = difflight * clamp( dot( lightVector, normalized ), 0, 1 ) ;
	vec4 spec = specColor * pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), tightness );

	color = texture2D(colorTexture, vUv);
	if ( useLighting != 0 )
		color = color * vColor * ( amblight + diffuse ) + spec;
	color.w = 1;
}