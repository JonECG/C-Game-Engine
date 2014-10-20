#version 400

in vec4 vColor;
in vec2 vUv;
in vec3 vNormal;
in vec4 vPosition;

uniform vec4 amblight;
uniform vec4 diffpos;
uniform vec4 difflight;
uniform vec4 specColor;
uniform float tightness;
uniform vec4 eye;

uniform int useTexture;
uniform int useTransMap;
uniform int useLighting;
uniform vec4 colorInfluence;

uniform sampler2D tex;
uniform sampler2D trans;

out vec4 color;

void main()
{
	if( useTexture != 0 )
	{
		color = texture2D(tex, vUv);
	}
	else
	{
		color = colorInfluence;
	}
	
	if( useTransMap != 0 )
	{
		vec4 transparency = texture2D(trans, vUv);
		if( transparency.r < .3 )
			discard;
	}
	
	if( useLighting != 0 )
	{
		vec3 normalized = normalize( vNormal );
		
		vec3 eyeVector = normalize( vec3( eye-vPosition ) );
		vec3 lightVector = normalize( vec3( diffpos - vPosition ) );
		
		vec4 diffuse = difflight * clamp( dot( lightVector, normalized ), 0, 1 ) ;
		float bitty = pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), tightness );
		vec4 spec = specColor * bitty;

		color = color * ( amblight + diffuse ) + spec;
		color.w = 1 - clamp( dot( normalized, eyeVector ) - bitty, 0, 1 );
	}
	
}