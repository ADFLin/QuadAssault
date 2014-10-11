#ifndef FixString_h__
#define FixString_h__

#include <cstring>
#include <string>

namespace Detail
{
	template< class CharT >
	struct CharFunTrait;

	template<>
	struct CharFunTrait< char > 
	{
		typedef char CharT;
		typedef std::string StdString;
		template< int N >
		static int    vprintf( CharT (&str)[ N ] , CharT const* fmt , va_list al ){ return ::vsprintf_s( str , fmt , al ); }
		template< int N >
		static void   copy( CharT (&dst)[ N ]  , CharT const* src ){ ::strcpy_s( dst , src ); }
		template< int N >
		static void   cat( CharT (&dst)[ N ]  , CharT const* src ){ ::strcat_s( dst , src ); }
		static int    compare( CharT const* s1 , CharT const* s2 ){ return ::strcmp( s1 , s2 ); }

	};
	template<>
	struct CharFunTrait< wchar_t > 
	{
		typedef wchar_t CharT;
		typedef std::wstring StdString;
		template< int N >
		static void   vprintf( CharT (&str)[ N ] , CharT const* fmt , va_list al ){ ::vswprintf_s( str , fmt , al ); }
		template< int N >
		static void   copy( CharT (&dst)[ N ]  , CharT const* src ){ ::wcscpy_s( dst , src ); }
		template< int N >
		static void   cat( CharT (&dst)[ N ]  , CharT const* src ){ ::wcscat_s( dst , src );  }
		static int    compare( CharT const* s1 , CharT const* s2 ){ return ::wcscmp( s1 , s2 ); }
	};
}

template< int N , class T = char >
class FixString
{
	typedef T CharT;
	typedef Detail::CharFunTrait< CharT > CFT;
	typedef typename CFT::StdString StdString;
public:
	FixString(){ mStr[0] = 0; }
	FixString( CharT const* str ){  assign( str ); }
	template< int M >
	FixString( FixString< M , CharT > const& other ){ assign( other.mStr );  }
	FixString( StdString const& str ){ assign( str.c_str() ); }

	FixString& operator = ( CharT const* str ){ assign( str ); return *this; }
	FixString& operator = ( FixString< N , CharT > const& other ){ assign( other.mStr );  return *this;  }
	template< int M >
	FixString& operator = ( FixString< M , CharT > const& other ){ assign( other.mStr );  return *this;  }
	FixString& operator = ( StdString const& str ){ assign( str.c_str() ); return *this; }

	bool operator == ( CharT const* str ) const { return compare( str ) == 0; }
	bool operator != ( CharT const* str ) const { return compare( str ) != 0; }

	FixString& operator += ( CharT const* str ){ CFT::cat( mStr , str ); return *this; }
	FixString& operator += ( StdString const& str ){ CFT::cat( mStr , str.c_str() ); return *this; }


	void  clear(){ mStr[0] = 0; }
	void  format( CharT const* fmt , ... )
	{
		va_list argptr;
		va_start( argptr, fmt );
		CFT::vprintf( mStr , fmt , argptr );
		va_end( argptr );
	}


	CharT const* c_str() const { return mStr; }
	

	operator CharT const*( ) const { return mStr; }
	operator CharT*      ( )       { return mStr; }



private:
	int   compare( CharT const* str ) const { return CFT::compare( mStr , str );  }
	void  assign( CharT const* str ){ CFT::copy( mStr , str );  }

	template< int M , class T >
	friend  class FixString;

	CharT mStr[ N ];
};

#endif // FixString_h__
