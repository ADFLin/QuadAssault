varying vec2 position;

void main()
{	
	position = gl_Vertex.xy;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();	
}