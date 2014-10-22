uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texGlow;

uniform float matId = 0.0;
void main()
{
	gl_FragData[0] = texture2D(texDiffuse, gl_TexCoord[0].st );
	gl_FragData[1] = vec4( texture2D(texNormal , gl_TexCoord[0].st ).rgb , matId );
	gl_FragData[2] = texture2D(texGlow   , gl_TexCoord[0].st );	
}