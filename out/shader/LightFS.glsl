uniform vec2  posLight;
uniform vec3  colorLight;

uniform float radius;
uniform float intensity;
uniform float angle;
uniform vec2  dir;
uniform int   isExplosion;

//uniform float frameHeight;
//uniform float scaleFactor;

uniform sampler2D texNormalMap;
uniform sampler2D texBaseColor;

varying vec2 position;

void main()
{		
	//vec2 pixel=gl_FragCoord.xy;	
	//pixel.y = frameHeight-pixel.y;
	//pixel = pixel / scaleFactor;
	//vec2 aux = posLight - scaleFactor * pixel;
	
	vec2 lightOffset = posLight - position;
	float d = length(lightOffset);

	if( d > radius )
		discard;

	vec3 normal = texture2D(texNormalMap, gl_TexCoord[0].st).rgb;

	normal=normal*2.0-1.0;
	normal=normalize(normal);
	
	vec3 lightDir = vec3(lightOffset.x / d , lightOffset.y / d , 0.0);
	
	float diffuseFactor;
	diffuseFactor = max(dot( lightDir , normal ), 0.0);	

	if(isExplosion != 0)
		diffuseFactor = 0.05 + 0.01 * diffuseFactor;

	if( texture2D( texNormalMap , gl_TexCoord[0].st).rgb == vec3(1.0, 1.0, 1.0) )
		diffuseFactor = 0.05;

	if( dot( lightDir.xy , dir)< angle * d / radius )
		discard;

	float opad = 10 * intensity / d;
	float decay = clamp((1.0 - d / radius), 0.0, 1.0);
	float c = decay * opad * diffuseFactor;

	gl_FragColor = vec4( c * colorLight , 1.0 );		
	
	
}