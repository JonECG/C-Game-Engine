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
uniform sampler2D opttex;

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

vec4 getSkyColor( vec3 position )
{
	float angleX = ( 1 - ( ( dot( vec3( 1, 0, 0 ), normalize( vec3( position.x, 0, position.z ) ) ) + 1 ) / 2 ) ) / 2;
		if( dot( normalize( vec3( position.x, 0, position.z ) ), vec3( 0, 0, 1 ) ) > 0 )
		{
			angleX = 1 - angleX;
		}
		
		float angleY = dot( normalize( vec3( position ) ), normalize( vec3( position.x, 0, position.z ) ) ) / 2;
		if( ( dot( normalize( vec3( position ) ), vec3( 0, 1, 0 ) ) > 0 ) )
		{
			angleY = 1 - angleY;
		}
		//vec3 normypos = normalize( vPosition );
	vec2 sampleUV = vec2( angleX, angleY );
	vec4 grabbedColor = texture2D(tex, sampleUV );
	
	vec4 resultColor = vec4(0,0,0,0);
	
	float factor = 20;
	//Nebulae
	resultColor.x = max( 0, (2* grabbedColor.x - 1)/ factor );
	resultColor.z = max( 0, (2* -grabbedColor.x + 1)/ factor );
	
	resultColor.x += max( 0, (2* texture2D(tex, -sampleUV ).x - 1)/ factor );
	resultColor.y += max( 0, (2* -texture2D(tex, -sampleUV ).x + 1)/ factor );
	resultColor.z += max( 0, (2* texture2D(tex, -sampleUV ).x - 1)/ factor );
	
	//Stars
	float divisor = grabbedColor.x + 0.25;
	vec2 usingUv = vec2( -sampleUV.y * 2, sampleUV.x/3 + 3 );
	float starAmount = 100 * max( max( 0, 0.01 - abs( mod( usingUv.x, divisor ) - divisor/2 ) )
	, max( 0, 0.01 - abs( mod( usingUv.y, divisor ) - divisor/2 ) ) );
	divisor = 0.0001;
	starAmount *= pow( 2 * abs( divisor/2 - mod( usingUv.x*usingUv.y + sin(grabbedColor.x), divisor ) ) / divisor, 8 + 6*sin(timey + usingUv.x * 30) );
	starAmount *= pow( 2 * abs( divisor/2 - mod( usingUv.y + cos(grabbedColor.x), divisor ) ) / divisor, 8 + 6*sin(timey*.15 + usingUv.y * 80) );
	//starAmount *= pow( mod( usingUV.y, 0.1 ) * 10, 10 );
	resultColor.x += starAmount;
	resultColor.y += starAmount;
	resultColor.z += starAmount;
	
	resultColor.x = max( 0, min( 1, resultColor.x ) );
	resultColor.y = max( 0, min( 1, resultColor.y ) );
	resultColor.z = max( 0, min( 1, resultColor.z ) );
	
	//cloud
	vec4 daySkyBack = vec4( 0, 0.3f, 0.6, 1 );
	vec4 duskSkyBack = vec4( 0.9f, 0.5f, 0.2, 1 );
	vec4 twilightSkyBack = vec4( 0.2f, 0.1f, 0.25f, 1 );
	vec4 nightSkyBack = vec4( 0, 0.0f, 0.2, 1 );
	
	vec4 worldPosition = vec4(0,0,0,0);
	float skyAngle = -dot( normalize(vec3(diffpos-eye)), normalize(vec3(worldPosition-eye)) );
	float perspAngle = dot( normalize(vec3(diffpos-eye)), normalize(vec3(position-vec3(eye))) );
	float tween = 5;
	float angle = (skyAngle * tween + perspAngle) / (tween+1);
	float range = 0.3f;
	
	float rawrat = 1 - angle;
	float rat = clamp( rawrat, 1-range, 1+range )-(1-range);
	rat *= 1 / (range*2);
	
	rawrat = abs( angle - 1 )/2;
	
	//1- pow( clamp( dot( normalize(diffpos-eye), normalize(vPosition-eye) ), 0 , 1 ), 2 );
	vec4 p1a = mix( daySkyBack, duskSkyBack, rat );
	vec4 p1b = mix( duskSkyBack, twilightSkyBack, rat );
	vec4 p1c = mix( twilightSkyBack, nightSkyBack, rat );
	
	vec4 p2a = mix( p1a, p1b, rat );
	vec4 p2b = mix( p1b, p1c, rat );
	
	vec4 skyBack = mix( p2a, p2b, rat );
	
	//vec4 skyBack = twilightSkyBack;
	vec4 cloud = vec4( 1, 1, 1, 1 );
	
	float clouding = max( 0, pow( (1.5* texture2D(tex, vec2( sampleUV.y, sampleUV.xx) ).x -0.5)/ 2 , 1.2f ) );
	
	vec4 skyColor = skyBack + ( cloud - skyBack ) * ( clouding * (1-rawrat) );
	
	resultColor = skyColor + ( resultColor - skyColor ) * pow( rawrat, 2 );
	
	//vec3 normPos = normalize( position );
	//return abs( vec4( normPos.x, normPos.y, normPos.z, 1 ) );
	return resultColor;
}

void main()
{
	vec3 newNormal = vNormal;
	vec4 fuckYouSpec = specColor;
	float usedSpec = tightness;
	
	
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
		newNormal = normalize( vNormal );
		//newNormal = findNearestNormal( vUv );
	}
	else
	{
		color = colorInfluence;
	}
	
	// if( useTransMap != 0 )
	// {
		// vec4 transparency = texture2D(trans, vUv);
		// if( transparency.r < .3 )
			// discard;
	// }
	
	if( useLighting != 0 )
	{
		vec3 normalized = normalize( newNormal );
		
		vec3 eyeVector = normalize( vec3( eye-vPosition ) );
		vec3 lightVector = normalize( vec3( diffpos - vPosition ) );
		
		vec4 diffuse = difflight * clamp( dot( lightVector, normalized ), 0, 1 ) ;
		vec4 spec = fuckYouSpec * pow( clamp( dot( - reflect( lightVector, vec3( normalized ) ), eyeVector ), 0, 1 ), usedSpec );

		color = color * ( amblight + diffuse ) + spec;
	}
	
	if( usedSpec < 10 )
	{
		vec3 normalized = normalize( newNormal );
		vec3 lightVector = normalize( vec3( vPosition - eye ) );
		vec3 reflected = reflect( lightVector, vec3( normalized ) );
		
		float rat = pow( clamp( 1 - abs( dot( lightVector, normalized ) ), 0, 1 ), 4 );
		color = color * ( 1 - rat ) + rat * getSkyColor( reflected * 10 );
	}
	// color.r = vUv.x;
	// color.g = vUv.y;
	// color.b = 0;
	//color = uvToSpherePos( vUv ).xyzz;
	color.w = 1;
}