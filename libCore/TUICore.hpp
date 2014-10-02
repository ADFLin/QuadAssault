#ifndef TUICore_hpp__
#define TUICore_hpp__

#include "TUICore.h"
#include <cassert>


template< class T >
TUICore<T>::TUICore( Vec2i const& pos , Vec2i const& size , T* parent ) 
	:mBoundRect( pos , size )
{
	init();

	if ( parent && parent != UI_NO_PARENT )
		parent->addChild( this );
}


template< class T >
TUICore<T>::TUICore()
	:mBoundRect( Vec2i(0,0) , Vec2i(0,0))
{
	init();
}


template< class T >
void TUICore< T >::init()
{
	mParent = NULL;
	mChild  = NULL;
	mNext   = NULL;
	mPrev   = NULL;
	mNumChild = 0;
	mManager = NULL;
	mFlag    = 0;
}


template< class T >
TUICore<T>::~TUICore()
{
	if ( mParent )
		_unlinkInternal();
}


template< class T >
TUICore<T>* TUICore<T>::hitTestChildren( Vec2i const& testPos )
{
	if ( !mChild )
		return NULL;

	Vec2i  pos = testPos;

	TUICore* result = NULL;
	TUICore* child = mChild;
	TUICore* ui = child->mPrev;

	while( 1 )
	{
		bool testFail = true;
		if ( !ui->checkFlag( UF_DISABLE | UF_BE_HIDDEN ) )
	    //if ( ui->isEnable() && ui->isShow() )
		{
			if ( ui->checkFlag( UF_HITTEST_CHILDREN ) )
			{
				Vec2i tPos = pos - ui->getPos();
				result = ui->hitTestChildren( tPos );
				if ( result )
					return result;
			}
			else if ( static_cast< T* >( ui )->doHitTest( pos ) )
			{
				result = ui;
				child = ui->mChild;

				if ( !child  )
					return result;

				pos -= ui->getPos();
				ui = child->mPrev;

				testFail = false;
			}
		}

		if ( testFail )
		{
			if ( ui == child )
				return result;

			ui = ui->mPrev;
		}
	}

	return result;
}


template< class T >
void TUICore<T>::_unlinkInternal()
{
	assert( mParent );

	if ( mParent->mChild == this )
	{
		mParent->mChild = mNext;

		if ( mNext )
			mNext->mPrev = mPrev;
	}
	else
	{
		mPrev->mNext = mNext;

		if ( mNext )
			mNext->mPrev = mPrev;
		else
			mParent->mChild->mPrev = mPrev;
	}

	--mParent->mNumChild;

	mParent = NULL;
	mNext = NULL;
	mPrev = NULL;
	

	if ( mParent != getManager()->getRoot() )
		static_cast< T* >( mParent )->onChangeChildrenOrder();

	mManager = NULL;
}


template< class T >
void TUICore<T>::linkChildInternal( TUICore* ui )
{
	assert( ui->mParent == NULL );

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
	ui->mParent = this;

	if ( mManager )
		ui->setManager( mManager );

	++mNumChild;
}
template< class T >
T& TUICore<T>::addChild( TUICore* ui )
{
	if ( ui->mParent )
		ui->_unlinkInternal();

	linkChildInternal( ui );

	ui->removeChildFlag( UF_WORLD_POS_VAILD );

	static_cast< T* >( ui )->onLink();

	return *_this();
}

template< class T >
bool TUICore<T>::isFocus()
{
	assert( getManager() );
	return getManager()->getFocusUI() == this;
}

template< class T >
T&  TUICore<T>::show( bool beS )
{
	if ( isShow() != beS )
	{
		if ( beS )
			_removeFlag( UF_BE_HIDDEN );
		else
			_addFlag( UF_BE_HIDDEN );

		_this()->onShow( beS );

		TUICore* ui = mChild;
		while ( ui )
		{
			ui->show( beS );
			ui = ui->mNext;
		}
	}
	return *_this();
}

template< class T >
T&  TUICore<T>::enable( bool beE = true )
{ 
	if ( isEnable() != beE )
	{
		if ( beE )
			_removeFlag( UF_DISABLE );
		else
			_addFlag( UF_DISABLE );

		_this()->onEnable( beE );
	}
	return *_this();
}


template< class T >
int TUICore<T>::getLevel()
{
	TUICore* ui = mParent;
	int result = 0;
	while ( ui ){ ++result; ui = ui->mParent; }
	//have not linked manger
	if ( !mManager && mParent )
		++result;

	return result;
}

template< class T >
int TUICore<T>::getOrder()
{
	assert( mParent );

	TUICore* ui = mParent->mChild;
	int count = 0;
	while ( 1 )
	{
		if ( ui == this )
			break;
		++count;
		ui = ui->mNext;
	}
	return count;
}


template< class T >
Vec2i const& TUICore<T>::getWorldPos()
{
	if ( !checkFlag( UF_WORLD_POS_VAILD ) )
	{
		mCacheWorldPos = getPos();

		if ( mParent && mParent != getManager()->getRoot() )
			mCacheWorldPos += mParent->getWorldPos();

		_addFlag( UF_WORLD_POS_VAILD );
	}
	return mCacheWorldPos;
}

template< class T >
T& TUICore<T>::setPos( Vec2i const& pos )
{
	Vec2i offset = pos - mBoundRect.min;

	mBoundRect.min = pos;
	mBoundRect.max += offset;

	_removeFlag( UF_WORLD_POS_VAILD );
	removeChildFlag( UF_WORLD_POS_VAILD );
	_this()->onChangePos( pos , true );

	return *_this();
}

template< class T >
void TUICore<T>::setManager( TUIManager<T>* mgr )
{
	mManager = mgr;
	TUICore* ui = getChild();
	while ( ui )
	{
		ui->setManager( mgr );
		ui = nextChild( ui );
	}
}

template< class T >
void TUICore<T>::setTopChild( TUICore* ui , bool beAlways )
{
	assert( ui->mNext );

	if ( mChild == ui )
	{
		mChild = ui->mNext;
		mChild->mPrev = ui->mPrev;
	}
	else
	{
		ui->mPrev->mNext = ui->mNext;
		ui->mNext->mPrev = ui->mPrev;
	}

	assert( mChild );

	TUICore* lastUI = mChild->mPrev;
	assert( lastUI );

	lastUI->mNext = ui;
	ui->mPrev = lastUI;
	ui->mNext = NULL;

	mChild->mPrev = ui;

	if ( getManager()->getRoot() != this )
		_this()->onChangeChildrenOrder();
}

template< class T >
T& TUICore<T>::setTop( bool beAlways )
{
	if ( beAlways )
		_addFlag( UF_STAY_TOP );

	if ( mNext )
	{
		getParent()->setTopChild( this , beAlways );
		_this()->onChangeOrder();
	}

	return *_this();
}

template< class T >
bool TUICore<T>::isTopUI()
{
	return getManager()->isTopUI( this );
}

template< class T >
void TUICore<T>::addChildFlag( unsigned flag )
{
	TUICore* ui = getChild();
	while( ui )
	{
		ui->_addFlag( flag );
		ui->addChildFlag( flag );
		ui = nextChild( ui );
	}
}

template< class T >
void TUICore<T>::removeChildFlag( unsigned flag )
{
	TUICore* ui = getChild();
	while( ui )
	{
		ui->_removeFlag( flag );
		ui->removeChildFlag( flag );
		ui = nextChild( ui );
	}
}


template< class T >
T& TUICore<T>::setFocus()
{
	getManager()->setFocusUI( this );
	return *_this();
}

template< class T >
void TUICore<T>::_destroyChildren()
{
	TUICore* ui = mChild;
	while( ui )
	{
		ui->_destroyChildren();
		getManager()->removeRefUI( ui );

		TUICore* next = ui->mNext;
		delete ui;
		ui = next;
	}
}

template< class T >
TUIManager<T>::TUIManager()
	:mUIFocus( NULL )
	,mUIMouse( NULL )
	,mUIModal( NULL )
	,mUICapture( NULL )
	,mUILastMouseMsg( NULL )
	,mRoot()
	,mRemoveUI()
	,mProcessingMsg( false )
{
	mRoot.setManager( this );
	mRemoveUI.setManager( this );
}


template< class T >
bool TUIManager<T>::procMouseMsg( MouseMsg const& msg )
{
	PROFILE_ENTRY("UI System");

	bool result = true;

	mProcessingMsg = true;

	mMouseMsg = msg;

	TUICore<T>* ui;
	if ( mUICapture )
	{
		ui = mUICapture;
	}
	else if ( mUIModal )
	{
		if (  testPointInRect( mMouseMsg.getPos() , mUIModal->mBoundRect ) )
			ui = mUIModal->hitTestChildren( mMouseMsg.getPos() - mUIModal->getPos() );
		else
			ui = NULL;
	}
	else
	{
		ui = hitTest( mMouseMsg.getPos() );
	}

	mUILastMouseMsg = ui;

	{
		PROFILE_ENTRY("Msg Process");

		if (  msg.onLeftDown() )
		{
			if ( mUIFocus != ui )
			{
				if ( mUIFocus )
					mUIFocus->focus( false );

				mUIFocus = ui;

				if ( ui )
					ui->focus( true );
			}
		}
		else if ( msg.onMoving() )
		{
			if ( mUIMouse != ui )
			{
				if ( mUIMouse )
					mUIMouse->mouseOverlap( false );

				mUIMouse = ui;

				if ( ui )
					ui->mouseOverlap( true );
			}
		}


		if ( ui )
		{
			if ( msg.isDraging() && ui != mUIFocus )
			{
				result = ( mUIFocus == NULL ) ;
			}
			else
			{
				result = ui->onMouseMsg( mMouseMsg );
				while ( result && ui->checkFlag( UF_PARENT_MOUSE_EVENT ) )
				{
					if ( ui->getParent() == &mRoot )
						break;
					ui->_removeFlag( UF_PARENT_MOUSE_EVENT );

					ui = static_cast< T* >( ui->getParent() );
					result = ui->onMouseMsg( mMouseMsg );
					mUILastMouseMsg = ui;
				}
			}

			//while ( bool still = ui->onMouseMsg( mMouseMsg ) )
			//{
			//	ui = ui->getParent();
			//	if ( !still || ui == getRoot() )
			//		return still;
			//}
		}
	}

	mProcessingMsg = false;

	cleanupDestroyUI();
	return result;
}

template< class T >
bool TUIManager<T>::procCharMsg( char c )
{
	bool result = true;
	mProcessingMsg = true;

	if ( mUIFocus )
	{
		result = mUIFocus->isEnable() ? mUIFocus->onCharMsg( c ) : true;
	}
	else if ( mUIModal )
	{
		result = mUIModal->isEnable() ? mUIModal->onCharMsg( c ) : true;
	}
	mProcessingMsg = false;
	return result;
}

template< class T >
bool TUIManager<T>::procKeyMsg( char key , bool beDown )
{
	bool result = true;
	mProcessingMsg = true;

	if ( mUIFocus )
	{
		result = mUIFocus->isEnable() ? mUIFocus->onKeyMsg( key , beDown ) : true;
	}
	else if ( mUIModal )
	{
		result = mUIModal->isEnable() ? mUIModal->onKeyMsg( key , beDown ) : true;
	}

	mProcessingMsg = false;
	return result;
}

template< class T >
T* TUIManager<T>::hitTest( Vec2i const& testPos )
{
	PROFILE_ENTRY("UI Hit Test");
	return static_cast< T* >( mRoot.hitTestChildren( testPos ) );
}

template< class T >
void TUIManager<T>::update()
{
	PROFILE_ENTRY("UI System");

	{
		PROFILE_ENTRY("UpdateUI");
		updateInternal( mRoot.mChild );
	}
}

template< class T >
void TUIManager<T>::updateInternal( TUICore<T>* ui )
{
	while ( ui )
	{
		if ( ui->isEnable() )
			ui->update();

		updateInternal( ui->getChild() );
		ui = ui->mNext;
	}
}

template< class T >
void TUIManager<T>::render()
{
	renderInternal( mRoot.mChild );
}

template< class T >
void TUIManager<T>::renderInternal( TUICore<T>* ui )
{
	while( ui )
	{
		if ( ui->isShow() && ui->clipTest() )
		{
			ui->prevRender();
			ui->render();
			ui->postRender();
			renderInternal( ui->getChild() );
			ui->postRenderChildren();
		}
		ui = ui->getNext();
	}
}

template< class T >
template< class Visitor >
void  TUIManager<T>::visitUI( Visitor visitor )
{
	PROFILE_ENTRY("UI System");
	{
		PROFILE_ENTRY("visitUI");
		visitUIInternal( visitor , mRoot.mChild );
	}
}

template< class T >
template< class Visitor >
void  TUIManager<T>::visitUIInternal( Visitor visitor , TUICore<T>* ui )
{
	while ( ui )
	{
		visitor( static_cast< T* >( ui ) );
		visitUIInternal( visitor , ui->getChild() );
		ui = ui->mNext;
	}
}

template< class T >
void TUIManager<T>::removeRefUI( TUICore<T>* ui )
{
	if ( mUICapture == ui ) 
	{
		mUICapture = NULL;
	}
	if ( mUIModal == ui )
	{
		mUIModal = NULL;
	}
	if ( mUIFocus == ui )   
	{
		mUIFocus = NULL;
		ui->focus( false );
	}
	if ( mUIMouse == ui )   
	{
		mUIMouse = NULL;
		ui->mouseOverlap( false );
	}
}

template< class T >
void TUIManager<T>::destroyUI( TUICore<T>* ui )
{
	if ( ui == NULL || ui->checkFlag( UF_MARK_DESTROY ) )
		return;

	ui->_addFlag( UF_MARK_DESTROY );

	if ( mProcessingMsg || ui->checkFlag( UF_BLOCK_DESTROY ))
	{
		TUICore<T>* parent = ui->getParent();
		if ( parent != &mRemoveUI )
		{
			ui->_unlinkInternal();
			mRemoveUI.linkChildInternal( ui );
		}
	}
	else
	{
		destroyNoCheck( ui );
	}
}

template< class T >
void TUIManager<T>::cleanupDestroyUI()
{
	TUICore<T>* ui = mRemoveUI.getChild();
	while ( ui )
	{
		destroyNoCheck( ui );
		ui = mRemoveUI.getChild();
	}
}

template< class T >
void TUIManager<T>::destroyNoCheck( TUICore<T>* ui )
{
	removeRefUI( ui );
	ui->_destroyChildren();
	ui->_unlinkInternal();
	ui->deleteThis();
}

template< class T >
void TUIManager<T>::cleanupUI()
{
	TUICore<T>* child = getRoot()->getChild();
	while ( child )
	{
		destroyUI( child );
		child = getRoot()->getChild();
	}
}

template< class T >
void TUIManager<T>::addUI( TUICore<T>* ui )
{
	assert( ui && ui->getParent() == NULL );
	mRoot.addChild( ui );
}

template< class T >
void TUIManager<T>::startModal( TUICore<T>* ui )
{
	if ( mUIModal )
		return;
	mUIModal = ui;
}

template< class T >
void TUIManager<T>::endModal( TUICore<T>* ui )
{
	if ( ui == mUIModal )
		mUIModal = NULL;
}

template< class T >
bool TUIManager<T>::isTopUI( TUICore<T>* ui )
{
	TUICore<T>* parent = ui->getParent();
	return parent == getRoot() || 
		   parent == &mRemoveUI;
}

template< class T >
void TUIManager<T>::setFocusUI( TUICore<T>* ui )
{
	if ( ui == mUIFocus )
		return;
	if ( mUIFocus )
		mUIFocus->focus( false );

	mUIFocus = ui;
	ui->focus( true );
	
}

#endif // TUICore_hpp__
