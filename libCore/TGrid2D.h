#ifndef TGrid2D_h__
#define TGrid2D_h__

#include <cassert>

template< class T >
class MappingPolicy
{
	T&       getData( T* storage , int sx , int sy , int i , int j );
	T const& getData( T const* storage , int sx , int sy , int i , int j ) const;
	void build( T* storage , int sx , int sy );
	void cleanup();
};

template< class T >
struct SimpleMappingPolicy
{
public:
	static T&       getData( T* storage , int sx , int sy , int i , int j )             
	{ assert( storage ); return storage[ j + sy * i ];  }
	static T const& getData( T const* storage , int sx , int sy , int i , int j )
	{ assert( storage ); return storage[ j + sy * i ];  }

	void build( T* storage , int sx , int sy ){}
	void cleanup( int sx , int sy ){}
};


template < class T >
struct FastMappingPolicy
{
	FastMappingPolicy(){  mMap = 0;  }
	T&       getData( T* storage , int sx , int sy , int i , int j )             
	{ assert( mMap ); return mMap[i][j]; }
	T const& getData( T const* storage , int sx , int sy , int i , int j ) const 
	{ assert( mMap ); return mMap[i][j]; }

	inline void build( T* storage , int sx , int sy )
	{
		mMap     = new T*[ sx ];
		
		T**  ptrMap = mMap;
		for( int i = 0 ; i < sx; ++i )
		{
			*ptrMap = storage;
			++ptrMap;
			storage += sy;
		}
	}
	inline void cleanup( int sx , int sy )
	{
		delete [] mMap;
		mMap = 0;
	}
	T**  mMap;
};

template < class T , template< class > class MappingPolicy = SimpleMappingPolicy >
class TGrid2D : private MappingPolicy< T >
{
	typedef MappingPolicy< T > MP;
public:
	
	TGrid2D()
	{
		mStorage = 0;
		mSizeX = mSizeY = 0;
	}
	TGrid2D( int sx , int sy )
	{
		build( sx , sy );
	}

	TGrid2D( TGrid2D const& rhs ){  copy( rhs );  }

	~TGrid2D(){  cleanup(); }

	typedef T*       iterator;
	typedef T const* const_iterator;

	iterator begin(){ return mStorage; }
	iterator end()  { return mStorage + mSizeX * mSizeY; }

	T&       getData( int i , int j )       { assert( isVaildRange( i , j ) ); return MP::getData( mStorage , mSizeX , mSizeY , i , j ); }
	T const& getData( int i , int j ) const { assert( isVaildRange( i , j ) ); return MP::getData( mStorage , mSizeX , mSizeY , i , j ); }

	T&       operator()( int i , int j )       { return getData( i , j ); }
	T const& operator()( int i , int j ) const { return getData( i , j ); }

	T&       operator[]( int idx )       { assert( 0 <= idx && idx < mSizeX * mSizeY ); return mStorage[ idx ]; }
	T const& operator[]( int idx ) const { assert( 0 <= idx && idx < mSizeX * mSizeY ); return mStorage[ idx ]; }

	void resize( int x , int y )
	{
		cleanup();
		build( x , y );
	}
	void fillData( T const& val ){	std::fill_n( mStorage , mSizeX * mSizeY , val );  }

	bool isVaildRange( int i , int j ) const
	{
		return 0 <= i && i < mSizeX && 
			   0 <= j && j < mSizeY;
	}

	TGrid2D& operator = ( TGrid2D const& rhs )
	{
		copy( rhs );
		return *this;
	}

	int      getSizeX() const { return mSizeX; }
	int      getSizeY() const { return mSizeY; }
	int      getRawDataSize() const { return mSizeX * mSizeY; }
	T*       getRawData()       { return mStorage; }
	T const* getRawData() const { return mStorage; }

	int toIndex( int x , int y ) const { return y + x * mSizeY; }

private:

	void build( int x , int y )
	{
		mSizeX = x;
		mSizeY = y;
		mStorage = new T[ mSizeX * mSizeY ];

		MP::build( mStorage , x , y );
	}
	void cleanup()
	{ 
		delete [] mStorage;
		mStorage = 0; 

		MP::cleanup( mSizeX , mSizeY );
		mSizeX = mSizeY = 0;
	}

	void copy( TGrid2D const& rhs )
	{
		cleanup();
		build( rhs.mSizeX , rhs.mSizeY );
		std::copy( rhs.mStorage , rhs.mStorage + rhs.mSizeX  * rhs.mSizeY , mStorage );
	}

	T*   mStorage;
	int  mSizeX;
	int  mSizeY;
};

#endif // TGrid2D_h__
