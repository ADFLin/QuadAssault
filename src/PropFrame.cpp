#include "PropFrame.h"

#include "GameInterface.h"
#include "RenderSystem.h"

#include "FixString.h"


void PorpTextCtrl::inputData()
{
	FixString< 256 > str;
	switch( mTypeData )
	{
	case PROP_FLOAT: str.format( "%f" , *((float*)mData) ); break;
	case PROP_INT:   str.format( "%d" , *((int*)mData) ); break;
	case PROP_BOOL:  str = *((bool*)mData) ? "true" : "false"; break;
	case PROP_STRING:str = *((string*)mData); break;
	}
	setValue( str );
}

void PorpTextCtrl::outputData()
{
	switch( mTypeData )
	{
	case PROP_FLOAT: *((float*)mData) = ::atof( getValue() );break;
	case PROP_INT:   *((int*)mData) = ::atoi( getValue() );break;
	case PROP_BOOL:  *((bool*)mData) = strcmp( getValue() , "true" ) ? true : false; break;
	case PROP_STRING:*((string*)mData) = getValue();break;
	}
}

PorpTextCtrl::PorpTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent ) 
	:GTextCtrl( id , pos , size , parent )
{
	text->setCharSize( 24 );
	text->setFont( getGame()->getFont(0) );
	text->setColor( Color( 255 , 255 , 0 ) );
}

PropFrame::PropFrame( int id , Vec2i const& pos , GWidget* parent ) 
	:BaseClass( id , pos , Vec2f( 300 , 400 ) , parent )
{

}

void PropFrame::addProp( char const* name , float& value )
{
	PorpTextCtrl* textCtrl = new PorpTextCtrl( UI_PROP_TEXTCTRL , calcWidgetPos() , getWidgetSize() , this );
	textCtrl->setData( value );
	addPorpWidget( name , textCtrl );
}

void PropFrame::addProp( char const* name , int& value )
{
	PorpTextCtrl* textCtrl = new PorpTextCtrl( UI_PROP_TEXTCTRL , calcWidgetPos() , getWidgetSize() , this );
	textCtrl->setData( value );
	addPorpWidget( name , textCtrl );
}

void PropFrame::addProp( char const* name , string& value )
{
	PorpTextCtrl* textCtrl = new PorpTextCtrl( UI_PROP_TEXTCTRL , calcWidgetPos() , getWidgetSize() , this );
	textCtrl->setData( value );
	addPorpWidget( name , textCtrl );
}

void PropFrame::addProp( char const* name , bool& value )
{
	PorpTextCtrl* textCtrl = new PorpTextCtrl( UI_PROP_TEXTCTRL , calcWidgetPos() , getWidgetSize() , this );
	textCtrl->setData( value );
	addPorpWidget( name , textCtrl );
}

void PropFrame::addPorpWidget( char const* name , GWidget* widget )
{
	PropData data;
	data.widget = widget;
	data.name   = IText::create( getGame()->getFont(0) , 22 , Color( 0 , 0 , 255 ) );
	data.name->setString( name );

	mPorps.push_back( data );
}

bool PropFrame::onChildEvent( int event , int id , GWidget* ui )
{
	switch( id )
	{
	case UI_PROP_TEXTCTRL:
		if ( event == EVT_TEXTCTRL_ENTER )
			ui->outputData();
		break;
	}
	return false;
}

void PropFrame::cleanAllPorp()
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
