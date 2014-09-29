#ifndef MetaTemplate_hpp__
#define MetaTemplate_hpp__

namespace Meta
{
	struct EmptyType {};
	struct TrueType  { enum { Result = 1 }; };
	struct FalseType { enum { Result = 0 }; };

	template < class T > 
	struct HaveResult
	{
		typedef T ResultType;
		enum   { Result = T::Result };
	};
	template < bool , class T1 , class T2 >
	struct Select{};

	template < class T1 , class T2 >
	struct Select< true , T1 , T2 >
	{
		typedef T1 ResultType;
	};

	template < class T1 , class T2 >
	struct Select< false , T1 , T2 >
	{
		typedef T2 ResultType;
	};

	template < bool , int N1 , int N2 >
	struct SelectValue{};

	template < int N1 , int N2 >
	struct SelectValue< true , N1 , N2 >{ enum  { Result = N1 }; };

	template < int N1 , int N2 >
	struct SelectValue< false , N1 , N2  >{ enum  { Result = N2 }; };


	template < class T , class Q >
	struct IsSameType : HaveResult< FalseType >{};

	template < class T >
	struct IsSameType< T , T > : HaveResult< TrueType >{};


	template < class T >
	struct IsPointer : HaveResult< FalseType >{};
	template < class T >
	struct IsPointer< T* > : HaveResult< TrueType >{};
	template < class T >
	struct IsPointer< T const* > : HaveResult< TrueType >{};

	template < class T >
	struct IsPrimary : HaveResult< 
							typename Select< IsPointer<T>::Result , 
							TrueType , FalseType >::ResultType >
	{};

#define DEINE_PRIMARY_TYPE( type )\
	template <> struct IsPrimary< type > : HaveResult< TrueType >{};

	DEINE_PRIMARY_TYPE( bool )

	DEINE_PRIMARY_TYPE( char )
	DEINE_PRIMARY_TYPE( short )
	DEINE_PRIMARY_TYPE( int )
	DEINE_PRIMARY_TYPE( long )
	DEINE_PRIMARY_TYPE( long long )

	DEINE_PRIMARY_TYPE( unsigned char )
	DEINE_PRIMARY_TYPE( unsigned short )
	DEINE_PRIMARY_TYPE( unsigned int )
	DEINE_PRIMARY_TYPE( unsigned long )
	DEINE_PRIMARY_TYPE( unsigned long long )

	DEINE_PRIMARY_TYPE( double )
	DEINE_PRIMARY_TYPE( float )

#undef DEINE_PRIMARY_TYPE

}//namespace MTSpace


#endif // MetaTemplate_hpp__