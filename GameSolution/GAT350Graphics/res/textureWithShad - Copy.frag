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

uniform mat4 lightMvp;

out vec4 color;

void main()
{
	bool isLit = true;
	
	if( useTransMap != 0 )
	{
		//Do the weird shadow stuff
		vec4 pos = vec4( vPosition.xyz, 1 );
		pos = lightMvp * pos;
		pos /= pos.w;
		
		
		vec2 uvsm = ( pos.xy + vec2(1,1) ) /2;
		vec4 depthed = texture2D(trans, uvsm );
		// float col = (pos.z + 1) / 2;//color.x;
		// col = pow( col, 40 );
		// color = vec4( 1-col, 1-col, 1-col, 1 );
		
		if( uvsm.x > 0 && uvsm.x < 1 && uvsm.y > 0 && uvsm.y < 1 )
			isLit = ( (pos.z + 1) / 2 ) < (depthed.x + 0.0001);
		//color = pos.zzzz;//texture2D(trans, pos.xy );
	}
	
	if( useTexture != 0 )
	{
		color = texture2D(tex, vUv);
	}
	else
	{
		color = colorInfluence;
	}
	
	
	
	if( useLighting != 0 )
	{
		if( isLit )
		{
			vec3 normalized = normalize( vNormal );
			
			vec3 eyeVector = normalize( vec3( eye-vPosition ) );
			vec3 lightVector = normalize( vec3( diffpos - vPosition ) );
			
			vec4 diffuse = difflight * clamp( dot( lightVector, normalized ), 0, 1 ) ;
			vec4 spec = specColor * pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), tightness );

			color = color * ( amblight + diffuse ) + spec;
		}
		else
		{
			color = color * amblight;
		}
	}
	color.w = 1;
}