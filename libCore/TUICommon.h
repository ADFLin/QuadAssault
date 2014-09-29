#ifndef TUICommon_h__
#define TUICommon_h__

#include "TUICore.h"
#include <algorithm>


enum ButtonState
{
	BS_NORMAL,
	BS_PRESS,
	BS_HIGHLIGHT ,
	BS_NUM_STATE ,
};

#define TVK_LEFT  0x25
#define TVK_UP    0x26
#define TVK_RIGHT 0x27
#define TVK_DOWN  0x28
#define TVK_ENTER 0x0d
#define TVK_CTRL  0x11

template < class Impl , class CoreImpl >
class TButtonUI : public CoreImpl
{
private:
	Impl* _this(){ return static_cast< Impl*>(this); }

public:
	TButtonUI( Vec2i const& pos , Vec2i const& size  , CoreImpl* parent )
		:CoreImpl(  pos , size , parent )
		,m_state( BS_NORMAL ){}

	ButtonState getButtonState() const { return m_state; }
	///////// override function ////////
public:
	void onClick(){}
	void onChangeState( ButtonState state ){}
	///////////////////////////////////
protected:
	void mouseOverlap( bool beInside );
	bool onMouseMsg( MouseMsg const& msg );

	void setButtonState( ButtonState state );
	ButtonState m_state;
};


template < class Impl , class CoreImpl >
class TPanelUI : public CoreImpl
{
	Impl* _this(){ return static_cast< Impl*>(this); }
public:
	TPanelUI( Vec2i const& pos , Vec2i const& size  , CoreImpl* parent )
		:CoreImpl(  pos , size , parent ){}

};


enum UISide
{
	SIDE_TOP ,
	SIDE_BOTTON ,
	SIDE_LEFT ,
	SIDE_RIGHT ,
};

template < class Impl , class CoreImpl >
class TNoteBookUI : public CoreImpl
{
	Impl* _this(){ return static_cast< Impl*>(this); }

public:
	TNoteBookUI( Vec2i const& pos , Vec2i const& size  , CoreImpl* parent );

	struct NullData {};

	class Page : public TPanelUI< Page , CoreImpl >
	{
	public:
		Page( Vec2i const& pos , Vec2i const& size  , CoreImpl* parent )
			:TPanelUI< Page , CoreImpl >( pos , size , parent ){}
	};

	class PageButton  : public TButtonUI< PageButton , CoreImpl >
	{
	public:
		PageButton( int _pID , Page* _page, Vec2i const& pos , Vec2i const& size , CoreImpl* parent )
			:TButtonUI< PageButton , CoreImpl >( pos , size , parent )
		{
			pID  = _pID;
			page = _page;
		}

		TNoteBookUI* getBook(){ return static_cast< TNoteBookUI* >( getParent() ); }
		void onClick(){  getBook()->changePage( pID );  }
		void render() {  getBook()->_this()->doRenderButton( this ); }

		std::string title;
		int   pID;
		Page* page;
	};
	///////// override function ////////
	void doRenderButton( PageButton* button ){}
	void doRenderPage( Page* page ){}
	void doRenderBackground(){}

	Vec2i getButtonSize(){ return Vec2i(0,0); }
	Vec2i getButtonOffset(){ return Vec2i(0,0); }
	Vec2i getPagePos(){ return Vec2i(0,0); }
	Vec2i getBaseButtonPos(){ return Vec2i(0,0); }

	void onChangePage( int pID ){}

	Vec2i  calcPageButtonPos( int pID )
	{
		return _this()->getBaseButtonPos() + pID * _this()->getButtonOffset();
	}
	////////////////////////////////////////////////////////////////

	void   changePage( int pID );
	Page*  addPage( char const* title );
	void   updatePageButtonPos();

	Vec2i const& getPageSize(){ return mPageSize; }
	Page*  getCurPage()        { return curPageButton->page; }

protected:
	//virtual 
	void   render();

	Vec2i      mPageSize;
	PageButton* curPageButton;
	typedef std::vector< PageButton* > ButtonVec;
	ButtonVec   m_PBVec;
};

template < class Impl , class CoreImpl >
class TSliderUI : public CoreImpl
{
	Impl* _this(){ return static_cast< Impl*>(this); }

public:
	TSliderUI( Vec2i const& pos , int length , Vec2i const& sizeTip  ,
		bool beH  , int minRange , int maxRange , CoreImpl* parent )
		:CoreImpl(  pos , ( beH ) ? Vec2i( length , sizeTip.y ) : Vec2i( sizeTip.x , length ) , parent )
		,m_minRange( minRange )
		,m_maxRange( maxRange )
		,m_beHorizontal( beH )
	{
		m_tipUI = new TipUI( sizeTip , this );
	}

	class TipUI : public CoreImpl
	{
	public:
		TipUI( Vec2i const& size , CoreImpl* parent )
			:CoreImpl( Vec2i(0,0) , size , parent ){}
		bool onMouseMsg( MouseMsg const& msg );
		TSliderUI* getSlider(){ return  static_cast< TSliderUI*>( getParent() ); }
	};


	///////// override function ////////
	void onScrollChange( int value ){}
	void doRenderTip( TipUI* ui ){}
	void doRenderBackground(){}
	/////////////////////////////////////////

	void  updateValue();

	void  correctTipPos( Vec2i& pos );

	int   getValue() const { return m_curValue; }
	int   getMaxValue() const { return m_maxRange; }
	int   getMinValue() const { return m_minRange; }

	void  setValue( int val )
	{
		m_curValue = std::max( m_minRange , std::min( val , m_maxRange ) );
		updateTipPos();
	}

	void setRange( int min , int max )
	{
		m_minRange = min;
		m_maxRange = max;
	}

protected:
	void onRender()
	{
		_this()->doRenderBackground();
		_this()->doRenderTip( m_tipUI );
	}

	void  updateTipPos();

	int    m_curValue;
	int    m_minRange;
	int    m_maxRange;
	TipUI* m_tipUI;
	bool   m_beHorizontal;
};

template < class Impl , class CoreImpl >
class TTextCtrlUI : public CoreImpl
{
private:
	Impl* _this(){ return static_cast< Impl*>(this); }

public:
	TTextCtrlUI( Vec2i const& pos , Vec2i const& size  , CoreImpl* parent )
		:CoreImpl(  pos , size , parent ){ mKeyInPos = 0; }

	char const* getValue() const { return mValue.c_str(); }
	void clearValue(){ mValue.clear(); mKeyInPos = 0; }
	void setValue( char const* str )
	{ 
		mValue = str; 
		if ( mKeyInPos > (int)mValue.size() )
			mKeyInPos = (int)mValue.size();
	}

	///////// override function ////////
public:
	void onPressEnter(){}
	void onTextChange(){}
	/////////////////////////////////////

protected:

	bool isDoubleChar( int pos );

	//virtual 
	bool onKeyMsg( char c , bool beDown );
	//virtual 
	bool onCharMsg( char c );

	int mKeyInPos;
	std::string mValue;

};


template < class Impl, class CoreImpl >
class TItemOwnerUI : public CoreImpl
{
private:
	Impl* _this(){ return static_cast< Impl*>(this); }

public:
	TItemOwnerUI( Vec2i const& pos , Vec2i const& size, CoreImpl* parent )
		:CoreImpl( pos , size , parent )
	{
		mCurSelect   = -1;
	}

	unsigned appendItem( char const* str )
	{ 
		mItemList.push_back( Item( str ) );
		return unsigned( mItemList.size() - 1 ); 
	}
	void     removeItem( char const* str );
	void     removeItem( unsigned pos )
	{
		if ( pos >= mItemList.size() )
			return;

		mItemList.erase( mItemList.begin() + pos );
		if ( mCurSelect == (int)pos )
			mCurSelect = -1;
		else if ( mCurSelect > (int)pos )
			--mCurSelect;
	}
	void     removeAllItem()
	{
		mItemList.clear();
		mCurSelect = -1;
	}
	unsigned getItemNum(){ return mItemList.size(); }
	void*    getSelectedItemData()
	{ 
		if ( mCurSelect != -1 ) 
			return getItemData( mCurSelect );
		return NULL;
	}

	int      getSelection(){ return mCurSelect; }
	void     setSelection( unsigned select )
	{ 
		assert( select < mItemList.size() );
		mCurSelect = select; 
	}

	int     findItem( char const* value )
	{
		for( int i = 0 ; i < (int)mItemList.size() ; ++i )
		{
			if ( mItemList[i].value == value )
				return i;
		}
		return -1;
	}

	void    setItemData( unsigned pos , void* data )
	{
		if ( pos >= mItemList.size() )
			return;
		mItemList[pos].userData = data;
	}
	void*   getItemData( unsigned pos )
	{
		return mItemList[pos].userData;
	}

	void    insertValue( unsigned pos , char const* str )
	{
		mItemList.insert( mItemList.begin() + pos , Item(str) );
	}

	char const* getSelectValue()
	{ 
		if ( mCurSelect == -1 )
			return NULL;
		return mItemList[ mCurSelect ].value.c_str();
	}

	void   modifySelection( unsigned pos )
	{
		setSelection( pos );
		_this()->onItemSelect( pos );
	}

protected:
	///////// override function ////////
	void onItemSelect( unsigned select ){}
protected:
	void tryMoveSelect( bool beNext );
	bool onKeyMsg( char key , bool beDown );

	struct Item
	{
		Item( char const* val )
			:value( val ),userData( NULL ){}
		std::string value;
		void*       userData;
	};
	typedef std::vector< Item > ItemVec;
	ItemVec    mItemList;
	int        mCurSelect;
};



template < class Impl, class CoreImpl >
class TChoiceUI : public TItemOwnerUI< Impl , CoreImpl >
{
private:
	Impl* _this(){ return static_cast< Impl*>(this); }

public:

	TChoiceUI( Vec2i const& pos , Vec2i const& size, CoreImpl* parent )
		:TItemOwnerUI< Impl , CoreImpl >( pos , size , parent )
	{
		mLightSelect = -1;
	}

	class Menu : public CoreImpl
	{
	public:
		Menu( Vec2i const& pos , Vec2i const& size, TChoiceUI* owner )
			:CoreImpl( pos , size , NULL )
			,mOwner( owner )
		{

		}

		virtual void focus( bool beF )
		{
			CoreImpl::focus( beF ); 
			if (!beF) 
				_getOwner()->destroyMenu( this );
		}
		TChoiceUI* _getOwner(){ return mOwner; }
		virtual void render(){  _getOwner()->_onRenderMenu( this );  }
		virtual bool onMouseMsg( MouseMsg const& msg ){  return _getOwner()->_onMenuMouseMsg( this , msg ); }

	private:
		TChoiceUI* mOwner;
	};

	///////// override function ////////
	void onItemSelect( unsigned select ){}
	void doRenderText( Vec2i const& pos , char const* str , bool beLight ){}
	void doRenderMenuBG( Menu* menu ){}
	int  getMenuItemHeight(){ return 15; }
	/////////////////////////////////////////////////////////////////

	bool onMouseMsg( MouseMsg const& msg );
protected:
	void destroyMenu( Menu* menu )
	{
		menu->destroy();
		_removeFlag( UF_HITTEST_CHILDREN );
	}
	void _onRenderMenu( Menu* menu );
	bool _onMenuMouseMsg( Menu* menu , MouseMsg const& msg );

	int        mLightSelect;
};


template < class Impl, class CoreImpl >
class TListCtrlUI : public TItemOwnerUI< Impl , CoreImpl >
{
private:
	Impl* _this(){ return static_cast< Impl*>(this); }
public:
	TListCtrlUI( Vec2i const& pos , Vec2i const& size, CoreImpl* parent )
		:TItemOwnerUI< Impl , CoreImpl >( pos , size , parent )
	{
		mIndexShowStart = 0;
	}

	void  ensureVisible( unsigned pos )
	{
		mIndexShowStart = std::min( pos , mItemList.size() - 1 );
	}
public:
	///////// override function ////////
	void onItemSelect( unsigned pos ){}
	void onItemLDClick( unsigned pos ){}
	void doRenderText( Vec2i const& pos , char const* str , bool beSelected ){}
	void doRenderBackground( Vec2i const& pos , Vec2i const& size ){}
	int  getItemHeight(){ return 15; }
	/////////////////////////////////////
protected:
	void onRender();
	bool onMouseMsg( MouseMsg const& msg );

	int  mIndexShowStart;
};

template < class CoreImpl >
class UIPackage
{
public:
	typedef CoreImpl               Widget;
	typedef TUICore< CoreImpl >    Core;
	typedef TUIManager< CoreImpl > Manager;  

	template < class Impl >
	class Slider : public TSliderUI< Impl , CoreImpl >
	{
	public:
		Slider( Vec2i const& pos , int length , Vec2i const& sizeTip  ,
			bool beH  , int minRange , int maxRange , CoreImpl* parent )
			:TSliderUI< Impl , CoreImpl >( pos , length , sizeTip  , beH  , minRange , maxRange ,  parent ){}
	};

#define DEFINE_UI_CLASS( Class , BaseClass )\
	template < class Impl >\
	class Class : public BaseClass< Impl , CoreImpl >\
	{\
	public:\
		Class( Vec2i const& pos , Vec2i const& size  , CoreImpl* node )\
			: BaseClass < Impl , CoreImpl >( pos ,size ,node ){}\
	};


	DEFINE_UI_CLASS( Panel    , TPanelUI )
	DEFINE_UI_CLASS( Button   , TButtonUI )
	DEFINE_UI_CLASS( TextCtrl , TTextCtrlUI )
	DEFINE_UI_CLASS( Choice   , TChoiceUI   )
	DEFINE_UI_CLASS( NoteBook , TNoteBookUI )
	DEFINE_UI_CLASS( ListCtrl , TListCtrlUI )

#undef DEFINE_UI_CLASS

	template < class T >
	static T castFast( CoreImpl* ui )
	{
#if _DEBUG
		return dynamic_cast< T >( ui );
#else
		return static_cast< T >( ui );
#endif
	}


};

#include "TUICommon.hpp"

#endif // TUICommon_h__

