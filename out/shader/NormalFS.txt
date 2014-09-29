uniform sampler2D tex;
uniform float rotacija;

void main()
{			
	float rotRad = rotacija / 180 * 3.14159265359;
	
	vec3 normal = texture2D(tex, gl_TexCoord[0].st).rgb;
	vec3 out_normal;
	
	out_normal.x = cos(rotRad) * normal.x - sin(rotRad) * normal.y;
	out_normal.y = sin(rotRad) * normal.x + cos(rotRad) * normal.y;
	out_normal.z = normal.z;	
	
	out_normal = normalize(out_normal);
	
	gl_FragColor = vec4(out_normal.xyz, 1.0);	
}