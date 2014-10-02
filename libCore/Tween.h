#ifndef Tween_h__
#define Tween_h__

#include <functional>
#include <vector>
#include <algorithm>

#include "MetaBase.hpp"

namespace Tween
{
	typedef std::tr1::function< void () > FinishCallback;
	typedef std::tr1::function< void () > StartCallback;

	class Access
	{
		class ValueType;
		class DataType;
		void operator()( DataType& data , ValueType const& value );
	};

	template< class TimeType > class GroupTweener;
	template< class TimeType > class SquenceTweener;

	class EmptyBuilder { };

	template< class T >
	struct ValueAccess
	{
		typedef T ValueType;
		typedef T DataType;
		void operator()( DataType& data , ValueType const& value ){ data = value; }
	};

	class NullParam 
	{
	public:
		template< class Fun , class T >
		T operator()( float t, T const& b, T const& c, float const& d )
		{
			return Fun()( t , b , c , d );
		}
	};

	template< class Access , bool BE_REF = true , class FunParam = NullParam >
	struct PropDataT : public FunParam
	{
		typedef PropDataT< Access , BE_REF , FunParam > PropData;
		typedef typename Access::ValueType ValueType;
		typedef typename Access::DataType  DataType;


		PropDataT( DataType& data , ValueType const& from , ValueType const& to )
			:mData( data )
			,mFrom( from )
			,mDiff( to - from )
		{

		}

		PropDataT( DataType& data , ValueType const& from , ValueType const& to , FunParam const& param )
			:FunParam( param )
			,mData( data )
			,mFrom( from )
			,mDiff( to - from )
		{

		}

		template< class Fun , class TimeType >
		void update( TimeType t , TimeType durtion )
		{
			Access()( mData  , FunParam::operator()< Fun >( t , mFrom , mDiff , durtion ) );
		}

		typedef typename Meta::Select< BE_REF , DataType& , DataType >::ResultType HoldType;
		HoldType  mData;
		ValueType mFrom;
		ValueType mDiff;
	};


	template< class TimeType >
	class IComponentT
	{
	public:
		virtual ~IComponentT(){}
		virtual TimeType update( TimeType time ) = 0;
		virtual void     modify( TimeType dt ) = 0;
		//virtual void     stop( bool goEnd ) = 0;
		virtual bool     isFinished() = 0;
		virtual void     reset() = 0;
	};


	template< class TIME > 
	class Detail
	{
	public:
		typedef TIME TimeType;

		typedef IComponentT< TimeType > IComponent;

		template< class T >
		class TweenBaseT
		{
			T* _this(){ return static_cast< T* >( this ); }
		public:

			TweenBaseT( TimeType durtion , TimeType delay = 0 )
				:mDurtion( durtion )
				,mCurTime( -delay )
				,mDelay( delay )
			{
				mRepeatTime = mDurtion;
				mRepeat     = 0;
				mTotalRepeat= 0;
			}

			void     doReset()
			{
				mRepeat  = mTotalRepeat;
				mCurTime = -mDelay;
			}

			void     doModify( TimeType dt )
			{
				if ( isFinished() )
					return;
				TimeType time = mCurTime + dt;
				if ( time < mDurtion )
				{
					_this()->accessValue( time );
				}
				else
				{
					_this()->accessValue( mDurtion );
				}
			}

			//  delay    durtion    repeat delay   durtion
			//--------|============|-------------|===========|-----------|=============|----->

			TimeType doUpdate( TimeType time )
			{
				if ( isFinished() )
					return TimeType( 0 );

				mCurTime += time;
				TimeType result = time; 

				for ( ;; )
				{
					if ( mCurTime < 0 )
						break;

					if ( mCurTime < mDurtion )
					{
						_this()->accessValue( mCurTime );
						break;
					}
					else if ( mRepeat != 0 )
					{
						if ( mRepeat > 0)
							mRepeat -= 1;
						mCurTime -= mRepeatTime;
					}
					else // mCurTime > mDurtion
					{
						_this()->accessValue( mDurtion );
						if( mFinishFun )
							mFinishFun();
						return time - ( mDurtion - mCurTime );
					}
				}
				return time;
			}

			void accessValue( TimeType t ){}


			bool  isFinished() const           { return mRepeat == 0 && mCurTime > mDurtion; }
			T&    finishCallback( FinishCallback fun ){ mFinishFun = fun; return *_this(); }
			T&    repeat( int num )            { mTotalRepeat = mRepeat = num; return *_this(); }
			T&    delay( TimeType time )       { mDelay = time ; mCurTime = -time; return *_this(); }
			T&    repeatDelay( TimeType time ) { mRepeatTime = mDurtion + time; return *_this(); }
			T&    cycle()                      { mTotalRepeat = mRepeat = -1; return *_this(); }

		protected:
			FinishCallback mFinishFun;

			int       mTotalRepeat;
			int       mRepeat;
			TimeType  mDelay;
			TimeType  mCurTime;
			TimeType  mDurtion;
			TimeType  mRepeatTime;
		};

		template< class Impl , class PropData , class Fun >
		class TweenCoreT : public TweenBaseT< Impl >
			             , private PropData
		{
			Impl* _this(){ return static_cast< Impl* >( this ); }

		public:
			typedef TweenCoreT< Impl , Fun , PropData > TweenCore;
			typedef TweenBaseT< Impl > TweenBase;


			template< class DataType , class ValueType >
			TweenCoreT( DataType& data , ValueType const& from , ValueType const& to , TimeType durtion , TimeType delay = 0 )
				:TweenBase( durtion , delay )
				,PropData( data , from , to )
			{

			}

			template< class DataType , class ValueType , class FunParam >
			TweenCoreT( DataType& data , ValueType const& from , ValueType const& to , TimeType durtion , TimeType delay , FunParam const& param )
				:TweenBase( durtion , delay )
				,PropData( data , from , to , param )
			{

			}

			void accessValue( TimeType t )
			{
				PropData::update< Fun >( t , mDurtion );
			}
		};

		template< class Impl >
		class MultiTweenCoreT : public TweenBaseT< Impl >
		{
			Impl* _this(){ return static_cast< Impl* >( this ); }
		public:
			typedef MultiTweenCoreT< Impl > MultiTweenCore;
			typedef TweenBaseT< Impl > TweenBase;

		public:

			MultiTweenCoreT( TimeType durtion , TimeType delay = 0 )
				:TweenBase( durtion , delay )
			{

			}

			~MultiTweenCoreT()
			{
				for( PropValueVec::iterator iter = mProps.begin() , end = mProps.end() ;
					iter != end ; ++iter )
				{
					IPropValue* pv = *iter;
					delete pv;
				}
			}

			struct IPropValue
			{
				virtual ~IPropValue(){}
				virtual void update( TimeType t , TimeType durtion ) = 0;
			};

			template< class Fun , class PropData >
			struct CPropValue : public IPropValue
				, private PropData
			{
				template< class DataType , class ValueType>
				CPropValue( DataType& data , ValueType const& from , ValueType const& to )
					:PropData( data , from , to ){}

				template< class DataType , class ValueType , class FunParam >
				CPropValue( DataType& data , ValueType const& from , ValueType const& to , FunParam const& param )
					:PropData( data , from , to , param ){}

				void update( TimeType t , TimeType durtion )
				{
					PropData::update< Fun >( t , durtion );
				}
			};

			template< class Fun , class T >
			Impl& addValue( T& data , T const& from , T const& to )
			{  
				typedef CPropValue< Fun , PropDataT< ValueAccess< T > >  > MyProp;
				MyProp* prop = new MyProp( data , from , to );
				mProps.push_back( prop );
				return *_this();
			}


			template< class Fun , class T , class FunParam >
			Impl& addValue( T& data , T const& from , T const& to , FunParam const& param )
			{  
				typedef CPropValue< Fun , PropDataT< ValueAccess< T > , FunParam >  > MyProp;
				MyProp* prop = new MyProp( data , from , to , param );
				mProps.push_back( prop );
				return *_this();
			}

			template< class Fun , class Access >
			Impl& add( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to )
			{  
				typedef CPropValue< Fun , PropDataT< Access >  > MyProp;
				MyProp* prop = new MyProp( data , from , to );
				mProps.push_back( prop );
				return *_this();
			}


			template< class Fun , class Access , class FunParam >
			Impl& add( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to ,
				FunParam const& param )
			{  
				typedef CPropValue< Fun , PropDataT< Access , FunParam >  > MyProp;
				MyProp* prop = new MyProp( data , from , to , param );
				mProps.push_back( prop );
				return *_this();
			}

			void accessValue( float t )
			{
				for( PropValueVec::iterator iter = mProps.begin() , end = mProps.end() ;
					iter != end ; ++iter )
				{
					IPropValue* pv = *iter;
					pv->update( t , mDurtion );
				}
			}

		private:
			typedef std::vector< IPropValue* > PropValueVec;
			std::vector< IPropValue* > mProps;
		};


		template< class Fun , class Access , class FunParam = NullParam >
		class CTween : public IComponent
			         , public TweenCoreT< CTween< Fun , Access , FunParam > ,  PropDataT< Access , true , FunParam > , Fun >
		{
		public:
			typedef TweenCoreT< CTween< Fun , Access , FunParam > , PropDataT< Access , true , FunParam > , Fun > Core;

			typedef typename Access::DataType DataType;
			typedef typename Access::ValueType ValueType;

			CTween( DataType& data , ValueType const& from , ValueType const& to , TimeType durtion , TimeType delay )
				:Core( data ,from , to , durtion , delay ){}

			CTween( DataType& data , ValueType const& from , ValueType const& to , TimeType durtion , TimeType delay , FunParam const& param )
				:Core( data ,from , to , durtion , delay , param ){}

			void     modify( TimeType dt ){ Core::doModify( dt ); }
			bool     isFinished(){ return Core::isFinished(); }
			TimeType update( TimeType time ){ return Core::doUpdate( time ); }
			void     reset(){ return Core::doReset(); }
		};

		class CMultiTween : public IComponent
			              , public MultiTweenCoreT< CMultiTween >
		{
		public:
			typedef MultiTweenCoreT< CMultiTween > Core;

			CMultiTween( TimeType durtion , TimeType delay )
				:Core( durtion , delay ){}

			void     modify( TimeType dt ){ Core::doModify( dt ); }
			bool     isFinished(){ return Core::isFinished(); }
			TimeType update( TimeType time ){ return Core::doUpdate( time ); }
			void     reset(){ return Core::doReset(); }
		};


		template< class Impl , class Access >
		class SimpleTweenerT
		{
		public:
			Impl* _this(){ return static_cast< Impl* >( this ); }

			typedef typename Access::DataType DataType;
			typedef typename Access::ValueType ValueType;

			template< class Fun , class T >
			CTween< Fun , ValueAccess< T > >&
			 tween( DataType& data , ValueType const& from , ValueType const& to , TimeType durtion , TimeType delay = 0 )
			{
				typedef CTween< Fun , ValueAccess< T >  > MyTween;
				MyTween* t = new MyTween( data , from , to , durtion , delay );
				_this()->addComponent( t );
				return *t;
			}

		};

		template< class Impl >
		class TweenerT : public IComponent
		{
			Impl* _this(){ return static_cast< Impl* >( this ); }


		public:
			template< class Fun , class T >
			CTween< Fun , ValueAccess< T > >&
				tweenValue( T& data , T const& from , T const& to , TimeType durtion , TimeType delay = 0 )
			{
				typedef CTween< Fun , ValueAccess< T >  > MyTween;
				MyTween* t = new MyTween( data , from , to , durtion , delay );
				_this()->addComponent( t );
				return *t;
			}

			template< class Fun , class T , class FunParam >
			CTween< Fun , ValueAccess< T > , FunParam >&
				tweenValue( T& data , T const& from , T const& to  , TimeType durtion , TimeType delay  , FunParam const& param )
			{
				typedef CTween< Fun , ValueAccess< T > , FunParam  > MyTween;
				MyTween* t = new MyTween( data , from , to , durtion , delay , param );
				_this()->addComponent( t );
				return *t;
			}


			template< class Fun  , class Access >
			inline CTween< Fun , Access >&
				tween( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to ,
				TimeType durtion , TimeType delay = 0 )
			{
				typedef CTween< Fun , Access > MyTween;
				MyTween* t = new MyTween( data , from , to , durtion , delay );
				_this()->addComponent( t );
				return *t;
			}

			template< class Fun , class Access , class FunParam >
			inline CTween< Fun , Access , FunParam >&
				tween( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to , 
				TimeType durtion , TimeType delay , FunParam const& param )
			{
				typedef CTween< Fun , Access , FunParam > MyTween;
				MyTween* t = new MyTween( data , from , to , durtion , delay , param );
				_this()->addComponent( t );
				return *t;
			}

			inline CMultiTween& 
				tweenMulti( TimeType durtion , TimeType delay = 0 )
			{
				CMultiTween* t = new CMultiTween( durtion , delay );
				_this()->addComponent( t );
				return *t;
			}

			typedef GroupTweener< TimeType >   CGroupTweener;
			typedef SquenceTweener< TimeType > CSquenceTweener;

			inline CGroupTweener& 
				group()
			{
				CGroupTweener* t = new CGroupTweener( false );
				_this()->addComponent( t );
				return *t;
			}

			inline CSquenceTweener&
				sequence()
			{
				CSquenceTweener* t = new CSquenceTweener;
				_this()->addComponent( t );
				return *t;
			}
		protected:
			void destroyComponent( IComponent* comp ){ delete comp; }
			void addComponent( IComponent* comp );
		};


		typedef GroupTweener< TimeType >   CGroupTweener;
		typedef SquenceTweener< TimeType > CSquenceTweener;

		template< class P , class Q >
		class Builder;


		template< class Q > 
		struct StorageTraits
		{
			typedef Q& Type;
		};

		template< > 
		struct StorageTraits< EmptyBuilder >
		{
			typedef EmptyBuilder Type;
		};
		template< class P , class Q >
		struct StorageTraits< Builder< P , Q > >
		{
			typedef Builder< P , Q > Type;
		};

		template< class P , class Q >
		class Builder
		{
			typedef Builder< P , Q > ThisType;
		public:
			Builder( P& p , Q& q )
				:mPrev( p ) , mCur( q ){}

			inline P& end(){ return mPrev; }

			//Tween fun
			inline ThisType& finishCallback( FinishCallback fun ){ mCur.finishCallback( fun ); return *this; }
			inline ThisType& repeat( int num )                   { mCur.repeat( num ); return *this; }
			inline ThisType& delay( TimeType time )              { mCur.delay( time ); return *this; }
			inline ThisType& repeatDelay( TimeType time )        { mCur.repeatDelay( time ); return *this; }
			inline ThisType& cycle()                             { mCur.cycle(); return *this; }

			//MultiTween
			template< class Fun , class T >
			inline ThisType& addValue( T& data , T const& from , T const& to )
			{  return makeBuilder( mCur.addValue< Fun >( data , from , to ) );  }


			template< class Fun , class T , class FunParam >
			inline ThisType& addValue( T& data , T const& from , T const& to , FunParam const& param )
			{  return makeBuilder( mCur.addValue< Fun >( data , from , to , param ) );  }

			template< class Fun , class Access >
			inline ThisType& add( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to )
			{  return makeBuilder( mCur.add< Fun , Access >( data , from , to ) );  }


			template< class Fun , class Access , class FunParam >
			inline ThisType& add( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to ,
				FunParam const& param )
			{  return makeBuilder( mCur.add< Fun , Access >( data , from , to , param ) );  }

			//Tweener fun
			template< class Fun , class T >
			inline Builder< ThisType , CTween< Fun , ValueAccess< T > > >
				tweenValue( T& data , T const& from , T const& to , TimeType durtion , TimeType delay = 0 )
			{  return makeBuilder( mCur.tweenValue< Fun >( data , from , to , durtion , delay ) );  }

			template< class Fun , class T , class FunParam >
			inline Builder< ThisType , CTween< Fun , ValueAccess< T > , FunParam > >
				tweenValue( T& data , T const& from , T const& to , TimeType durtion , TimeType delay , FunParam const& param )
			{  return makeBuilder( mCur.tweenValue< Fun >( data , from , to , durtion , delay , param ) );  }

			template< class Fun  , class Access >
			inline Builder< ThisType , CTween< Fun , Access > >
				tween( typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to ,
				TimeType durtion , TimeType delay = 0 )
			{  return makeBuilder( mCur.tween< Fun , Access >( data , from , to , durtion , delay ) );  }


			template< class Fun , class Access , class FunParam >
			inline Builder< ThisType , CTween< Fun , Access , FunParam > >
				tween(  typename Access::DataType& data , 
				typename Access::ValueType const& from , 
				typename Access::ValueType const& to , 
				TimeType durtion , TimeType delay , FunParam const& param )
			{  return makeBuilder( mCur.tween< Fun , Access >( data , from , to , durtion , delay , param ) );  }


			inline Builder< ThisType , CMultiTween > 
				tweenMulti( TimeType durtion , TimeType delay = 0 )
			{
				return makeBuilder( mCur.tweenMulti( durtion , delay ) );
			}

			inline Builder< ThisType , CGroupTweener >
				group(){  return makeBuilder( mCur.group() );  }

			inline Builder< ThisType , CSquenceTweener >
				sequence(){  return makeBuilder( mCur.sequence() );  }
		private:

			typename StorageTraits< Q >::Type mCur;
			typename StorageTraits< P >::Type mPrev;
		
			template< class U >
			Builder< ThisType , U > makeBuilder( U& u ){ return Builder< ThisType , U >( *this , u ); }
			template< class U , class V >
			Builder< ThisType , V > makeBuilder( Builder< U , V > const& b ){ return Builder< ThisType , V >( *this , b.mCur ); }
		};
	};

	template< class TimeType >
	class Define
	{
	public:
		//typedef typename Detail< TimeType >::IComponent IComponent;
		typedef typename Detail< TimeType >::CMultiTween CMultiTween;
		typedef IComponentT< TimeType > IComponent;

	};

	template< class TimeType >
	class GroupTweener : public Detail< TimeType >::template TweenerT< GroupTweener< TimeType >  >
		               , public Define< TimeType >
	{
		typedef GroupTweener ThisType;
	public:
		GroupTweener( bool beRM = true )
		{
			mAutoRemove = beRM;
		}
		~GroupTweener()
		{
			cleanup( true );
		}

	private:
		void modify( TimeType dt )
		{
			int size = (int)mActiveComps.size();
			for( CompVec::iterator iter = mStorage.begin() , end = mStorage.end();
				iter != end; ++iter )
			{
				IComponent* comp = *iter;
				comp->modify( dt );
			}
		}

	public:

		ThisType& autoRemove( bool beE ){ mAutoRemove = beE; return *this; }

		void remove( IComponent* comp )
		{
			CompVec::iterator iter = std::find( mStorage.begin() , mStorage.end() , comp );
			if ( iter == mStorage.end() )
				return;

			mStorage.erase( iter );
			destroyComponent( comp );

			iter = std::find( mActiveComps.begin() , mActiveComps.end() , comp );
			if ( iter == mActiveComps.end() )
				return;
			mActiveComps.erase( iter );
		}

		void reset()
		{
			mActiveComps.assign( mStorage.begin() , mStorage.end() );
			for( CompVec::iterator iter = mStorage.begin() , end = mStorage.end();
				iter != end; ++iter )
			{
				IComponent* comp = *iter;
				comp->reset();
			}
		}

		TimeType update( TimeType time )
		{
			int idx = 0;
			TimeType result = 0;
			for ( int i = 0; i < (int)mActiveComps.size() ; ++i )
			{
				IComponent* comp = mActiveComps[i];
				comp->update( time );
				if ( !comp->isFinished() )
				{
					if ( i != idx )
						mActiveComps[ idx ] = comp;

					++idx;
				}
				else if ( mAutoRemove )
				{
					mStorage.erase( std::find( mStorage.begin() , mStorage.end() , comp ) );
					destroyComponent( comp );
				}
			}

			if ( idx != mActiveComps.size()  )
				mActiveComps.resize( idx );

			return 0;
		}

		void cleanup( bool beDestroy )
		{
			for( CompVec::iterator iter = mStorage.begin() , end = mStorage.end();
				  iter != end; ++iter )
			{
				destroyComponent( *iter );
			}
			if ( !beDestroy )
			{
				mStorage.clear();
				mActiveComps.clear();
			}
		}

		void clear(){ cleanup( false ); }

		bool isFinished(){ return getActiveNum() != 0; }
		void addComponent( IComponent* comp )
		{ 
			mStorage.push_back( comp ); 
			mActiveComps.push_back( comp ); 
		}
		int  getActiveNum() const{ return (int)mActiveComps.size(); }
		int  getTotalNum() const { return (int)mStorage.size(); }

		bool    mAutoRemove;
		typedef std::vector< IComponent* > CompVec;
		CompVec mActiveComps;
		CompVec mStorage;
	};

	template< class TimeType >
	class SquenceTweener : public Detail< TimeType >::template TweenerT< SquenceTweener< TimeType > >
		                 , public Define< TimeType >
	{
		typedef SquenceTweener< TimeType > ThisType;
	public:
		SquenceTweener()
		{
			mNumFinished = 0;
			mTotalRepeat = 0;
			mRepeat   = 0;
		}
		~SquenceTweener()
		{
			cleanup( true );
		}

		ThisType& cycle()
		{
			mRepeat = mTotalRepeat = -1;
			return *this;
		}
		void reset()
		{
			for( CompVec::iterator iter = mStorage.begin() , end = mStorage.end();
				iter != end; ++iter )
			{
				IComponent* comp = *iter;
				comp->reset();
			}

			mRepeat = mTotalRepeat;
			mNumFinished = 0;
		}

		void remove( IComponent* comp )
		{
			int idx = 0;
			for( ; idx < (int)mStorage.size() ; ++idx )
			{
				if ( mStorage[ idx ] == comp )
					break;
			}

			if ( idx >= (int)mStorage.size() )
				return;

			destroyComponent( mStorage[idx] );
			mStorage.erase( mStorage[ idx ] );

			if ( idx < mNumFinished )
			{
				--mNumFinished;
			}
		}

		void modify( TimeType dt )
		{
			if ( mNumFinished < (int)mStorage.size() )
			{



			}
		}

		TimeType update( TimeType time )
		{
			TimeType totalUseTime = 0;
			while (  mNumFinished < (int)mStorage.size() )
			{
				IComponent* comp = mStorage[mNumFinished];
				TimeType useTime = comp->update( time );
				totalUseTime += useTime;

				if ( !comp->isFinished() )		
					break;
				time -= useTime;

				++mNumFinished;
				if ( mNumFinished == mStorage.size() )
				{
					if ( mRepeat == 0 )
						break;

					if ( mRepeat > 0 )
						mRepeat -= 1;

					reset();
				}
			}
			return totalUseTime;
		}

		void clear(){ cleanup( false ); }
		bool isFinished(){ return mNumFinished == (int)mStorage.size(); }


		void addComponent( IComponent* comp )
		{ 
			mStorage.push_back( comp ); 
		}


		void cleanup( bool beDestroy )
		{
			for( CompVec::iterator iter = mStorage.begin() , end = mStorage.end();
				iter != end; ++iter )
			{
				destroyComponent( *iter );
			}
			if ( !beDestroy )
			{
				mStorage.clear();
				mNumFinished = 0;
			}
		}
		int mNumFinished;
		int mTotalRepeat;
		int mRepeat;
		typedef std::vector< IComponent* > CompVec;
		CompVec mStorage;
	};

	template< class T , class TimeType >
	class BuilderImpl : public Detail< TimeType >::template Builder< EmptyBuilder, T >
	{
	public:
		BuilderImpl(T& t) :Detail< TimeType >::template Builder< EmptyBuilder, T >(EmptyBuilder(), t){}
	};

	template< class TimeType >
	typename BuilderImpl< GroupTweener< TimeType >, TimeType >
		build( GroupTweener< TimeType >& tweener )
	{ 
		return BuilderImpl< GroupTweener< TimeType >, TimeType >(tweener);
	}

	template< class TimeType >
	typename BuilderImpl< SquenceTweener< TimeType >, TimeType >
		build( SquenceTweener< TimeType >& tweener )
	{ 
		return BuilderImpl< SquenceTweener< TimeType >, TimeType >(tweener);
	}

}//namespace Tween

#endif // Tween_h__
