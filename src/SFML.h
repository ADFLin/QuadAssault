#ifndef SFML_h__
#define SFML_h__

#include <Windows.h>

namespace sf
{
	class Color
	{
		typedef unsigned char Byte;
	public:
		Color( Byte r , Byte g , Byte b )
			:r(r),g(g),b(b){}

		Byte r , g , b;
	};
	struct Rect
	{
		int Width;
		int Height;
	};
	struct Style
	{
		enum Value
		{
			Close ,
			FullScreen ,
		};
	};
	class Keyboard
	{
	public:

		struct Key
		{
			enum Value
			{
				A = 'A',
				D = 'D',
				G = 'G',
				I = 'I',
				K = 'K',
				W = 'W',
				S = 'S',

				Escape = VK_ESCAPE ,


				LShift = VK_LSHIFT,
				Left   = VK_LEFT ,
				Right  = VK_RIGHT ,
				Up     = VK_UP ,
				Down   = VK_DOWN ,

				F1   = VK_F1 ,
				F2   = VK_F2 ,
				F3   = VK_F3 ,
				F4   = VK_F4 ,
				F5   = VK_F5 ,
				F6   = VK_F6 ,

				Num1 = '1',
				Num2 = '2',
				Num3 = '3',
				Num4 = '4',
				Num5 = '5',
				Num6 = '6',
				Num7 = '7',
				Num8 = '8',
				Num9 = '9',

				Numpad1 = VK_NUMPAD1 ,
				Numpad2 = VK_NUMPAD2 ,
				Numpad3 = VK_NUMPAD3 ,
				Numpad4 = VK_NUMPAD4 ,
				Numpad5 = VK_NUMPAD5 ,
				Numpad6 = VK_NUMPAD6 ,
				Numpad7 = VK_NUMPAD7 ,
				Numpad8 = VK_NUMPAD8 ,
				Numpad9 = VK_NUMPAD9 ,
			};
		};
		static bool isKeyPressed( Key::Value key )
		{
			return ( ::GetAsyncKeyState( int( key ) ) & 0x8000 ) != 0;
		}
	};

	class Mouse
	{
	public:
		struct Button
		{
			enum Value
			{
				Left ,
				Middle ,
				Right ,
			};

		};
		static bool IsButtonPressed( Button::Value b )
		{

			return false;
		}
	};

	struct Event
	{
		enum TypeValue
		{
			Closed ,
			MouseMoved ,
			MouseButtonPressed ,
			MouseButtonReleased ,
			KeyPressed ,
			KeyReleased ,
		};
		TypeValue Type;


		union
		{
			struct  
			{
				int X;
				int Y;
			} MouseMove;

			struct
			{
				Mouse::Button::Value Button;

			} MouseButton;

			struct  
			{
				Keyboard::Key::Value Code;
			} Key;
		};

	};

	class Font
	{
	public:
		bool loadFromFile( char const* )
		{
			return false;
		}

	};

	class Text
	{
	public:
		Rect const& getRect(){ return mRect; }
		void setCharacterSize( int size ){}
		void setString( char const* str ){}
		void setPosition( int x , int y ){}
		void setColor( Color const& color ){}
		void setFont( Font const& font ){}
	private:
		Rect mRect;

	};

	class VideoMode
	{
	public:
		VideoMode( int w , int h , int cbit )
			:Width(w),Height(h),ColorBit(cbit){}
		int Width;
		int Height;
		int ColorBit;
	};

	class RenderWindow
	{
	public:
		RenderWindow();

		bool Create( VideoMode const& mode , char const* title , int style );
		void Close();
		void Display();


		int GetHeight(){ return mHeight; }
		int GetWidth(){ return mWidth; }

		void ShowMouseCursor( bool beS );

		long GetFrameTime(){ return 0; }

		bool PollEvent( Event& event );

		void Draw( Text const& text )
		{

		}

		void SaveGLStages(){}
		void RestoreGLStages(){}



		void  ProcMsg( UINT message, WPARAM wParam, LPARAM lParam );

	private:
		Event* mEvent;
		int    mWidth;
		int    mHeight;
		HWND   mhWnd;
		HDC    mhDC;
		HGLRC  mhRC;
	};

	class Image
	{
	public:
		Image();
		bool  loadFromFile( char const* path );
		int   GetHeight(){ return mHeight; }
		int   GetWidth(){ return mWidth; }
		void* GetPixelsPtr(){ return 0; }
	private:

		HDC   mhDC;
		void* mPixelPtr;
		int   mWidth;
		int   mHeight;
	};



	class SoundBuffer
	{
	public:
		bool loadFromFile( char const* path )
		{
			return true;
		}

	};


	class Sound
	{
	public:
		Sound()
		{
			mBuffer = 0;
		}
		enum Status
		{
			Playing ,
			Finish ,
		};
		bool loadFromFile( char const* path )
		{
			return true;
		}

		Status GetStatus(){ return Finish; }

		SoundBuffer* GetBuffer(){ return mBuffer; }
		void         SetBuffer( SoundBuffer& buffer ){}
		void         play(){}
		void         Stop(){}
	private:
		SoundBuffer* mBuffer;
	};

	class Music
	{
	public:

		bool OpenFromFile( char const* path ){ return false; }
		void SetVolume( int value ){}
		void SetLoop( bool beL ){}
		void play(){}
		void Stop(){}
	};






}//namespace sf

#endif // SFML_h__
