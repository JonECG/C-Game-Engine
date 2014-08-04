#version 400

in vec4 vColor;
in vec3 vTangent;
in vec2 vUv;
in vec3 vNormal;
in vec4 vPosition;
in float vHandedness;

uniform vec4 amblight;
uniform vec4 diffpos;
uniform vec4 difflight;
uniform vec4 specColor;
uniform float tightness;
uniform float normalFade;
uniform vec4 eye;

uniform int useTexture;
uniform int useTransMap;
uniform int useLighting;
uniform int useAmbientOcclusion;
uniform vec4 colorInfluence;

uniform float pleaseColor;
uniform float pleaseNormal;
uniform float pleaseAmbient;

uniform sampler2D tex;
uniform sampler2D trans;
uniform sampler2D amo;

out vec4 color;

void main()
{
	if( useTexture != 0 )
	{
		color = texture2D(tex, vUv);
	}
	else
	{
		color = vColor;
	}
	
	if( pleaseColor == 0 )
	{
		color = vec4( 1, 1, 1, 1 );
	}
	
	//color = vColor;
	
	vec3 normalized = normalize( vNormal );
	vec3 normedTangent = normalize( vTangent );
	
	if( useTransMap != 0 && pleaseNormal != 0 )
	{
		vec3 newNormalized = normalize( vec3( texture2D(trans, vUv) ) * 2.0f - vec3( 1, 1, 1 ) );
		newNormalized.y = -newNormalized.y;
		
		mat3 transforming = mat3( normedTangent, -vHandedness * cross(normalized, normedTangent), normalized );
		newNormalized = transforming*newNormalized;
		
		normalized = normalize( ( normalized * normalFade + newNormalized * ( 1 - normalFade ) ) );
	}
	
	if( useLighting != 0 )
	{
		vec3 eyeVector = normalize( vec3( eye-vPosition ) );
		vec3 lightVector = normalize( vec3( diffpos - vPosition ) );
		
		vec4 diffuse = difflight * clamp( dot( lightVector, normalized ), 0, 1 ) ;
		vec4 spec = specColor * pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), tightness );

		color = color * ( amblight + diffuse ) + spec;
	}
	
	if( useAmbientOcclusion != 0 && pleaseAmbient != 0 )
	{
		color *= texture2D(amo, vUv);
	}
	color.w = 1;
}