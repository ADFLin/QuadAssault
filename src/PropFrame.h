#ifndef PropFrame_h__
#define PropFrame_h__

#include "Base.h"

#include "Object.h"
#include "GUISystem.h"
#include "Dependence.h"

enum PropType
{
	PROP_FLOAT ,
	PROP_INT ,
	PROP_BOOL ,
	PROP_STRING ,
};


class PorpTextCtrl : public GTextCtrl
{
public:
	PorpTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );
	void     inputData();
	void     outputData();
	void     setData( float&  data ){ mData = &data; mTypeData = PROP_FLOAT; }
	void     setData( int&    data ){ mData = &data; mTypeData = PROP_INT; }
	void     setData( string& data ){ mData = &data; mTypeData = PROP_STRING;  }
	void     setData( bool&   data ){ mData = &data; mTypeData = PROP_BOOL; }
	void*    mData; 
	PropType mTypeData;
};

class PropFrame : public GFrame
	            , public IPropEditor
{
	typedef GFrame BaseClass;
public:

	PropFrame( int id , Vec2i const& pos , GWidget* parent );

	enum 
	{
		UI_PROP_TEXTCTRL = UI_WIDGET_ID ,
	};

	Vec2i getWidgetSize(){ return Vec2i( 100 , 20 ); }
	Vec2i calcWidgetPos()
	{
		return Vec2i( 100 , 10 ) + Vec2i( 0 , mPorps.size() * ( getWidgetSize().y + 5 ) );
	}

	virtual void addProp( char const* name , float& value );
	virtual void addProp( char const* name , int& value );
	virtual void addProp( char const* name , string& value );
	virtual void addProp( char const* name , bool& value );

	void addPorpWidget( char const* name , GWidget* widget );


	virtual bool onChildEvent( int event , int id , GWidget* ui );
	virtual void onRender();

	void     inputData();
	void     outputData();
	void     cleanAllPorp();

	struct PropData
	{
		IText*    name;
		GWidget*  widget;
	};
	typedef std::vector< PropData >  PropDataVec;
	PropDataVec mPorps;

};

#endif // PropFrame_h__
