#include "PlatformWin.h"

void PlatformWin::procSystemMsg()
{
	MSG msg;
	while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE ) )
	{
		// Process the message
		if ( msg.message == WM_QUIT )
			return;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int64 PlatformWin::getTickCount()
{
	return  ::GetTickCount();
}

GameWindowWin* PlatformWin::createWindow( char const* title , Vec2i const& size , int colorBit , bool bFullScreen )
{
	GameWindowWin* win = new GameWindowWin;
	if ( !win->create( title , size , colorBit , bFullScreen ) )
	{
		delete win;
		return NULL;
	}
	return win;
}

GameWindowWin::GameWindowWin()
{
#ifdef USE_SFML_WIN

#else
	mSize = Vec2i::Zero();
	mhWnd = NULL;
	mhDC  = NULL;
	mhRC  = NULL;

	mListener = NULL;
	mMouseState = 0;
#endif
}

#if !USE_SFML_WIN
LRESULT CALLBACK GameWindowWin::DefaultProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HDC hDC;
	switch (message)                  /* handle the messages */
	{
	case WM_CREATE:
		{
			CREATESTRUCT* ps = (CREATESTRUCT*)lParam;
			SetWindowLong( hWnd , GWL_USERDATA , (LONG)ps->lpCreateParams );
		}
		break;
	case WM_DESTROY:
		::PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
		break;

	}

	GameWindowWin* window = ( GameWindowWin* )GetWindowLong( hWnd , GWL_USERDATA );

	if ( !window->procMsg( message , wParam , lParam ) )
		return 0;

	return ::DefWindowProc( hWnd , message , wParam , lParam );
}

bool GameWindowWin::procMsg( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_PAINT:
		::BeginPaint( mhWnd , NULL );
		::EndPaint( mhWnd , NULL );
		return true;
	case WM_CLOSE:
		return false;
	case WM_MBUTTONDOWN : case WM_MBUTTONUP : case WM_MBUTTONDBLCLK :
	case WM_LBUTTONDOWN : case WM_LBUTTONUP : case WM_LBUTTONDBLCLK :
	case WM_RBUTTONDOWN : case WM_RBUTTONUP : case WM_RBUTTONDBLCLK :
	case WM_MOUSEMOVE:    case WM_MOUSEWHEEL:
		return precMouseMsg( message , wParam , lParam );

	case WM_KEYDOWN: case WM_KEYUP: 
		return mListener->onKey( (char)wParam , message == WM_KEYDOWN );
	case WM_CHAR:
		return mListener->onChar( (char)wParam );
	}
	return true;

}

bool GameWindowWin::precMouseMsg( UINT message, WPARAM wParam, LPARAM lParam )
{
	unsigned button = 0;

	switch ( message )
	{
#define CASE_MOUSE_MSG( BUTTON , WDOWN , WUP , WDCLICK )\
case WDOWN:\
	mMouseState |= BUTTON;\
	button = BUTTON | MBS_DOWN;\
	::SetCapture( mhWnd );\
	break;\
case WUP:\
	mMouseState &= ~BUTTON ;\
	button = BUTTON ;\
	::ReleaseCapture();\
	break;\
case WDCLICK:\
	button = BUTTON | MBS_DOUBLE_CLICK ;\
	break;

		CASE_MOUSE_MSG( MBS_MIDDLE , WM_MBUTTONDOWN , WM_MBUTTONUP , WM_MBUTTONDBLCLK )
		CASE_MOUSE_MSG( MBS_LEFT   , WM_LBUTTONDOWN , WM_LBUTTONUP , WM_LBUTTONDBLCLK )
		CASE_MOUSE_MSG( MBS_RIGHT  , WM_RBUTTONDOWN , WM_RBUTTONUP , WM_RBUTTONDBLCLK )
#undef CASE_MOUSE_MSG

case WM_MOUSEMOVE:
	button = MBS_MOVING;
	break;
case WM_MOUSEWHEEL:
	{
		int zDelta = HIWORD(wParam);
		if ( zDelta == WHEEL_DELTA )
			button = MBS_WHEEL;
		else
			button = MBS_WHEEL | MBS_DOWN;
	}
	break;
	}

	int x = (int) LOWORD(lParam);
	int y = (int) HIWORD(lParam);

	if (x >= 32767) x -= 65536;
	if (y >= 32767) y -= 65536;

	return mListener->onMouse( MouseMsg( x , y , button , mMouseState ) );
}
#endif

bool GameWindowWin::create( char const* title , Vec2i const& size , int colorBit , bool bFullScreen )
{
#if USE_SFML_WIN
	int style = sf::Style::Close;		
	mImpl.create(sf::VideoMode( size.x , size.y , colorBit), title, style);
	return mImpl.isOpen();

#else
	TCHAR const* FRAME_CLASS_NAME = TEXT("GameWindow");
	WNDCLASSEX  wc;

	HINSTANCE hInstance = ::GetModuleHandle( NULL );

	// Create the window class for the main window
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS /*| CS_OWNDC*/ | CS_CLASSDC;
	wc.lpfnWndProc    = DefaultProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = hInstance;
	wc.hIcon          = LoadIcon(hInstance,MAKEINTRESOURCE(0));
	wc.hIconSm        = LoadIcon(hInstance,MAKEINTRESOURCE(0));
	wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName   = NULL;
	wc.lpszClassName  = FRAME_CLASS_NAME;

	// Register the window class
	if (!RegisterClassEx(&wc))
		return false;

	DWORD winStyle ,exStyle;

	if ( bFullScreen )
	{
		DEVMODE dmScreenSettings;

		memset( &dmScreenSettings , 0 , sizeof( dmScreenSettings) );

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= size.x;
		dmScreenSettings.dmPelsHeight	= size.y;
		dmScreenSettings.dmBitsPerPel	= colorBit;
		dmScreenSettings.dmFields       = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if ( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL )
		{
			return false;
		}

		winStyle = WS_POPUP;
		exStyle  = WS_EX_APPWINDOW;
	}
	else
	{
		winStyle  = WS_MINIMIZEBOX | WS_VISIBLE | WS_CAPTION | WS_SYSMENU ;
		exStyle   = WS_EX_OVERLAPPEDWINDOW;
	}

	RECT rect;
	SetRect (&rect , 0, 0, size.x , size.y );
	AdjustWindowRectEx(&rect,winStyle, FALSE ,exStyle);

	mhWnd = CreateWindowEx(
		exStyle,
		FRAME_CLASS_NAME ,
		title ,
		winStyle ,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right -rect.left ,
		rect.bottom - rect.top ,
		NULL,
		NULL,
		hInstance ,
		LPVOID( this ) );


	if ( !mhWnd )
		return false;

	mhDC = GetDC( mhWnd );

	mSize = size;


	ShowWindow( mhWnd , SW_SHOWDEFAULT );
	UpdateWindow( mhWnd );

	PIXELFORMATDESCRIPTOR pfd;
	::ZeroMemory( &pfd , sizeof( pfd ) );


	pfd.nSize    = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pfd.cColorBits   = colorBit;
	pfd.cDepthBits   = 32;
	pfd.cStencilBits = 0;

	int pixelFmt = ::ChoosePixelFormat( mhDC , &pfd );

	if (  pixelFmt == -1 || !::SetPixelFormat( mhDC , pixelFmt , &pfd )  )
		return false;

	mhRC = ::wglCreateContext( mhDC );

	if ( mhDC == NULL )
		return false;

	if ( !::wglMakeCurrent( mhDC , mhRC )  )
		return false;


	return true;
#endif
}

void GameWindowWin::close()
{
#if USE_SFML_WIN
	mImpl.close();
#else
	if ( mhRC )
		::wglDeleteContext( mhRC );

	if ( mhDC )
		::ReleaseDC( mhWnd , mhDC );

	if ( mhWnd )
		::DestroyWindow( mhWnd );

	mhDC  = NULL;
	mhWnd = NULL;
	mhRC  = NULL;
#endif

}

void GameWindowWin::display()
{
#if USE_SFML_WIN
	mImpl.display();
#else
	::SwapBuffers( mhDC );
#endif
}

void GameWindowWin::showCursor( bool bShow )
{
#if USE_SFML_WIN
	mImpl.setMouseCursorVisible( bShow );
#else
	::ShowCursor( bShow );
#endif
}

bool GameWindowWin::setActive( bool bActive )
{

#if USE_SFML_WIN
	return mImpl.setActive( bActive );
#else
	if ( bActive )
	{
		if ( !::wglMakeCurrent( mhDC , mhRC ) )
			return false;
	}
	return true;
#endif

}
