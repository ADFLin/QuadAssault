#ifndef EasingFun_h__
#define EasingFun_h__

// ==================================================================================================================================
// TWEENING EQUATIONS functions -----------------------------------------------------------------------------------------------------
// (the original equations are Robert Penner's work as mentioned on the disclaimer)

/**
* Easing equation function for a simple linear tweening, with no easing.
*
* @param t        Current time (in frames or seconds).
* @param b        Starting value.
* @param c        Change needed in value.
* @param d        Expected easing duration (in frames or seconds).
* @return        The correct value.
*/

namespace Easing {

	template< class T >
	class Length { float get( T& val ); };

	double const PI = 3.1415926535897932384;

    struct Linear
    {
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return ( t / d ) * c + b;
        }
    };

    /**
     * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
     */
    struct IQuad
    {
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t/=d;
            return c*t*t + b;
        }
    };

    /**
     * Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
     */
    class OQuad
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t/=d;
            t*=(t-2);
            return c * (-t ) + b;
        }
    };

	

    /**
     * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOQuad
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if ((t/=d/2) < 1) return c/2*t*t + b;
            --t;
            return -(t*(t-2)-1)/2 * c + b;
        }
    };

    /**
     * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
     */
    class OIQuad
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OQuad()(t*2, b, c/2, d );
            return IQuad()((t*2)-d, b+c/2, c/2, d );
        }
    };

    /**
     * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
     */
    class ICubic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t/=d;
            return c*t*t*t + b;
        }
    };

    /**
     * Easing equation function for a cubic (t^3) easing out: decelerating from zero velocity.
     */
    class OCubic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t=(t/d)-1;
            return c*(t*t*t + 1) + b;
        }
    };

    /**
     * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOCubic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if ((t/=d/2) < 1) return c/2*t*t*t + b;
            t-=2;
            return c/2*(t*t*t + 2) + b;
        }
    };

    /**
     * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
     */
    class OICubic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d )
		{
            if (t < d/2) return OCubic()(t*2, b, c/2, d );
            return ICubic()((t*2)-d, b+c/2, c/2, d );
        }
    };

    /**
     * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
     */
    class IQuart
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t/=d;
            return c*t*t*t*t + b;
        }
    };

    /**
     * Easing equation function for a quartic (t^4) easing out: decelerating from zero velocity.
     */
    class OQuart
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t=(t/d)-1;
            return -(t*t*t*t - 1) * c + b;
        }
    };

    /**
     * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOQuart
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
            t-=2;
            return -c/2 * (t*t*t*t - 2) + b;
        }
    };

    /**
     * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
     */
    class OIQuart
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OQuart()(t*2, b, c/2, d );
            return IQuart()((t*2)-d, b+c/2, c/2, d );
        }
    };

    /**
     * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
     */
    class IQuint
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t/=d;
            return c*t*t*t*t*t + b;
        }
    };

    /**
     * Easing equation function for a quintic (t^5) easing out: decelerating from zero velocity.
     */
    class OQuint
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t=(t/d)-1;
            return c*(t*t*t*t*t + 1) + b;
        }
    };

    /**
     * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOQuint
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
            t-=2;
            return c/2*(t*t*t*t*t + 2) + b;
        }
    };

    /**
     * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
     */
    class OIQuint
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OQuint()(t*2, b, c/2, d );
            return IQuint()((t*2)-d, b+c/2, c/2, d );
        }
    };

    /**
     * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
     */
    class ISine
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return -c * cosf(t/d * (PI/2)) + c + b;
        }
    };

    //Added by arch_jslin
    /**
     * Easing equation function for a sinusoidal (sin(t)) easing in: going through a sine cycle
     *                                                               (back to the original point)
     */
    class SineCirc
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return static_cast<T>(c * (-cosf(t/d * (PI*2))+1)/2 + b);
        }
    };

    /**
     * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating from zero velocity.
     */
    class OSine
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return c * sinf(t/d * (PI/2)) + b;
        }
    };

    /**
     * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOSine
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return -c/2 * (cosf(PI*t/d) - 1) + b;
        }
    };

    /**
     * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
     */
    class OISine
    {
    public:
		template< class T >
        T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OSine()(t*2, b, c/2, d );
            return ISine()((t*2)-d, b+c/2, c/2, d );
        }
    };

    /**
     * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
     */
    class IExpo
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b - c * 0.001f;
        }
    };

    /**
     * Easing equation function for an exponential (2^t) easing out: decelerating from zero velocity.
     */
    class OExpo
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return (t==d) ? b+c : c * 1.001f * (-pow(2, -10 * t/d) + 1) + b;
        }
    };

    /**
     * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOExpo
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t==0) return b;
            if (t==d) return b+c;
            if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b - c * 0.0005f;
            return c/2 * 1.0005f * (-pow(2, -10 * --t) + 2) + b;
        }
    };

    /**
     * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
     */
    class OIExpo
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OExpo()(t*2, b, c/2, d );
            return IExpo()((t*2)-d, b+c/2, c/2, d );
        }
    };

    /**
     * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
     */
    class ICirc
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t/=d;
            return -c * (sqrt(1 - t*t) - 1) + b;
        }
    };

    /**
     * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating from zero velocity.
     */
    class OCirc
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            t=(t/d)-1;
            return c * sqrt(1 - t*t) + b;
        }
    };

    /**
     * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
     */
    class IOCirc
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
            t-=2;
            return c/2 * (sqrt(1 - t*t) + 1) + b;
        }
    };

    /**
     * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
     */
    class OICirc
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OCirc()(t*2, b, c/2, d );
            return ICirc()((t*2)-d, b+c/2, c/2, d );
        }
    };


    /**
     * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
     * @param a        Amplitude.
     * @param p        Period.
     */
    class IElastic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ,
                           T a = T(), float p=.0f) //additional param.
        {
            if(t==0) return b;  if((t/=d)==1) return b+c;  if(p==.0f) p=d*.3;
            float s = .0f;
			float la = Length<T>().get(a), lc = Length<T>().get(c); bool sign = ((c/a) >= T()) ? true : false;
            if( la==.0f || la < lc ) { a=c; s=p/4; }
            else s = p / (2*PI) * asinf (lc/la * (sign?1:-1));
            t-=1;
            return -(a*pow(2,10*t) * sinf( (t*d-s)*(2*PI)/p )) + b;
        }
    };

	template< class T , class Fun >
	class ElasticParam
	{
	public:
		typedef Fun EasingFun;
		T operator()( float t, T const& b, T const& c, float const& d ){ Fun().operator()( t , b , c , d , a , p ); }
		T     a;
		float p;
	};


    /**
     * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
     * @param a        Amplitude.
     * @param p        Period.
     */
    class OElastic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d , T a = T(), float p=.0f) //additional param.
        {
            if(t==0) return b;  if((t/=d)==1) return b+c;  if(p==.0f) p=d*.3;
            float s = .0f;
            float la = length(a), lc = length(c); bool sign = ((c/a) >= T()) ? true : false;
            if( la==.0f || la < lc ) { a=c; s=p/4; }
            else s = p / (2*PI) * asinf (lc/la * (sign?1:-1));
            return (a*pow(2,-10*t) * sinf( (t*d-s)*(2*PI)/p ) + c + b);
        }
    };

    /**
     * EEasing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
     * @param a        Amplitude.
     * @param p        Period.
     */
    class IOElastic
    {
    public:
		template<class T>
		T operator()( float t, T const& b, T const& c, float const& d , T a = T(), float p=.0f ) //additional param.
        {
            if (t==0) return b;  if ((t/=d/2)==2) return b+c;  if(p==.0f) p=d*(.3*1.5);
            float s = .0f;
            float la = length(a), lc = length(c); bool sign = ((c/a) >= T()) ? true : false;
            if ( length(a)==.0f || length(a) < length(c) ) { a=c; s=p/4; }
            else s = p/(2*PI) * asinf (lc/la * (sign?1:-1));
            t-=1;
            if (t < 1) return -.5*(a*pow(2,10*t) * sinf( (t*d-s)*(2*PI)/p )) + b;
            return a*pow(2,-10*t) * sinf( (t*d-s)*(2*PI)/p )*.5 + c + b;
        }
    };

    /**
     * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
     * @param a        Amplitude.
     * @param p        Period.
     */
    class OIElastic
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ,
			         T const& a = T(), float const& p=.0f) //additional param.
        {
            if (t < d/2) return OElastic()(t*2, b, c/2, d , a, p);
            return IElastic()((t*2)-d, b+c/2, c/2, d , a, p);
        }
    };

    /**
     * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
     * @param s        Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot,
     *              and the default value of 1.70158 produces an overshoot of 10 percent).
     */
    class IBack
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d ,
                           float s=.0f) //additional param.
        {
            if (s==.0f) s = 1.70158f;
            t/=d;
            return c*t*t*((s+1)*t - s) + b;
        }
    };

	template< class Fun >
	class BackParam
	{
	public:
		typedef Fun EasingFun;
		template< class T >
		T operator()( float t, T const& b, T const& c, float const& d ){ Fun().operator()( t , b , c , d , s ); }
		float s;
	};

    /**
     * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
     * @param s        Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot,
     *              and the default value of 1.70158 produces an overshoot of 10 percent).
     */
    class OBack
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d , float s=.0f) //additional param.
        {
            if (s==.0f) s = 1.70158f;
            t=(t/d)-1;
            return c*(t*t*((s+1)*t + s) + 1) + b;
        }
    };


    /**
     * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
     * @param s        Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot,
     *              and the default value of 1.70158 produces an overshoot of 10 percent).
     */
    class IOBack
    {
    public:
		template<class T>
		T operator()(float t, T const& b, T const& c, float const& d , float s=.0f) //additional param.
        {
            if (s==.0f) s = 1.70158f;
            s*=1.525;
            if ((t/=d/2) < 1) return c/2*(t*t*((s+1)*t - s)) + b;
            t-=2;
            return c/2*(t*t*((s+1)*t + s) + 2) + b;
        }
    };

    /**
     *  Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
     */
    class OIBack
    {
    public:
		template< class T >
		T operator()( float t, T const& b, T const& c, float const& d  , float s=.0f ) //additional param.
        {
            if (t < d/2) return OBack()(t*2, b, c/2, d, s);
            return IBack()((t*2)-d, b+c/2, c/2, d , s);
        }
    };

    /**
     * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
     */
    class OBounce
    {
    public:
		template< class T >
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if ((t/=d) < (1/2.75f)) {
                return c*(7.5625f*t*t) + b;
            } else if (t < (2/2.75f)) {
                t-=(1.5f/2.75f);
                return c*(7.5625f*t*t + .75f) + b;
            } else if (t < (2.5f/2.75f)) {
                t-=(2.25f/2.75f);
                return c*(7.5625f*t*t + .9375f) + b;
            } else {
                t-=(2.625f/2.75f);
                return c*(7.5625f*t*t + .984375f) + b;
            }
        }
    };

    /**
     * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
     */
    class IBounce
    {
    public:
		template< class T >
		T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            return c - OBounce()( d- t, T(), c, d ) + b;
        }
    };

    /**
     * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
     */
   
    class IOBounce
    {
    public:
		template<class T>
        T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return IBounce()(t*2, T(), c, d ) * 0.5 + b;
            else return OBounce()(t*2-d, T(), c, d ) * 0.5 + c* 0.5 + b;
        }
    };

    /**
     * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
     */
    class OIBounce
    {
    public:
		template<class T>
        T operator()(float t, T const& b, T const& c, float const& d ) 
		{
            if (t < d/2) return OBounce()(t*2, b, c/2, d );
            return IBounce()((t*2)-d, b+c/2, c/2, d );
        }
    };



	template< class Fun >
	struct CycleFun
	{
		template<class T>
		T operator()( float t , T const& b, T const& c, float const& d ) 
		{
			if (t < d/2) return Fun()( t , b , c , d / 2 );
			return  Fun()( d - t , b , c , d / 2 );
		}
	};
	typedef CycleFun< Linear  > CLinear;
	typedef CycleFun< IQuad   > CIQuad;
	typedef CycleFun< OQuad   > COQuad;
	typedef CycleFun< IOQuad  > CIOQuad;
	typedef CycleFun< ICubic  > CICubic;
	typedef CycleFun< OCubic  > COCubic;
	typedef CycleFun< IOCubic > CIOCubic;
	

} //namespace Earsing


#endif // EasingFun_h__
