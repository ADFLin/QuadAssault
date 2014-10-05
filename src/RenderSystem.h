#ifndef RenderSystem_h__
#define RenderSystem_h__

#include "Platform.h"

#include "TVector2.h"
typedef TVector2< float > Vec2f;

struct Color
{
	Color(){}
	Color( uint8 r , uint8 g , uint8 b )
		:r(r),g(g),b(b){}
	uint8 r , g , b;
};

class WindowRenderTarget;

class IFont
{
public:
	virtual void release() = 0;
	static IFont* loadFont( char const* path );
};

class IText
{
public:
	virtual void setString( char const* str ) = 0;
	virtual void setColor( Color const& color ) = 0;
	virtual void setFont( IFont* font ) = 0;
	virtual void setCharSize( int size ) = 0;
	virtual void release() = 0;
	static IText* create( IFont* font , int size , Color const& color );
	static IText* create();
};

enum
{
	TEXT_SIDE_LEFT   = BIT(0),
	TEXT_SIDE_RIGHT  = BIT(1),
	TEXT_SIDE_TOP    = BIT(2),
	TEXT_SIDE_BOTTOM = BIT(3),
};

class RenderSystem
{
public:
	RenderSystem();
	void init( GameWindow& window );

	void prevRender();
	void postRender();
	void drawText(  IText* text , Vec2f const& pos ,unsigned sideFlag = 0 );

private:
#if USE_SFML_WIN
	sf::RenderWindow* mRenderTarget;
#else
	WindowRenderTarget* mRenderTarget;
#endif
};

RenderSystem* getRenderSystem();

#endif // RenderSystem_h__
