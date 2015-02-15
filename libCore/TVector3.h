#ifndef TVector3_h__
#define TVector3_h__

template< class T >
class TVector3
{
public:
	typedef T RefConstType;
	typedef T ScaleType;

	TVector3(){}
	TVector3( T v[] ){ setValue( v[0] , v[1] , v[2] ); }
	TVector3( RefConstType sx,RefConstType sy, RefConstType sz);

	template< class Q >
	TVector3( TVector3< Q > const& rhs )
		:x( rhs.x ),y(rhs.y),z(rhs.z){}

	void setValue( RefConstType sx,RefConstType sy,RefConstType sz)
	{ x=sx; y=sy; z=sz;}

	T length2() const { return dot( *this ); }

	operator T*(){ return &x; }
	operator T const*() const { return &x; }

	TVector3& operator += ( TVector3 const& v);
	TVector3& operator -= ( TVector3 const& v);
	TVector3& operator *= ( T s );


	TVector3 mul( TVector3 const& v )
	{
		return TVector3( x * v.x , y * v.y , z * v.z );
	}

	T        dot(TVector3 const& b) const;
	TVector3 cross(TVector3 const& b) const;

	bool     operator == ( TVector3 const& v ) const { return x == v.x && y == v.y && z == v.z; }
	bool     operator != ( TVector3 const& v ) const { return !this->operator == ( v ); }

public:
	T x,y,z;

private:
	void operator + ( int ) const;
	void operator - ( int ) const;
	void operator +=( int ) const;
	void operator -=( int ) const;
};



template< class T >
inline TVector3<T>::TVector3( RefConstType sx, RefConstType sy, RefConstType sz ) 
	:x(sx),y(sy),z(sz)
{

}

template< class T >
inline T TVector3<T>::dot( TVector3 const& b ) const
{
	return x*b.x + y*b.y + z*b.z;
}

template< class T >
inline TVector3<T> TVector3<T>::cross( TVector3<T> const& b ) const
{
	return TVector3( y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x );
}

template< class T >
inline TVector3<T>& TVector3<T>::operator += (const TVector3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

template< class T >
inline TVector3<T>& TVector3<T>::operator -= (const TVector3& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

template< class T >
inline TVector3<T>& TVector3<T>::operator *= ( RefConstType s )
{
	x *= s; y *= s; z *= s;
	return *this;
}

template< class T >
inline TVector3<T> operator + (TVector3<T> const& a,TVector3<T> const& b )
{
	return TVector3< T >(a.x+b.x,a.y+b.y,a.z+b.z);
}

template< class T >
inline TVector3<T> operator- (TVector3<T> const& a,TVector3<T> const& b)
{
	return TVector3< T >(a.x-b.x,a.y-b.y,a.z-b.z);
}

template< class T >
inline TVector3<T> operator * ( typename TVector3<T>::RefConstType s , TVector3<T> const& v )
{
	return TVector3< T >( s*v.x , s*v.y , s*v.z );
}

template< class T >
inline TVector3<T> operator*(TVector3<T> const& v, typename TVector3<T>::RefConstType s)
{
	return TVector3<T>( s*v.x , s*v.y , s*v.z );
}

template< class T >
inline TVector3<T> operator/(TVector3<T> const& v, typename TVector3<T>::RefConstType s)
{
	return TVector3<T>( v.x / s , v.y / s , v.z / s );
}


template< class T >
inline TVector3<T> operator-(TVector3<T> const& a)
{
	return TVector3<T>(-a.x,-a.y,-a.z);
}

#endif // TVector3_h__