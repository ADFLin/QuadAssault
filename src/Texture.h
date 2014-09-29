#ifndef Texture_h__
#define Texture_h__

#include "Base.h"
#include "Dependence.h"

class Texture
{
public:
	Texture();
	Texture(char const* file, GLuint id);
	~Texture();

	void bind();
	string file;
	GLuint id;				
};

#endif // Texture_h__
