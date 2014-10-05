uniform sampler2D texGeometry;
uniform sampler2D texLightmap;
uniform vec3 ambientLight;

void main()
{		
	vec4 color =  texture2D(texLightmap, gl_TexCoord[0].st);
	gl_FragColor = color;
}