#ifndef UICore_h__
#define UICore_h__

#include <cstdlib>
#include <cassert>
#include <vector>

#include "SysMsg.h"
//#include "ProfileSystem.h"
#define PROFILE_ENTRY(...)


#ifndef BIT
#define BIT( n ) ( 1 << ( n ) )
#endif 

#define UI_NO_PARENT (void*)(~0)

enum UIFlag
{
	UF_STAY_TOP           = BIT(1) ,
	UF_WORLD_POS_VAILD    = BIT(2) ,
	UF_DISABLE            = BIT(3) ,
	UF_BE_HIDDEN          = BIT(4) ,
	UF_HITTEST_CHILDREN   = BIT(5) ,
	UF_BLOCK_DESTROY      = BIT(6) ,
	UF_PARENT_MOUSE_EVENT = BIT(7) ,
	UF_MARK_DESTROY       = BIT(8) ,
};

#ifdef max
	#undef max
#endif
#ifdef min
	#undef min
#endif
class TRect
{
public:
	TRect( Vec2i const& _pos , Vec2i const& _size )
		:min( _pos )
		,max( _pos + _size )
	{

	}

	Vec2i getSize(){ return max - min; }
	Vec2i max;
	Vec2i min;
};

inline
bool testPointInRect( Vec2i const& pos , TRect const& rect )
{
	return  rect.min.x <= pos.x && pos.x <= rect.max.x &&
		    rect.min.y <= pos.y && pos.y <= rect.max.y ;
}


template< class T >
class TUIManager;

template< class T >
class NodeT
{




};
template< class T >
class Root
{

	NodeT< T >* mChildren;
};


template< class T >
class TUILink
{
protected:
	T*      getPrev(){ return mPrev; }
	T*      getNext(){ return mNext; }
	T*      linkChild( T* ui )
	{
		if ( mChild )
		{
			TUICore* lastUI = mChild->mPrev;
			lastUI->mNext = ui;
			ui->mPrev = lastUI;
		}
		else
		{
			mChild = ui;
		}
		mChild->mPrev = ui;
		ui->mNext = NULL;
	}
	void   unlinkChild( T* ui )
	{

	}
protected:
	T*      mChild;
	T*      mNext;
	T*      mPrev;
};

template< class T >
class TUICore
{
	T* _this(){ return static_cast< T* >( this );  }
public:
	TUICore( Vec2i const& pos , Vec2i const& size , T* parent );
	virtual ~TUICore();

	T*             getChild(){ return static_cast< T*>( mChild );  }
	T*             nextChild( TUICore* prev ){ assert( prev ); return static_cast< T*>( prev->mNext ); }
	
	TUICore*       getParent(){ return mParent; }
	TUIManager<T>* getManager(){ return mManager; }
	int            getChildrenNum(){ return mNumChild; }
	int            getLevel();
	int            getOrder();

	Vec2i const&  getWorldPos();
	Vec2i const&  getPos() const { return mBoundRect.min; }
	Vec2i         getSize(){ return mBoundRect.max - mBoundRect.min; }
	TRect const&  getBoundRect() const { return mBoundRect; }



	bool           isFocus();
	bool           isEnable() const { return !checkFlag( UF_DISABLE ); }
	bool           isShow() const { return !checkFlag( UF_BE_HIDDEN ); }
	bool           isTopUI();

	T&             setPos( Vec2i const& pos );
	T&             setSize( Vec2i const& size ){  mBoundRect.max = mBoundRect.min + size;  return *_this(); }
	T&             setTop( bool beAlways = false );
	T&             setFocus();
	T&             show( bool beS );
	T&             enable( bool beE = true );

	T&             addChild( TUICore* ui );
	void           destroy(){  assert( getManager() ); getManager()->destroyUI( this );  }

public:
	void          _unlinkInternal();
private:
	void          linkChildInternal( TUICore* ui );

protected:
	void    onLink(){}
	void    onUnlink(){}
	void    onChangeOrder(){}
	void    onChangeChildrenOrder(){}
	void    onEnable( bool beE ){}
	void    onUpdateUI(){}
	void    onChangePos( Vec2i const& pos , bool beLocal ){}
	void    onShow( bool beS ){}
	void    onMouse( bool beIn ){}
	void    onRender(){}
	void    onPrevRender(){}
	void    onPostRender(){}
	void    onPostRenderChildren(){}
	void    onFocus( bool beF ){}
	void    onResize( Vec2i const& size ){}
	bool    doHitTest( Vec2i const& pos ){ return testPointInRect( pos , mBoundRect ); }
	bool    doClipTest(){ return true; }
	
public:

	virtual void  deleteThis(){ delete this; }
	virtual bool  onKeyMsg( char key , bool beDown ){ (void)key; (void)beDown; return true; }
	virtual bool  onCharMsg( char c ){ (void)c; return true; }
	virtual bool  onMouseMsg( MouseMsg const& msg){ (void)msg; return false; }

private:


	void          setTopChild( TUICore* ui , bool beAlways );
	void          _destroyChildren();

	void          setFlag( unsigned flag ){	mFlag = flag;	}
	unsigned      getFlag(){ return mFlag; }

	bool          checkFlag( unsigned flag ) const { return ( mFlag & flag ) != 0; }

	void          addChildFlag( unsigned flag );
	void          removeChildFlag( unsigned flag );

	TUICore*      getPrev(){ return mPrev; }
	TUICore*      getNext(){ return mNext; }
	void          setManager( TUIManager<T>* mgr );

	TUICore*      hitTestChildren( Vec2i const& testPos );
	bool          clipTest(){ return _this()->doClipTest(); }


protected:
	void          skipMouseMsg(){ _addFlag( UF_PARENT_MOUSE_EVENT ); }
	void          lockDestroy()   { _addFlag( UF_BLOCK_DESTROY );  }
	void          unlockDestroy() { _removeFlag( UF_BLOCK_DESTROY ); }



	void          _addFlag( unsigned flag ){ mFlag |= flag; }
	void          _removeFlag( unsigned flag ){ mFlag &= ~flag; }

	virtual void  mouseOverlap( bool beOverlap ){  _this()->onMouse( beOverlap ); }
	virtual void  render(){  _this()->onRender();  }
	virtual void  focus( bool beF ){ _this()->onFocus( beF); }
	void    update()    {  _this()->onUpdateUI();   }

private:

	void    prevRender()        {  _this()->onPrevRender();  }
	void    postRender()        {  _this()->onPostRender();  }
	void    postRenderChildren(){  _this()->onPostRenderChildren(); }
	
private:
	TUICore();

	void          init();
	friend class TUIManager< T >;

protected:

	TUIManager<T>* mManager;
	TUICore*       mParent;
	TUICore*       mChild;

	TUICore*       mNext;
	TUICore*       mPrev;

	Vec2i          mCacheWorldPos;
	unsigned       mFlag;
	int            mNumChild;
	TRect          mBoundRect;
};


template < class T >
class TUIManager
{
public:
	TUIManager();
	void      addUI( TUICore<T>* ui );

	TUICore<T>*  getRoot(){ return &mRoot; }
	
	void      update();
	void      render();

	void      destroyUI( TUICore<T>* ui );
	void      cleanupUI();

	bool      isTopUI( TUICore<T>* ui );
	MouseMsg& getLastMouseMsg(){ return mMouseMsg; }

	T*        hitTest( Vec2i const& testPos );

	T*        getModalUI()       { return static_cast< T* >( mUIModal ); }
	T*        getFocusUI()       { return static_cast< T* >( mUIFocus ); }
	T*        getLastMouseMsgUI(){ return static_cast< T* >( mUILastMouseMsg ); }
	T*        getMouseUI()       { return static_cast< T* >( mUIMouse ); }

	void      startModal( TUICore<T>* ui );
	void      endModal( TUICore<T>* ui );

	bool      procCharMsg( char c );
	bool      procMouseMsg( MouseMsg const& msg );
	bool      procKeyMsg( char key , bool beDown );

	void      setFocusUI( TUICore<T>* ui );

	void      captureMouse( TUICore<T>* ui){ mUICapture = ui;}
	void      releaseMouse(){ mUICapture = NULL; }

	void      cleanupDestroyUI();

	template< class Visitor >
	void      visitUI( Visitor visitor );

protected:
	void        destroyNoCheck( TUICore<T>* ui );
	void        removeRefUI( TUICore<T>* ui );
	void        updateInternal( TUICore<T>* ui );
	void        renderInternal( TUICore<T>* ui );
	template< class Visitor >
	void        visitUIInternal( Visitor visitor , TUICore<T>* ui );

	TUICore<T>*    mUILastMouseMsg;
	TUICore<T>*    mUICapture;
	TUICore<T>*    mUIModal;
	TUICore<T>*    mUIFocus;
	TUICore<T>*    mUIMouse;

	bool         mProcessingMsg;
	MouseMsg     mMouseMsg;

	TUICore<T>   mRemoveUI;
	TUICore<T>   mRoot;

	friend class TUICore<T>;
private:
	TUIManager( TUIManager const& ){}
};


#include "TUICore.hpp"

#endif // UICore_h__
