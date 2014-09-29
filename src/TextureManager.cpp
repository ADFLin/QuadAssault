#include "TextureManager.h"

#include "Texture.h"
#include "Dependence.h"
#include "DataPath.h"

#include <iostream>

Texture::Texture()
{
	//file = NULL;
	id   = 0;
}

Texture::Texture(char const* file, GLuint id)
{
	this->file=file;
	this->id=id;
}		

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D , id );
}

Texture::~Texture()
{
	if ( id )
		glDeleteTextures( 1 , &id );

}

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	cleanup();
}


void TextureManager::cleanup()
{
	for(int i=0; i<mTextures.size(); i++)
	{
		delete mTextures[i];
	}
	mTextures.clear();
}

Texture* TextureManager::getTexture(int i)
{
	return mTextures[i];
}

Texture* TextureManager::getTexture(char const* name)
{	
	for(int i=0; i<mTextures.size(); i++)
	{
		if( mTextures[i]->file == name )
		{
			return mTextures[i];
		}
	}
	return loadTexture(name);
}

void TextureManager::destroyTexture(int i)
{
	delete mTextures[i];
	mTextures.erase(mTextures.begin()+i);
}

void TextureManager::destroyTexture(char const* name)
{
	for(int i=0; i<mTextures.size(); i++)
	{
		if( mTextures[i]->file == name )
		{
			delete mTextures[i];
			mTextures.erase(mTextures.begin()+i);
			break;
		}
	}
}

Texture* TextureManager::loadTexture(char const* name)
{	
	GLuint id;
	sf::Image image;
	string path = TEXTURE_DIR;
	path += name;
	if( !image.loadFromFile( path.c_str() ))
	{
		MessageBox(NULL,TEXT("Greska kod ucitavanja textura."),TEXT("Error."),MB_OK);
		return NULL;
	}
		
	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D,id);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.GetWidth(), image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPtr());			
	glTexImage2D( GL_TEXTURE_2D, 0, 4, image.getSize().x , image.getSize().y , 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr() );

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	Texture* tex = new Texture(name,id);
	mTextures.push_back( tex );
	std::cout << "Textura loaded : " << name << std::endl;
	return tex;

}
