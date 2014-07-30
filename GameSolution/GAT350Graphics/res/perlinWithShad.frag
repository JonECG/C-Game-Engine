#version 400

in vec4 vColor;
in vec2 vNewUv;
in vec2 vUv;
in vec3 vNormal;
in vec4 vPosition;

in mat4 vWorld;

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

uniform mat4 lightMvp;

out vec4 color;

vec3 uvToSpherePos( vec2 uv )
{
	float xMult = 2, yMult = 3;
	float cutOff = 0.35 + sin(timey+uv.x*xMult+uv.y*yMult)*0.02;

	vec3 normal = normalize( vec3( cos( ( uv.x - 0.5 ) * 6.28 ), sin( ( uv.x - 0.5 ) * 6.28 ), sin( ( -uv.y + .5  ) * 3.14 ) ) );
	//vec4 usePosition = in_position + vec4(in_normal.xyz,0)* pow( max( cutOff, texture2D( tex, in_uv ).x*dist) , 2 )/10;
	vec3 result = normal + ( normal
		* pow( max( cutOff, texture2D(tex, uv ).r * (2 * max( 0, min( 0.5, 0.6-length( uv - vec2( 0.5, 0.5 ) ) ) ) ) ), 2 ) )/10 ;
	
	return result;
}

vec3 findNearestNormal( vec2 uv )
{
	float conv = 0.005;
	vec3 thisSpot = uvToSpherePos( uv - vec2( conv, conv ));
	vec3 upSpot = uvToSpherePos( uv + vec2( 0, conv ) );
	vec3 rightSpot = uvToSpherePos( uv + vec2( conv, 0 ) );
	// vec3 thisSpot = vNormal * texture2D(tex, vUv ).r * (2 * max( 0, min( 0.5, 0.6-length( vUv - vec2( 0.5, 0.5 ) ) ) ) );
	// vec3 upSpot = (vNormal+vec3(0,0,0.1)) *texture2D(tex, vUv + vec2( 0, 0.001 ) ).r * (2 * max( 0, min( 0.5, 0.6-length( ( vUv + vec2( 0, 0.001 ) ) - vec2( 0.5, 0.5 ) ) ) ) );
	// vec3 rightSpot = (vNormal+vec3(0.1,0,0)) *texture2D(tex, vUv + vec2( 0.001, 0 ) ).r * (2 * max( 0, min( 0.5, 0.6-length( ( vUv + vec2( 0.001, 0 ) ) - vec2( 0.5, 0.5 ) ) ) ) );
	return normalize( mat3(vWorld) * cross( normalize( upSpot-thisSpot), normalize( rightSpot-thisSpot) ) );
}

void main()
{
	vec3 newNormal = vNormal;
	vec4 fuckYouSpec = specColor;
	float usedSpec = tightness;
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
		
		isLit = ( (pos.z + 1) / 2 ) < (depthed.x + 0.0001);
		//color = pos.zzzz;//texture2D(trans, pos.xy );
	}
	
	if( useTexture != 0 )
	{
		float dist = 2 * max( 0, min( 0.5, 0.6-length( vNewUv - vec2( 0.5, 0.5 ) ) ) );
		color = texture2D(tex, vNewUv );
		
		vec4 deepestWater = vec4( 0, 0.13, 0.22, 1 );
		vec4 deeperWater = vec4( 0, 0.14, 0.24, 1 );
		vec4 deepWater = vec4( 0, 0.17, 0.28, 1 );
		vec4 water = vec4( 0, 0.2, 0.3, 1 );
		vec4 waterFroth = vec4( 0.8, 0.9, 1.0, 1 );
		vec4 sand = vec4( 1, 0.8, 0.4, 1 );
		vec4 mountain = vec4( .5, 0.4, 0.2, 1 );
		vec4 mountainBase = vec4( .4, 0.4, 0.3, 1 );
		vec4 darkSand = vec4( 0.6, 0.4, 0.3, 1 );
		vec4 forest = vec4( 0.2, 0.45, 0.0, 1 );
		vec4 grass = vec4( 0.4, 0.6, 0.1, 1 );
		vec4 canopy = vec4( 0.9, 0.8, 1, 1 );
		
		float xMult = 2, yMult = 3;
		float cutOff = 0.35 + sin(timey+vNewUv.x*xMult+vNewUv.y*yMult)*0.02;
		float points[] = float[13]( 0, 0.05, 0.1, 0.25, 0.34 + sin(timey+vNewUv.x*xMult+vNewUv.y*yMult - 0.5)*0.02, 0.35 + sin(timey+vNewUv.x*xMult+vNewUv.y*yMult)*0.02, 0.355 - sin(timey+vNewUv.x*xMult+vNewUv.y*yMult + 3.1415/2)*0.015, 0.39, 0.47, 0.58 ,0.65, 0.78, 1 ); 
		vec4 colorParts[] = vec4[12]( deepestWater, deeperWater, deepWater, water, waterFroth, darkSand, sand, grass, forest, mountainBase, mountain, canopy );
		
		if (color.r*dist < cutOff )
		{
			usedSpec = 3;
		}
		else
		{
			fuckYouSpec = vec4( 0,0,0,1);
			usedSpec = 10000;
		}
		bool found = false;
		
		for( int i = 0; i < 12 && !found; i++ )
		{
			if (points[i]>=color.r * dist)
			{
				color = colorParts[ min(i-1,10) ];
				found = true;
			}
		}
		
		newNormal = vec3( 0, 0, 0 );
		if( isLit )
		{
			float subdivide = 2;
			int radial = 3;
			float conv = 0.001;
			for( int i = 0; i < radial; i++ )
			{
				for( int j = 1; j < subdivide; j++ )
				{
					float angle = 3.14 * j / subdivide;
					newNormal += findNearestNormal( (vUv) + vec2( cos( j / subdivide ), sin( j / subdivide ) ) * i * conv  );
				}
			}
			
			newNormal = normalize( newNormal );
			//newNormal = findNearestNormal( vUv );
		}
	}
	else
	{
		color = colorInfluence;
	}
	
	if( useLighting != 0 )
	{
		if( isLit )
		{
			vec3 normalized = normalize( newNormal );
			
			vec3 eyeVector = normalize( vec3( eye-vPosition ) );
			vec3 lightVector = normalize( vec3( diffpos - vPosition ) );
			
			vec4 diffuse = difflight * clamp( dot( lightVector, normalized ), 0, 1 ) ;
			vec4 spec = fuckYouSpec * pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), usedSpec );

			color = color * ( amblight + diffuse ) + spec;
		}
		else
		{
			color = color * amblight;
		}
	}
	// color.r = vUv.x;
	// color.g = vUv.y;
	// color.b = 0;
	//color = uvToSpherePos( vUv ).xyzz;
	color.w = 1;
}