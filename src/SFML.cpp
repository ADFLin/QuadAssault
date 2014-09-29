#if 0
#include "SFML.h"

#include <olectl.h>

#include <cassert>

namespace sf
{
	
	LRESULT CALLBACK DefultProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		HDC hDC;
		switch (message)                  /* handle the messages */
		{
		case WM_CREATE:
			{
				CREATESTRUCT* ps = (CREATESTRUCT*)lParam;
				//SetWindowLong( hWnd , GWL_USERDATA , (LONG)ps->lpCreateParams );
			}
			break;
		case WM_DESTROY:
			::PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
			break;
		}

		//RenderWindow* window = ( RenderWindow* )GetWindowLong( hWnd , GWL_USERDATA );
		//window->ProcMsg( message , wParam , lParam );

		return ::DefWindowProc (hWnd, message, wParam, lParam);
	}

	RenderWindow::RenderWindow()
	{
		mhWnd = NULL;
		mhDC = NULL;
		mhRC = NULL;
		mWidth = mHeight = 0;
	}

	bool RenderWindow::Create( VideoMode const& mode , char const* title , int style )
	{
		TCHAR const* FRAME_CLASS_NAME = TEXT("SFMLFrame");
		WNDCLASSEX  wc;

		HINSTANCE hInstance = ::GetModuleHandle( NULL );

		// Create the window class for the main window
		wc.cbSize         = sizeof(WNDCLASSEX);
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS /*| CS_OWNDC*/ | CS_CLASSDC;
		wc.lpfnWndProc    = DefultProc;
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

		if ( style == Style::FullScreen )
		{
			DEVMODE dmScreenSettings;

			memset( &dmScreenSettings , 0 , sizeof( dmScreenSettings) );

			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth	= mode.Width;
			dmScreenSettings.dmPelsHeight	= mode.Height;
			dmScreenSettings.dmBitsPerPel	= mode.ColorBit;
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
		SetRect (&rect , 0, 0, mode.Width , mode.Height );
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

		mWidth  = mode.Width;
		mHeight = mode.Height;


		ShowWindow( mhWnd , SW_SHOWDEFAULT );
		UpdateWindow( mhWnd );

		PIXELFORMATDESCRIPTOR pfd;
		::ZeroMemory( &pfd , sizeof( pfd ) );


		pfd.nSize    = sizeof( pfd );
		pfd.nVersion = 1;
		pfd.dwFlags  = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;

		pfd.cColorBits   = mode.ColorBit;
		pfd.cDepthBits   = 32;
		pfd.cStencilBits = 0;

		int pixelFmt = ::ChoosePixelFormat( mhDC , &pfd );

		if (  pixelFmt == -1 || !::SetPixelFormat( mhDC , pixelFmt , &pfd )  )
			return false;

		mhRC = ::wglCreateContext( mhDC );

		if ( mhRC == NULL )
			return false;

		if ( !::wglMakeCurrent( mhDC , mhRC )  )
			return false;


		return true;
	}

	void RenderWindow::Close()
	{
		if ( mhRC )
			::wglDeleteContext( mhRC );

		if ( mhDC )
			::ReleaseDC( mhWnd , mhDC );

		if ( mhWnd )
			::DestroyWindow( mhWnd );

		mhDC  = NULL;
		mhWnd = NULL;
		mhRC  = NULL;
		mWidth = mHeight = 0;
	}

	void RenderWindow::Display()
	{
		glFlush();
		::SwapBuffers( mhDC );
	}

	void RenderWindow::ShowMouseCursor( bool beS )
	{
		::ShowCursor( beS );
	}

	bool RenderWindow::PollEvent( Event& event )
	{
		mEvent = &event;

		MSG msg;
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			// Process the message
			if ( msg.message == WM_QUIT)
				return false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return mEvent != NULL;
	}

	void RenderWindow::ProcMsg( UINT message, WPARAM wParam, LPARAM lParam )
	{
		assert( mEvent );
		switch( message )
		{
		case WM_KEYDOWN:
			mEvent->Type = Event::KeyPressed;
			mEvent->Key.Code = Keyboard::Key::Value( wParam );
			break;
		case WM_KEYUP:
			mEvent->Type = Event::KeyReleased;
			mEvent->Key.Code = Keyboard::Key::Value( wParam );
			break;
		case WM_LBUTTONDOWN:

		default:
			mEvent = NULL;
		}
	}


	Image::Image()
	{
		mhDC = NULL;
		mPixelPtr = NULL;
		mWidth = mHeight = 0;
	}

	bool Image::loadFromFile( char const* path )
	{
		bool result = false;

		mhDC = CreateCompatibleDC(GetDC(0));						    // Create The Windows Compatible Device Context
		if( !mhDC )														// Did Creation Fail?
			return false;												// Return False (Failure)

		char  szPath[MAX_PATH+1];										// Full Path To Picture
		if (strstr(path, "http://"))									// If PathName Contains http:// Then...
		{
			strcpy(szPath, path );										// Append The PathName To szPath
		}
		else																// Otherwise... We Are Loading From A File
		{
			GetCurrentDirectory(MAX_PATH, szPath);							// Get Our Working Directory
			strcat(szPath, "\\");											// Append "\" After The Working Directory
			strcat(szPath, path);										// Append The PathName
		}
		OLECHAR		wszPath[MAX_PATH+1];									// Full Path To Picture (WCHAR)
		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// Convert From ASCII To Unicode

		IPicture *pPicture = NULL;												// IPicture Interface
		HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

		if(FAILED(hr))														// If Loading Failed
			return false;													// Return False


		long		lWidth , lHeight;			                        // Width , Height  In Logical Units										
		pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
		pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
		long lWidthPixels = MulDiv(lWidth, GetDeviceCaps(mhDC, LOGPIXELSX), 2540); // Width In Pixels
		long lHeightPixels = MulDiv(lHeight, GetDeviceCaps(mhDC, LOGPIXELSY), 2540); // Height In Pixels

		GLint		glMaxTexDim ;											// Holds Maximum Texture Size
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported
		// Resize Image To Closest Power Of Two
		//if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		//	lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
		//else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		//	lWidthPixels = glMaxTexDim;

		//if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		//	lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
		//else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		//	lHeightPixels = glMaxTexDim;

		//	Create A Temporary Bitmap
		BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
		                             										// Pointer To The Bitmap Bits

		bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
		bi.bmiHeader.biBitCount		= 32;									// 32 Bit
		bi.bmiHeader.biWidth		= lWidthPixels;							// Power Of Two Width
		bi.bmiHeader.biHeight		= -lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
		bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
		bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane


		//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
		HBITMAP    hbmpTemp = CreateDIBSection(mhDC, &bi, DIB_RGB_COLORS, (void**)&mPixelPtr, 0, 0); // Holds The Bitmap Temporarily

		if( !hbmpTemp )		// Did Creation Fail?
		{
			DeleteDC(mhDC);
			mhDC = NULL;
			pPicture->Release();										    // Decrements IPicture Reference Count
			return false;		
		}

		SelectObject(mhDC, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

		// Render The IPicture On To The Bitmap
		pPicture->Render(mhDC, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);


		DWORD* pBits = (DWORD*)mPixelPtr;		
		// Convert From BGR To RGB Format And Add An Alpha Value Of 255
		for(long i = 0; i < lWidthPixels * lHeightPixels; i++)				// Loop Through All Of The Pixels
		{
			BYTE* pPixel	= (BYTE*)(&pBits[i]);							// Grab The Current Pixel
			BYTE  temp		= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
			pPixel[0]		= pPixel[2];									// Move Red Value To Correct Position (1st)
			pPixel[2]		= temp;											// Move Temp Value To Correct Blue Position (3rd)
			pPixel[3]	    = 255;
		}
		return true;
	}

}//namespace sf
#endif