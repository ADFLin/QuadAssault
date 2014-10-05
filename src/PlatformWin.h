#ifndef PlatformWin_h__
#define PlatformWin_h__

#include "PlatformBase.h"
#include "Win32Header.h"

#include "TVector2.h"
typedef TVector2< int > Vec2i;

class GameWindowWin;

#define USE_SFML_WIN 1

#if USE_SFML_WIN
#include "Dependence.h"
#endif



class PlatformWin
{
public:
	static int64           getTickCount();
	static GameWindowWin*  createWindow( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );
	static void            procSystemMsg();
};

class GameWindowWin
{

public:
	void setSystemListener( ISystemListener& listener ){ mListener = &listener; }

#if USE_SFML_WIN

#else
	Vec2i getSize(){ return mSize; }
#endif
	
	void  showCursor( bool bShow );
	void  close();
	void  display();
	bool  setActive( bool bActive );
	void  release(){ delete this; }

private:

	friend class PlatformWin;
	GameWindowWin();
	bool create( char const* title , Vec2i const& size , int colorBit , bool bFullScreen );

	ISystemListener* mListener;

#if USE_SFML_WIN
public:
	sf::RenderWindow mImpl;
#else

	static LRESULT CALLBACK DefaultProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	bool procMsg( UINT message, WPARAM wParam, LPARAM lParam );
	bool precMouseMsg( UINT message, WPARAM wParam, LPARAM lParam );

	unsigned mMouseState;
	HWND  mhWnd;
	HDC   mhDC;
	HGLRC mhRC;
	Vec2i mSize;
	int   mWidth;
	int   mHeight;
#endif

};


#endif // PlatformWin_h__
