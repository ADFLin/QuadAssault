#ifndef PlatformWin_h__
#define PlatformWin_h__

#include "PlatformBase.h"


#include "Win32Header.h"

class GameWindowWin;
class WGLContext;

#define USE_SFML_WINDOW  1

#if USE_SFML_WINDOW
#include "PlatformSF.h"
#endif


class GameWindowWin
{
public:
	~GameWindowWin();
	void  release(){ delete this; }
	Vec2i getSize(){ return mSize; }
	
	void  showCursor( bool bShow );
	void  close();
	void  setSystemListener( ISystemListener& listener ){ mListener = &listener; }
	void  procSystemMessage();
	HWND  getSystemHandle(){ return mhWnd; }

private:

	friend class PlatformWin;
	GameWindowWin();
	
	bool create( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );

	ISystemListener* mListener;

	unsigned mMouseState;

	static LRESULT CALLBACK DefaultProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	bool procMsg( UINT message, WPARAM wParam, LPARAM lParam );
	bool precMouseMsg( UINT message, WPARAM wParam, LPARAM lParam );

	
	HWND     mhWnd;
	Vec2i    mSize;
};

class WGLContext
{
public:
	WGLContext();
	~WGLContext();
	bool init( GameWindowWin& window , GLConfig& config );
	void release();
	bool setCurrent();
	void swapBuffers();

private:
	HDC   mhDC;
	HGLRC mhRC;
};

#if USE_SFML_WINDOW
typedef GameWindowSF GameWindow;
typedef GLContextSF  GLContext;
#else
typedef GameWindowWin GameWindow;
typedef WGLContext    GLContext;
#endif

class PlatformWin
{
public:
	static int64           getTickCount();
	static bool            isKeyPressed( unsigned key );
	static bool            isButtonPressed( unsigned button );

	static GameWindow*     createWindow( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );
	static GLContext*      createGLContext( GameWindow& window , GLConfig& config );
};


#endif // PlatformWin_h__
