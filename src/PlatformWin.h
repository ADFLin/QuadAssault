#ifndef PlatformWin_h__
#define PlatformWin_h__

#include "PlatformBase.h"
#include "Win32Header.h"

#include "TVector2.h"
typedef TVector2< int > Vec2i;

class GameWindowWin;
class WGLContext;

#define USE_SFML_WINDOW  1

#if USE_SFML_WINDOW
#include "Dependence.h"
#endif


class PlatformWin
{
public:
	static int64           getTickCount();
	static bool            isKeyPressed( unsigned key );
	static bool            isButtonPressed( unsigned button );

	static GameWindowWin*  createWindow( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );
	static WGLContext*     createGLContext( GameWindowWin& window , GLConfig& config );
};

class GameWindowWin
{
public:
	~GameWindowWin();

	void  release(){ delete this; }

#if USE_SFML_WINDOW

#else
	Vec2i getSize(){ return mSize; }
#endif
	
	void  showCursor( bool bShow );
	void  close();
	

	void  setSystemListener( ISystemListener& listener ){ mListener = &listener; }
	void  procSystemMessage();
#if USE_SFML_WINDOW

#else
	HWND  getSystemHandle(){ return mhWnd; }
#endif

private:

	friend class PlatformWin;
	GameWindowWin();
	
	bool create( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );

	ISystemListener* mListener;

	unsigned mMouseState;
#if USE_SFML_WINDOW
public:
	sf::RenderWindow mImpl;

#else


	static LRESULT CALLBACK DefaultProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	bool procMsg( UINT message, WPARAM wParam, LPARAM lParam );
	bool precMouseMsg( UINT message, WPARAM wParam, LPARAM lParam );

	
	HWND     mhWnd;
	Vec2i    mSize;


	HDC   mhDC;
	HGLRC mhRC;
#endif

};

class WGLContext
{
public:
	bool init( GameWindowWin& window , GLConfig& config );
	void release();
	bool setCurrent();
	void swapBuffers();

private:
#if USE_SFML_WINDOW
public:
	sf::RenderWindow* mWindow;
#else
	HDC   mhDC;
	HGLRC mhRC;
#endif
};


#endif // PlatformWin_h__
