#ifndef TVector2_h__
#define TVector2_h__

template<class T>
class TVector2
{
	typedef T const& RefConstType;
public:
	typedef T ScaleType;
	
	TVector2(){}
	TVector2( RefConstType x ,RefConstType y){ setValue(x,y); }

	template< class Q >
	TVector2( TVector2<Q> const& v ){ setValue( T(v.x) , T(v.y) ); }

	template< class Q >
	TVector2& operator = (TVector2<Q> const& v){  x = T(v.x);  y = T(v.y);  return *this; }

	void     setValue( RefConstType vx,RefConstType vy){	x = vx; y = vy;  }
	T        dot  ( TVector2 const& v ) const { return x * v.x + y * v.y ; }
	T        cross( TVector2 const& v ) const { return x * v.y - y * v.x ; }
	T        length2()                  const { return x * x + y * y; }

	TVector2 mul( TVector2 const& v ) const { return TVector2( x * v.x , y * v.y );  }
	TVector2 div( TVector2 const& v ) const { return TVector2( x / v.x , y / v.y );  }

	TVector2& operator *= ( RefConstType s )  {  x *= s ; y *= s; return *this;  }
	TVector2& operator /= ( RefConstType s )  {  x /= s ; y /= s; return *this;  }
	TVector2& operator += ( TVector2 const& v ){  x += v.x;  y += v.y;  return *this;  }
	TVector2& operator -= ( TVector2 const& v ){  x -= v.x;  y -= v.y;  return *this;  }

	TVector2 const operator - ( void ) const { return  TVector2( -x , -y );  }

	operator T*(){ return &x; }
	operator T const*() const { return &x; }

	static TVector2 Zero(){ return TVector2(0,0); }
	static TVector2 PositiveX(){ return TVector2(1,0); }
	static TVector2 PositiveY(){ return TVector2(0,1); }
	static TVector2 NegativeX(){ return TVector2(-1,0); }
	static TVector2 NegativeY(){ return TVector2(0,-1); }

	TVector2 const operator + (TVector2 const& v) const {	return TVector2(x + v.x,y + v.y);  }
	TVector2 const operator - (TVector2 const& v) const {	return TVector2(x - v.x,y - v.y);  }

	TVector2 const operator * ( RefConstType s ) const {	return TVector2( x * s, y * s );  }
	TVector2 const operator / ( RefConstType s ) const {	return TVector2( x / s, y / s );  }
	
	bool operator == (TVector2 const& v) const {  return x == v.x  && y == v.y;  }
	bool operator != (TVector2 const& v) const {  return ! (*this == v ); }

	friend TVector2 const operator * ( RefConstType s,TVector2 const& v){  return TVector2(s*v.x,s*v.y);	}

public:
	T x,y;

private:
	void operator + ( int ) const;
	void operator - ( int ) const;
	void operator +=( int ) const;
	void operator -=( int ) const;

};


#endif // TVector2_h__

