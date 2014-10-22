uniform sampler2D texBaseColor;
uniform sampler2D texGlow;

void main()
{
	vec3 baseColor = texture2D( texBaseColor , gl_TexCoord[0].st ).rgb;
	vec3 glowColor = texture2D( texGlow , gl_TexCoord[0].st ).rgb;

	if ( glowColor == vec3(0,0,0) )
		discard;

	gl_FragColor = vec4( 4 * baseColor * glowColor , 1.0 );
}