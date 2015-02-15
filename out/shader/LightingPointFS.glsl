uniform sampler1D texMaterial;
uniform sampler2D texBaseColor;
uniform sampler2D texNormal;

varying vec2 position;

struct LightParam
{
	vec2  pos;
	vec3  color;
	float ambIntensity;
	float difIntensity;
	float speIntensity;
	float radius;
};

uniform LightParam gLight;

struct MaterialParam
{
	float ka;
	float kd;
	float ks;
	float power;
};

MaterialParam getMaterial( in float id )
{
	MaterialParam mat;
	vec4 c = texture1D( texMaterial , id );
	mat.ka = c.r;
	mat.kd = c.g;
	mat.ks = c.b;
	mat.power = c.a;
	return mat;
}

void main()
{
	vec3 N = texture2D( texNormal , gl_TexCoord[0].st ).rgb;
	float matId = texture2D( texNormal , gl_TexCoord[0].st ).a;
	vec3 baseColor = texture2D( texBaseColor , gl_TexCoord[0].st ).rgb;

	if ( N == vec3(0,0,0) )
		discard;

	vec2 offset = gLight.pos - position;
	float d = length( offset );

	if ( d > gLight.radius )
		discard;

	vec3 normal = normalize( 2.0 * N - 1.0 );
	vec3 lightDir = normalize( vec3( offset.x  , offset.y  , 50 ) );
	
	float difFactor;
	if ( N == vec3(1,1,1) )
		difFactor = 1.0;
	else
		difFactor = clamp( dot( lightDir , normal ) , 0.0 , 1.0 );

	vec3 hDir = reflect( lightDir , normal );

	float speFactor = clamp( dot( hDir , vec3(0,0,-1) ) , 0.0 , 1.0 );
	float decay = clamp(( 1.0 - d / gLight.radius ), 0.0, 1.0 ) / ( d + 1 );
	//float decay = clamp(( 1.0 - d / gLight.radius ), 0.0, 1.0 ) / 100;

	MaterialParam mat = getMaterial( matId );
	vec3 color = gLight.color * baseColor * ( ( gLight.ambIntensity * mat.ka + gLight.difIntensity * mat.kd * difFactor + gLight.speIntensity * mat.ks * pow( speFactor , mat.power ) ) * decay );

	gl_FragColor = vec4( color , 1.0 ) ;
	//gl_FragColor = vec4( texture1D( texMaterial , matId ).rgb , 1.0 ) ;
	//gl_FragColor = vec4( texture2D( texNormal , gl_TexCoord[0].st ).rgb , 1.0 );
	//gl_FragColor = vec4( baseColor , 1.0 );
}