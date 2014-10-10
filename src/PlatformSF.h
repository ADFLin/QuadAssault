#ifndef PlatformSF_h__
#define PlatformSF_h__

#include "PlatformBase.h"

#include <SFML/Window.hpp>

class GameWindowSF
{
public:
	
	~GameWindowSF();

	void  release(){ delete this; }
	Vec2i getSize();
	void  showCursor( bool bShow );
	void  close();
	void  setSystemListener( ISystemListener& listener ){ mListener = &listener; }
	void  procSystemMessage();

private:
	friend class PlatformSF;
	friend class GLContextSF;
	friend class PlatformWin;
	friend class RenderSystem;

	GameWindowSF();
	bool create( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );

	ISystemListener* mListener;
	unsigned mMouseState;
	sf::RenderWindow mImpl;
};


class GLContextSF
{
public:
	bool init( GameWindowSF& window , GLConfig& config );
	void release();
	bool setCurrent();
	void swapBuffers();

private:

public:
	sf::RenderWindow* mWindow;

};

typedef GameWindowSF GameWindow;
typedef GLContextSF  GLContext;

class PlatformSF
{
public:
	static int64           getTickCount();
	static bool            isKeyPressed( unsigned key );
	static bool            isButtonPressed( unsigned button );

	static GameWindow*   createWindow( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );
	static GLContext*    createGLContext( GameWindow& window , GLConfig& config );
};


#endif // PlatformSF_h__
