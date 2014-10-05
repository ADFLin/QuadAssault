#include "RenderSystem.h"

#include <cassert>
#include "Dependence.h"

class CFont : public IFont
{
public:
	sf::Font mImpl;

	virtual void release()
	{
		delete this;
	}
};

IFont* IFont::loadFont( char const* path )
{
	CFont* font = new CFont;
	if ( !font->mImpl.loadFromFile( path ) )
	{
		delete font;
		return NULL;
	}
	return font;
}

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

#if !USE_SFML_WIN

class WindowRenderTarget : public sf::RenderTarget
{
public:
	void init()
	{
		initialize();
	}
	virtual sf::Vector2u getSize() const
	{
		Vec2i size = window->getSize();
		return sf::Vector2u( size.x , size.y );
	}

	virtual bool activate( bool active )
	{
		return window->setActive( active );
	}

	GameWindow* window;
};

#endif

RenderSystem* gSystem = NULL;
RenderSystem* getRenderSystem(){  return gSystem;  }
RenderSystem::RenderSystem()
{
	assert( gSystem == NULL );
	gSystem = this;
}

void RenderSystem::init( GameWindow& window )
{
#if USE_SFML_WIN
	mRenderTarget = &window.mImpl;
#else
	mRenderTarget = new WindowRenderTarget;
	mRenderTarget->window = &window;
	mRenderTarget->init();
#endif
}

void RenderSystem::drawText( IText* text , Vec2f const& pos , unsigned sideFlag )
{

	sf::Text& textImpl = static_cast< CText* >( text )->mImpl;

	Vec2f rPos = pos;

	if ( ( sideFlag & TEXT_SIDE_LEFT ) == 0 )
	{
		if ( sideFlag & TEXT_SIDE_RIGHT )
			rPos.x -= textImpl.getLocalBounds().width ;
		else
			rPos.x -= textImpl.getLocalBounds().width / 2 ;
	}

	if ( ( sideFlag & TEXT_SIDE_TOP ) == 0 )
	{
		if ( sideFlag & TEXT_SIDE_BOTTOM )
			rPos.y -= textImpl.getLocalBounds().height;
		else
			rPos.y -= textImpl.getLocalBounds().height / 2;
	}
	textImpl.setPosition( rPos.x , rPos.y ); 

	mRenderTarget->pushGLStates();
	mRenderTarget->draw( textImpl );
	mRenderTarget->popGLStates();
}

void RenderSystem::prevRender()
{

}

void RenderSystem::postRender()
{
#if USE_SFML_WIN
	mRenderTarget->display();
#else
	//::glFlush();
#endif
}
