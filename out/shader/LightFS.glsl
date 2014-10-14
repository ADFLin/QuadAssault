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

varying vec2 position;

void main()
{		
	//vec2 pixel=gl_FragCoord.xy;	
	//pixel.y = frameHeight-pixel.y;
	//pixel = pixel / scaleFactor;
	//vec2 aux = posLight - scaleFactor * pixel;
	
	vec2 aux = posLight - position;
	float d = length(aux);

	if( d > radius )
		discard;

	vec3 normal=texture2D(texNormalMap, gl_TexCoord[0].st).rgb;

	normal=normal*2.0-1.0;
	normal=normalize(normal);
	
	vec3 dd=vec3(aux.x, aux.y, 0.0);
	
	float opad= 1.0 /d*intensity*10;
	float shadow;
	if(isExplosion == 0)
		shadow = max(dot(normalize(dd), normal), 0.0);	
	else
		shadow = 0.05;

	float decay = clamp((1.0 - d / radius), 0.0, 1.0);

	if( texture2D( texNormalMap , gl_TexCoord[0].st).rgb == vec3(1.0, 1.0, 1.0) )
		shadow=0.05;

	if( dot(normalize(aux), dir)<angle*d/radius )
		shadow = 0;

	gl_FragColor = vec4(opad*shadow,opad*shadow,opad*shadow,1.0)*vec4(colorLight*decay,1.0);		
	
	
}