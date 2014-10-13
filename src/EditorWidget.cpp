#include "EditorWidget.h"

#include "GameInterface.h"
#include "RenderSystem.h"
#include "Block.h"

#include "FixString.h"


PorpTextCtrl::PorpTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:GTextCtrl( id , pos , size , parent )
{
	text->setCharSize( 20 );
	text->setFont( getGame()->getFont(0) );
	text->setColor( Color( 255 , 255 , 0 ) );
}

PorpTextCtrl::~PorpTextCtrl()
{

}

void PorpTextCtrl::inputData()
{
	FString str;
	if ( mPorpData.getString( str ) )
		setValue( str );
}

void PorpTextCtrl::outputData()
{
	mPorpData.setValue( getValue() );
}

IntPropChioce::IntPropChioce( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:BaseClass( id , pos , size , parent )
{
	mDataSize = 0;
	mData = NULL;

}

void IntPropChioce::init( int numSet , int valueSet[] , char const* strSet[] )
{
	for( int i = 0 ; i < numSet ; ++i )
	{
		unsigned pos = appendItem( strSet[i] );
		setItemData( pos , (void*)valueSet[i] );
	}
}

void IntPropChioce::inputData()
{
	if ( !mData )
		return;

	int value;
	switch( mDataSize )
	{
	case 1: value = *((char*)mData);  break;
	case 2: value = *((short*)mData); break;
	case 4: value = *((int*)mData);   break;
	}
	for( int i = 0 ; i < getItemNum() ; ++i )
	{
		if ( value == (int)getItemData( i ) )
		{
			setSelection( i );
			break;
		}
	}
}

void IntPropChioce::outputData()
{
	if ( !mData )
		return;

	int pos = getSelection();
	if ( pos == -1 )
		return;

	int  value = (int)getItemData( pos );
	switch( mDataSize )
	{
	case 1: *((char*)mData)  = value; break;
	case 2: *((short*)mData) = value; break;
	case 4: *((int*)mData)   = value; break;
	}
}

PropFrame::PropFrame( int id , Vec2i const& pos , GWidget* parent ) 
	:BaseClass( id , pos , Vec2f( 250 , 400 ) , parent )
{
	mEditObj = NULL;
}

void PropFrame::addPorpWidget( char const* name , GWidget* widget )
{
	PropData data;
	data.widget = widget;
	data.name   = IText::create( getGame()->getFont(0) , 20 , Color( 0 , 0 , 255 ) );
	data.name->setString( name );

	mPorps.push_back( data );
}

bool PropFrame::onChildEvent( int event , int id , GWidget* ui )
{
	switch( id )
	{
	case UI_INT_PROP_CHIOCE:
		if ( event == EVT_CHOICE_SELECT )
		{
			ui->outputData();
			if ( mEditObj )
				mEditObj->updateEdit();
		}
		break;
	case UI_PROP_TEXTCTRL:
		if ( event == EVT_TEXTCTRL_ENTER )
		{
			ui->outputData();
			if ( mEditObj )
				mEditObj->updateEdit();
		}
		break;
	}
	return false;
}

void PropFrame::changeEdit( IEditable& obj )
{
	cleanupAllPorp();
	mEditObj = &obj;
	mEditObj->enumProp( *this );
	inputData();
}

void PropFrame::cleanupAllPorp()
{
	for( PropDataVec::iterator iter= mPorps.begin() , itEnd = mPorps.end();
		iter != itEnd ; ++iter )
	{
		PropData& data = *iter;
		data.widget->destroy();
		data.name->release();
	}
	mPorps.clear();
}

void PropFrame::onRender()
{
	BaseClass::onRender();

	Vec2i pos = getWorldPos();
	int i = 0;
	for( PropDataVec::iterator iter= mPorps.begin() , itEnd = mPorps.end();
		iter != itEnd ; ++iter )
	{
		PropData& data = *iter;

		getRenderSystem()->drawText( data.name , 
			pos + Vec2i( 5 , TopSideHeight + 5 + i * ( getWidgetSize().y + 5 ) + getWidgetSize().y / 2 ) , 
			TEXT_SIDE_LEFT );

		++i;
	}
}

void PropFrame::inputData()
{
	for( PropDataVec::iterator iter= mPorps.begin() , itEnd = mPorps.end();
		iter != itEnd ; ++iter )
	{
		PropData& data = *iter;
		data.widget->inputData();
	}
}

void PropFrame::outputData()
{
	for( PropDataVec::iterator iter= mPorps.begin() , itEnd = mPorps.end();
		iter != itEnd ; ++iter )
	{
		PropData& data = *iter;
		data.widget->outputData();
	}
}

Vec2i PropFrame::calcWidgetPos()
{
	int x = getSize().x - ( getWidgetSize().x + 5 );
	int y = TopSideHeight + 5 + mPorps.size() * ( getWidgetSize().y + 5 );
	return Vec2i( x , y );
}

void PropFrame::addProp( char const* name , void* value , int sizeValue , int numSet , int valueSet[] , char const* strSet[] )
{
	IntPropChioce* chioce = new IntPropChioce( UI_INT_PROP_CHIOCE , calcWidgetPos() , getWidgetSize() , this );
	chioce->init( numSet , valueSet , strSet );
	chioce->setData( value , sizeValue );
	addPorpWidget( name , chioce );
}

void PropFrame::addProp( char const* name , Vec2f& value )
{
	FixString< 128 > fullName;
	fullName = name;
	fullName += ".X";
	addProp( fullName , value.x );
	fullName = name;
	fullName += ".Y";
	addProp( fullName , value.y );
}

void PropFrame::addProp( char const* name , Vec3f& value )
{
	FixString< 128 > fullName;
	fullName = name;
	fullName += ".X";
	addProp( fullName , value.x );
	fullName = name;
	fullName += ".Y";
	addProp( fullName , value.y );
	fullName = name;
	fullName += ".Z";
	addProp( fullName , value.z );
}

void PropFrame::addProp( char const* name , bool& value )
{
	int valueSet[] = { 1 , 0 };
	char const* strSet[] = { "True" , "False" };
	addEnumProp( name , value , 2 , valueSet , strSet );
}

void PropFrame::removeEdit()
{
	if ( mEditObj )
	{
		cleanupAllPorp();
		mEditObj = NULL;
	}
}

TileEditFrame::TileEditFrame( int id , Vec2f const& pos , GWidget* parent ) 
	:BaseClass( id , pos , Vec2f(  4 + 2 * ButtonLength + 2 ,  4 + NUM_BLOCK_TYPE * ( ButtonLength + 2 ) + TopSideHeight ) , parent )
{
	char const* blockName[] =
	{
#define BLOCK_INFO( TYPE , NAME ) NAME ,
#include "BlockType.h"
	};

	for( int i = 0; i < NUM_BLOCK_TYPE ; ++i )
	{
		Vec2i pos;
		pos.x = ( i % 2 ) * ( ButtonLength + 2 ) + 2;
		pos.y = ( i / 2 ) * ( ButtonLength + 2 ) + TopSideHeight + 2;
		GImageButton* button = new GImageButton( UI_TILE_SELECT , pos , Vec2i( ButtonLength , ButtonLength ) , this );
		button->texImag = Block::FromType( i )->getTexture( 0 );
		button->setHelpText( blockName[i] );
		button->setUserData( (void*)i );
	}
}

ObjectEditFrame::ObjectEditFrame( int id , Vec2f const& pos , GWidget* parent ) 
	:BaseClass( id , pos , Vec2i( 4 , 4 ) + Vec2i( 0 , 0 ) , parent )
{

	Vec2i posCur = Vec2i( 2 , 2 + TopSideHeight );

	GTextButton* button = new GTextButton( UI_OBJECT_DESTROY , posCur , ButtonSize() , this );
	button->text->setFont( getGame()->getFont(0) );
	button->text->setCharSize( 20 );
	button->text->setString( "Destroy" );
	posCur.y += ButtonSize().y + 2;

	mObjectListCtrl = new GListCtrl( UI_OBJECT_LISTCTRL , posCur , Vec2i(0,0) , this );
}

void ObjectEditFrame::setupObjectList( ObjectCreator& creator )
{
	ObjectFactoryMap& of = creator.getFactoryMap();
	mObjectListCtrl->removeAllItem();

	int num = 0;
	for( ObjectFactoryMap::iterator iter = of.begin() , itEnd = of.end();
		iter != itEnd ; ++iter )
	{

		unsigned idx = mObjectListCtrl->appendItem( iter->first );
		mObjectListCtrl->setItemData( idx , (void*)iter->first );
		++num;
	}


	Vec2i size;
	size.x = ButtonSize().x;
	size.y = num * mObjectListCtrl->getItemHeight();
	mObjectListCtrl->setSize( size );
	
	size.x += 4;
	size.y += TopSideHeight + 4 + ButtonSize().y;
	setSize( size );

}

ActionEditFrame::ActionEditFrame( int id , Vec2i const& pos , GWidget* widget ) 
	:BaseClass( id , pos , Vec2i( ListCtrlWidth + ButtonSize().x + 4 + 3 , 200 ) , widget )
{
	mListCtrl = new GListCtrl( UI_ACTION_LISTCTRL , Vec2i( 2 , TopSideHeight + 2 ) , Vec2i( 100 , 100 ) , this );
	mTrigger = NULL;
}

void ActionEditFrame::setupActionList( ActionCreator& creator )
{
	Vec2i  basePos = Vec2i( ListCtrlWidth + 2 + 3 , TopSideHeight + 2 );
	ActionFactoryMap& of = creator.getFactoryMap();
	int num = 0;
	for( ActionFactoryMap::iterator iter = of.begin() , itEnd = of.end();
		iter != itEnd ; ++iter )
	{
		Vec2i pos = basePos;
		pos.y += num  * ( ButtonSize().y + 2 );
		GTextButton* button = new GTextButton( UI_ACTION_SELECT , pos , ButtonSize() , this );
		button->text->setFont( getGame()->getFont(0) );
		button->text->setCharSize( 20 );
		button->text->setString( iter->first );
		button->setUserData( (void*)iter->second );
		++num;
	}
}

void ActionEditFrame::refreshList()
{
	mListCtrl->removeAllItem();
	if ( mTrigger )
	{
		ActionList& actions = mTrigger->getActions();
		for( ActionList::iterator iter = actions.begin() ,itEnd = actions.end();
			iter != itEnd; ++iter )
		{
			Action* act = *iter;
			unsigned idx = mListCtrl->appendItem( act->getName() );
			mListCtrl->setItemData( idx , (void*) act );
		}
	}
}

void ActionEditFrame::setTrigger( TriggerBase* trigger )
{
	mTrigger = trigger;
	refreshList();
}
