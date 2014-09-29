#ifndef TextureManager_h__
#define TextureManager_h__

#include "Singleton.h"
#include <map>

class Texture;

class TextureManager
{
private:
	std::vector< Texture* > mTextures;
public:
	TextureManager();		
	~TextureManager();		

	Texture* getTexture(int i);			
	Texture* getTexture(char const* name);
	void     destroyTexture(int i);		
	void     destroyTexture(char const* name);
	Texture* loadTexture(char const* name);
	void     cleanup();
};

#endif // TextureManager_h__