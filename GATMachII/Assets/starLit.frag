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
		
		float factor = 20;
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
		starAmount *= pow( 2 * abs( divisor/2 - mod( usingUv.x*usingUv.y + sin(grabbedColor.x), divisor ) ) / divisor, 8 + 6*sin(timey + usingUv.x * 30) );
		starAmount *= pow( 2 * abs( divisor/2 - mod( usingUv.y + cos(grabbedColor.x), divisor ) ) / divisor, 8 + 6*sin(timey*.15 + usingUv.y * 80) );
		//starAmount *= pow( mod( vUv.y, 0.1 ) * 10, 10 );
		color.x += starAmount;
		color.y += starAmount;
		color.z += starAmount;
		
		color.x = max( 0, min( 1, color.x ) );
		color.y = max( 0, min( 1, color.y ) );
		color.z = max( 0, min( 1, color.z ) );
		
		//cloud
		vec4 daySkyBack = vec4( 0, 0.3f, 0.6, 1 );
		vec4 duskSkyBack = vec4( 0.9f, 0.5f, 0.2, 1 );
		vec4 twilightSkyBack = vec4( 0.2f, 0.1f, 0.25f, 1 );
		vec4 nightSkyBack = vec4( 0, 0.0f, 0.2, 1 );
		
		vec4 worldPosition = vec4(0,0,0,0);
		float skyAngle = -dot( normalize(vec3(diffpos-eye)), normalize(vec3(worldPosition-eye)) );
		float perspAngle = dot( normalize(vec3(diffpos-eye)), normalize(vec3(vPosition-eye)) );
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
		
		float clouding = max( 0, pow( (1.5* texture2D(tex, vec2( vUv.y, vUv.xx) ).x -0.5)/ 2 , 1.2f ) );
		
		vec4 skyColor = skyBack + ( cloud - skyBack ) * ( clouding * (1-rawrat) );
		
		color = skyColor + ( color - skyColor ) * pow( rawrat, 2 );
		
	}
	
	color.w = 1;
}