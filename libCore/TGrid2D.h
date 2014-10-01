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
	void swap( MappingPolicy& p );
};

template< class T >
struct SimpleMapping
{
public:
	static T&       getData( T* storage , int sx , int sy , int i , int j )             
	{ assert( storage ); return storage[ i + sx * j ];  }
	static T const& getData( T const* storage , int sx , int sy , int i , int j )
	{ assert( storage ); return storage[ i + sx * j ];  }

	void build( T* storage , int sx , int sy ){}
	void cleanup( int sx , int sy ){}
	void swap( SimpleMapping& p ){}
};


template < class T >
struct FastMapping
{
	FastMapping(){  mMap = 0;  }
	T&       getData( T* storage , int sx , int sy , int i , int j )             
	{ assert( mMap ); return mMap[j][i]; }
	T const& getData( T const* storage , int sx , int sy , int i , int j ) const 
	{ assert( mMap ); return mMap[j][i]; }

	inline void build( T* storage , int sx , int sy )
	{
		mMap     = new T*[ sy ];
		
		T**  ptrMap = mMap;
		for( int i = 0 ; i < sy; ++i )
		{
			*ptrMap = storage;
			++ptrMap;
			storage += sx;
		}
	}
	inline void cleanup( int sx , int sy )
	{
		delete [] mMap;
		mMap = 0;
	}

	void swap( FastMapping& p )
	{
		using std::swap;
		swap( mMap , p.mMap );
	}
	T**  mMap;
};

template < class T , template< class > class MappingPolicy = SimpleMapping >
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

	T&       getData( int i , int j )       { assert( checkRange( i , j ) ); return MP::getData( mStorage , mSizeX , mSizeY , i , j ); }
	T const& getData( int i , int j ) const { assert( checkRange( i , j ) ); return MP::getData( mStorage , mSizeX , mSizeY , i , j ); }

	T&       operator()( int i , int j )       { return getData( i , j ); }
	T const& operator()( int i , int j ) const { return getData( i , j ); }


	T&       operator[]( int idx )       { assert( 0 <= idx && idx < mSizeX * mSizeY ); return mStorage[ idx ]; }
	T const& operator[]( int idx ) const { assert( 0 <= idx && idx < mSizeX * mSizeY ); return mStorage[ idx ]; }

	T*       getRawData()       { return mStorage; }
	T const* getRawData() const { return mStorage; }

	int  toIndex( int x , int y ) const { return x + y * mSizeX; }

	void resize( int x , int y )
	{
		cleanup();
		build( x , y );
	}
	void fillValue( T const& val ){	std::fill_n( mStorage , mSizeX * mSizeY , val );  }

	bool checkRange( int i , int j ) const
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


	void     swap( TGrid2D& other )
	{
		using std::swap;
		swap( mStorage , other.mStorage );
		swap( mSizeX , other.mSizeX );
		swap( mSizeY , other.mSizeY );
		MP::swap( *this );
	}


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
