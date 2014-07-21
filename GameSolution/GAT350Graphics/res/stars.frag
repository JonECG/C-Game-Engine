#version 400

in vec4 vColor;
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

out vec4 color;

void main()
{
	if( useTexture != 0 )
	{
		//float dist = 2 * max( 0, min( 0.5, 0.6-length( vUv - vec2( 0.5, 0.5 ) ) ) );
		vec4 grabbedColor = texture2D(tex, vUv );
		
		color = vec4(0,0,0,0);
		
		float factor = 16;
		//Nebulae
		color.x = max( 0, (2* grabbedColor.x - 1)/ factor );
		color.z = max( 0, (2* -grabbedColor.x + 1)/ factor );
		
		color.x += max( 0, (2* texture2D(tex, -vUv ).x - 1)/ factor );
		color.y += max( 0, (2* -texture2D(tex, -vUv ).x + 1)/ factor );
		color.z += max( 0, (2* texture2D(tex, -vUv ).x - 1)/ factor );
		
		//Stars
		float divisor = grabbedColor.x + 0.25;
		vec2 usingUv = vec2( -vUv.y * 2, vUv.x/3 + 3 );
		float starAmount = 100 * max( max( 0, 0.01 - abs( mod( usingUv.x, divisor ) - divisor/2 ) )
		, max( 0, 0.01 - abs( mod( usingUv.y, divisor ) - divisor/2 ) ) );
		divisor = 0.0001;
		starAmount *= pow( 2 * abs( divisor/2 - mod( usingUv.x*usingUv.y + sin(grabbedColor.x), divisor ) ) / divisor, 5 );
		starAmount *= pow( 2 * abs( divisor/2 - mod( usingUv.y + cos(grabbedColor.x), divisor ) ) / divisor, 5 );
		//starAmount *= pow( mod( vUv.y, 0.1 ) * 10, 10 );
		color.x += starAmount;
		color.y += starAmount;
		color.z += starAmount;
		
		color.x = max( 0, min( 1, color.x ) );
		color.y = max( 0, min( 1, color.y ) );
		color.z = max( 0, min( 1, color.z ) );
		
		// vec4 water = vec4( 0, 0.2, 0.3, 1 );
		// vec4 waterFroth = vec4( 0.8, 0.9, 1.0, 1 );
		// vec4 sand = vec4( 1, 0.8, 0.4, 1 );
		// vec4 dirt = vec4( .5, 0.4, 0.2, 1 );
		// vec4 darkSand = vec4( 0.6, 0.4, 0.3, 1 );
		// vec4 grass = vec4( 0.4, 0.6, 0.1, 1 );
		// vec4 canopy = vec4( 0.9, 0.8, 0.9, 1 );
		
		// float xMult = 2, yMult = 3;
		// float cutOff = 0.35 + sin(timey+vUv.x*xMult+vUv.y*yMult)*0.02;
		// float points[] = float[8]( 0, 0.34 + sin(timey+vUv.x*xMult+vUv.y*yMult - 0.5)*0.02, 0.35 + sin(timey+vUv.x*xMult+vUv.y*yMult)*0.02, 0.355 - sin(timey+vUv.x*xMult+vUv.y*yMult + 3.1415/2)*0.015, 0.5, 0.65 ,0.8, 1 ); 
		// vec4 colorParts[] = vec4[7]( water, waterFroth, darkSand, sand, grass, dirt, canopy );

		// bool found = false;
		
		// for( int i = 0; i < 8 && !found; i++ )
		// {
			// if (points[i]>=color.r * dist)
			// {
				// color = colorParts[ min(i-1,6) ];
				// found = true;
			// }
		// }
		
	}
	
	// color.r = vUv.x;
	// color.g = vUv.y;
	// color.b = 0;
	//color = uvToSpherePos( vUv ).xyzz;
	color.w = 1;
}