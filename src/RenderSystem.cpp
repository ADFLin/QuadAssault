#include "RenderSystem.h"


#include "Dependence.h"

#include <cassert>
#include <iostream>

#if USE_SFML_WINDOW

class CFont : public IFont
{
public:
	sf::Font mImpl;

	bool load( char const* path )
	{
		return mImpl.loadFromFile( path );
	}

	virtual void release()
	{
		delete this;
	}
};

class CText : public IText
{
public:
	CText()
	{

	}

	CText( IFont* font , int size , Color const& color )
		:mImpl( "" , static_cast< CFont* >( font )->mImpl , size )
	{
		mImpl.setColor( sf::Color( color.r , color.g , color.b ) );
	}
	sf::Text mImpl;

	Vec2f getBoundSize() const
	{
		sf::FloatRect rect = mImpl.getLocalBounds();
		return Vec2f( rect.width , rect.height );
	}
	virtual void setString( char const* str )
	{
		mImpl.setString( str );
	}
	virtual void setColor( Color const& color )
	{
		mImpl.setColor( sf::Color( color.r , color.g , color.b ) );
	}
	virtual void setFont( IFont* font )
	{
		mImpl.setFont( static_cast< CFont* >( font )->mImpl );
	}
	virtual void setCharSize( int size )
	{
		mImpl.setCharacterSize( size );
	}

	virtual void release()
	{
		delete this;
	}
};

#else //FIXME : FPS is low  :( , and decrease by Time

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

class CFont : public IFont
{
public:

	CFont()
	{
		mFace = NULL;
	}
	~CFont()
	{
		if ( mFace )
			FT_Done_Face( mFace );
	}
	bool load( char const* path )
	{
		if( FT_New_Face( sLibrary , path , 0, &mFace ) )
		{
			std::cerr << "Could not open font" << std::endl;
			return false;
		}
		return true;
	}

	struct GlyphInfo
	{
		int dx , dy;
		float tx , ty;
		int width , rows;
		int offset;

	};
	struct Data
	{
		Data()
		{

		}
		~Data()
		{
			glDeleteLists(list_base,128);
			glDeleteTextures(128,textures);
		}
		int      refcount;
		int      h;
		GlyphInfo info[128];
		GLuint    textures[128];
		GLuint    list_base;
	};


	void buildData( Data& data , int size )
	{
		data.h = size;
		//For some twisted reason, Freetype measures font size
		//in terms of 1/64ths of pixels.  Thus, to make a font
		//h pixels high, we need to request a size of h*64.
		//(h << 6 is just a prettier way of writting h*64)
		FT_Set_Char_Size( mFace, size << 6, size << 6, 96, 96);

		data.list_base = glGenLists(128);
		glGenTextures( 128, data.textures );

		//This is where we actually create each of the fonts display lists.
		for(unsigned char i=0;i<128;i++)
			make_dlist(mFace,i,data.list_base, data.textures , data.info[i]);

	}

	inline int next_p2 ( int a )
	{
		int rval=1;
		while(rval<a) rval<<=1;
		return rval;
	}

	void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base , GlyphInfo& info ) 
	{

		//The first thing we do is get FreeType to render our character
		//into a bitmap.  This actually requires a couple of FreeType commands:

		//Load the Glyph for our character.
		if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
			throw std::runtime_error("FT_Load_Glyph failed");

		//Move the face's glyph into a Glyph object.
		FT_Glyph glyph;
		if(FT_Get_Glyph( face->glyph, &glyph ))
			throw std::runtime_error("FT_Get_Glyph failed");

		//Convert the glyph to a bitmap.
		FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL , 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		//This reference will make accessing the bitmap easier
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		//Use our helper function to get the widths of
		//the bitmap data that we will need in order to create
		//our texture.
		int width = next_p2( bitmap.width );
		int height = next_p2( bitmap.rows );

		//Allocate memory for the texture data.
		GLubyte* expanded_data = new GLubyte[ 2 * width * height];

		//Here we fill in the data for the expanded bitmap.
		//Notice that we are using two channel bitmap (one for
		//luminocity and one for alpha), but we assign
		//both luminocity and alpha to the value that we
		//find in the FreeType bitmap. 
		//We use the ?: operator so that value which we use
		//will be 0 if we are in the padding zone, and whatever
		//is the the Freetype bitmap otherwise.
		for(int j=0; j <height;j++) 
		{
			for(int i=0; i < width; i++)
			{
				expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
					(i>=bitmap.width || j>=bitmap.rows) ?
					0 : bitmap.buffer[i + bitmap.width*j];
			}
		}


		//Now we just setup some texture paramaters.
		glBindTexture( GL_TEXTURE_2D, tex_base[ch]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		//Here we actually create the texture itself, notice
		//that we are using GL_LUMINANCE_ALPHA to indicate that
		//we are using 2 channel data.
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

		//With the texture created, we don't need to expanded data anymore
		delete [] expanded_data;


		info.dx = bitmap_glyph->left;
		info.dy = bitmap_glyph->top-bitmap.rows;
		info.tx = (float)bitmap.width / (float)width;
		info.ty = (float)bitmap.rows / (float)height;
		info.rows  = bitmap.rows;
		info.width = bitmap.width;
		info.offset = face->glyph->advance.x >> 6;
		//So now we can create the display list
		glNewList(list_base+ch,GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D,tex_base[ch]);

		//glPushMatrix();

		//first we need to move over a little so that
		//the character has the right amount of space
		//between it and the one before it.
		glTranslatef(bitmap_glyph->left,0,0);

		//Now we move down a little in the case that the
		//bitmap extends past the bottom of the line 
		//(this is only true for characters like 'g' or 'y'.
		glTranslatef(0,bitmap_glyph->top-bitmap.rows,0);

		//Now we need to account for the fact that many of
		//our textures are filled with empty padding space.
		//We figure what portion of the texture is used by 
		//the actual character and store that information in 
		//the x and y variables, then when we draw the
		//quad, we will only reference the parts of the texture
		//that we contain the character itself.
		float x=(float)bitmap.width / (float)width;
		float y=(float)bitmap.rows / (float)height;

		//Here we draw the texturemaped quads.
		//The bitmap that we got from FreeType was not 
		//oriented quite like we would like it to be,
		//so we need to link the texture to the quad
		//so that the result will be properly aligned.
		glBegin(GL_QUADS);
		glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
		glTexCoord2d(0,y); glVertex2f(0,0);
		glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
		glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
		glEnd();
		//glPopMatrix();
		glTranslatef(face->glyph->advance.x >> 6 ,0,0);


		//increment the raster position as if we were a bitmap font.
		//(only needed if you want to calculate text length)
		//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

		//Finnish the display list
		glEndList();

		FT_Done_Glyph(glyph);
	}


	virtual void release()
	{
		delete this;
	}

	static bool initilize()
	{
		if ( FT_Init_FreeType( &sLibrary ) ) 
		{
			std::cerr << "Could not init freetype library" << std::endl;
			return false;
		}
		return true;
	}

	static void cleanup()
	{
		if ( sLibrary )
			FT_Done_FreeType(sLibrary);
	}

	static FT_Library sLibrary;

	Data* getData( int size )
	{
		DataMap::iterator iter = mDataMap.find( size );
		if ( iter != mDataMap.end() )
		{
			return iter->second;
		}

		Data* data = new Data;
		buildData( *data , size );
		mDataMap.insert( std::make_pair( size , data ) );
		return data;
	}

	typedef std::map< int ,  Data* > DataMap;
	DataMap  mDataMap;
	FT_Face  mFace;
	
};

FT_Library CFont::sLibrary = NULL;

class CText : public IText
{
public:
	CText()
	{
		mFont = NULL;
		mData = NULL;
		mCharSize = 0;
	}

	CText( IFont* font , int size , Color const& color )
		:mColor( color )
		,mFont( static_cast< CFont* >( font ) )
		,mCharSize( size )
	{
		assert( size );
		if ( font )
			mData = mFont->getData( size );
	}

	typedef std::vector< char > Line;
	typedef std::vector< Line > LineVec;

	virtual Vec2f getBoundSize() const
	{
		sf::FloatRect rect = mImpl.getLocalBounds();
		return Vec2f( 0 , 0 );
	}
	virtual void setString( char const* str )
	{
		mStr = str;
		//Here is some code to split the text that we have been
		//given into a set of lines.  
		//This could be made much neater by using
		//a regular expression library such as the one avliable from
		//boost.org (I've only done it out by hand to avoid complicating
		//this tutorial with unnecessary library dependencies).
		char const* start_line = mStr.c_str();
		char const* c = mStr.c_str();
		for( ; *c ; ++c ) 
		{
			switch( *c )
			{
			case '\n':
				{
					mLines.push_back( std::vector< char >( start_line , c ) );
					start_line=c+1;
				}
				break;
			}
		}
		if(start_line) 
		{
			mLines.push_back( std::vector< char >( start_line , c ) );
		}
	}
	virtual void setColor( Color const& color )
	{
		mColor = color;
	}
	virtual void setFont( IFont* font )
	{
		mFont = static_cast< CFont* >( font );
		if ( mFont && mCharSize )
			mData = mFont->getData( mCharSize );
	}
	virtual void setCharSize( int size )
	{
		mCharSize = size;
		if ( mFont )
			mData = mFont->getData( mCharSize );
	}

	virtual void release()
	{
		delete this;
	}

	void pushScreenCoordinateMatrix() 
	{
		glPushAttrib(GL_TRANSFORM_BIT);
		GLint	viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
		glPopAttrib();
	}

	/// Pops the projection matrix without changing the current
	/// MatrixMode.
	void pop_projection_matrix() 
	{
		glPushAttrib(GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
	}

	void print( float x , float y )
	{
		if ( !mData )
			return;

		// We want a coordinate system where things coresponding to window pixels.
		pushScreenCoordinateMatrix();					

		GLuint font= mData->list_base;
		float h = mData->h / 0.63f;						//We make the height about 1.5* that of


		glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
		
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

		//glListBase(font);

		float modelview_matrix[16];	
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

		glColor4ub( mColor.r , mColor.g , mColor.b , mColor.a );

		//This is where the text display actually happens.
		//For each line of text we reset the modelview matrix
		//so that the line's text will start in the correct position.
		//Notice that we need to reset the matrix, rather than just translating
		//down by h. This is because when each character is
		//draw it modifies the current matrix so that the next character
		//will be drawn immediatly after it.  
		int size = mLines.size();
		for( int i = 0 ; i < size ; ++i ) 
		{
			Line& line = mLines[i];
			glPushMatrix();

			glLoadIdentity();
			glTranslatef(x,600-( y-h*i),0);
			glMultMatrixf(modelview_matrix);

			//  The commented out raster position stuff can be useful if you need to
			//  know the length of the text that you are creating.
			//  If you decide to use it make sure to also uncomment the glBitmap command
			//  in make_dlist().
			//	glRasterPos2f(0,0);
			for( int n = 0 ; n < line.size(); ++n )
			{
				char ch = line[n];

				CFont::GlyphInfo& info = mData->info[ ch ];
				glBindTexture(GL_TEXTURE_2D, mData->textures[ ch ]);

				glPushMatrix();

				//first we need to move over a little so that
				//the character has the right amount of space
				//between it and the one before it.
				//glTranslatef( info.dx , info.dy ,0);

				//Here we draw the texturemaped quads.
				//The bitmap that we got from FreeType was not 
				//oriented quite like we would like it to be,
				//so we need to link the texture to the quad
				//so that the result will be properly aligned.
				glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex2f(0,info.rows);
				glTexCoord2f(0,info.ty); glVertex2f(0,0);
				glTexCoord2f(info.tx,info.ty); glVertex2f(info.width,0);
				glTexCoord2f(info.tx,0); glVertex2f(info.width,info.rows);
				glEnd();
				glPopMatrix();
				glTranslatef( info.offset ,0,0);
			}
			//glCallLists(mLines[i].length(), GL_UNSIGNED_BYTE, mLines[i].c_str());
			//	float rpos[4];
			//	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
			//	float len=x-rpos[0];
			glPopMatrix();
		}


		glPopAttrib();		

		pop_projection_matrix();

	}
	CFont::Data* mData;
	CFont*       mFont;
	String       mStr;

	LineVec      mLines;
	Color        mColor;
	int          mCharSize;
};

#endif



IFont* IFont::loadFont( char const* path )
{
	CFont* font = new CFont;
	if ( !font->load( path ) )
	{
		delete font;
		return NULL;
	}
	return font;
}

IText* IText::create( IFont* font , int size , Color const& color )
{
	CText* text = new CText( font , size , color );
	return text;
}

IText* IText::create()
{
	CText* text = new CText();
	return text;
}


RenderSystem* gSystem = NULL;
RenderSystem* getRenderSystem(){  return gSystem;  }
RenderSystem::RenderSystem()
{
	assert( gSystem == NULL );
	gSystem = this;
}

bool RenderSystem::init( GameWindow& window )
{
	GLConfig config;
	config.colorBits = 32;
	mContext = Platform::createGLContext( window , config );

	if ( !mContext )
		return false;

	std::cout << "Initilize glew..." << std::endl;
	GLenum result = glewInit();
	if(result != GLEW_OK )
	{
		std::cerr << "ERROR: Impossible to initialize Glew. Your graphics card probably does not support Shader Model 2.0." << std::endl;
		return false;
	}

#if USE_SFML_WINDOW
	mRenderWindow = &window.mImpl;
#else

	mRenderWindow = &window;
	CFont::initilize();
#endif

	return true;
}

void RenderSystem::drawText( IText* text , Vec2f const& pos , unsigned sideFlag )
{

#if USE_SFML_WINDOW
	sf::Text& textImpl = static_cast< CText* >( text )->mImpl;

	Vec2f rPos = pos;

	if ( ( sideFlag & TEXT_SIDE_LEFT ) == 0 )
	{
		if ( sideFlag & TEXT_SIDE_RIGHT )
			rPos.x -= textImpl.getLocalBounds().width ;
		else
			rPos.x -= textImpl.getLocalBounds().width / 2 ;
	}

	rPos.y -= textImpl.getLocalBounds().top;
	if ( ( sideFlag & TEXT_SIDE_TOP ) == 0 )
	{
		if ( sideFlag & TEXT_SIDE_BOTTOM )
			rPos.y -= textImpl.getLocalBounds().height;
		else
			rPos.y -= textImpl.getLocalBounds().height / 2;
	}
	textImpl.setPosition( rPos.x , rPos.y ); 

	mRenderWindow->pushGLStates();
	mRenderWindow->draw( textImpl );
	mRenderWindow->popGLStates();
#else

	static_cast< CText* >( text )->print( pos.x , pos.y );


#endif
}

bool RenderSystem::prevRender()
{
	if ( !mContext->setCurrent() )
		return false;

	return true;
}

void RenderSystem::postRender()
{
	mContext->swapBuffers();
}

void RenderSystem::cleanup()
{
	mContext->release();
}
