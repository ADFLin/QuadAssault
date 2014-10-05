uniform sampler2D texGeometry;
uniform sampler2D texLightmap;
uniform vec3 ambientLight;

void main()
{		
	vec4 amb=vec4(ambientLight,1.0);
	
	vec4 t1=texture2D(texGeometry, gl_TexCoord[0].st);
	vec4 t2=texture2D(texLightmap, gl_TexCoord[0].st);

	vec4 color = t1 * amb + 4.0 * t1 * t2;

	gl_FragColor = color;
}