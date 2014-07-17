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
uniform float timey;

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
		float dist = 2 * max( 0, min( 0.5, 0.6-length( vUv - vec2( 0.5, 0.5 ) ) ) );
		color = texture2D(tex, vUv );
		vec4 water = vec4( 0, 0.2, 0.3, 1 );
		vec4 waterFroth = vec4( 0.8, 0.9, 1.0, 1 );
		vec4 sand = vec4( 1, 0.8, 0.4, 1 );
		vec4 dirt = vec4( .5, 0.4, 0.2, 1 );
		vec4 darkSand = vec4( 0.6, 0.4, 0.3, 1 );
		vec4 grass = vec4( 0.4, 0.6, 0.1, 1 );
		vec4 canopy = vec4( 0.1, 0.3, 0.0, 1 );
		
		float points[] = float[8]( 0, 0.34 + sin(timey - 0.5)*0.02, 0.35 + sin(timey)*0.02, 0.355 - sin(timey + 3.1415/2)*0.015, 0.5, 0.65 ,0.8, 1 ); 
		vec4 colorParts[] = vec4[7]( water, waterFroth, darkSand, sand, grass, dirt, canopy );
		
		bool found = false;
		
		for( int i = 0; i < 8 && !found; i++ )
		{
			if (points[i]>=color.r * dist)
			{
				color = colorParts[ min(i-1,6) ];
				found = true;
			}
		}
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
		vec4 spec = specColor * pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), tightness );

		color = color * ( amblight + diffuse ) + spec;
	}
	color.w = 1;
}