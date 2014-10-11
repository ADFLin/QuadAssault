#ifndef EditorWidget_h__
#define EditorWidget_h__

#include "Base.h"

#include "GameEdit.h"
#include "GUISystem.h"
#include "Dependence.h"
#include "ObjectFactory.h"

enum
{
	UI_PROP_FRAME = UI_EDIT_ID ,

	UI_CREATE_LIGHT ,  
	UI_CREATE_TRIGGER ,
	
	UI_NEW_MAP ,
	UI_SAVE_MAP  ,
	UI_TRIGGER_EDIT ,
	UI_OBJECT_EDIT ,
	UI_TILE_EDIT ,

	UI_TILE_SELECT ,
	UI_OBJECT_SELECT ,
};

class IPropCtrl
{
public:
	virtual void        output( char const* str ) = 0;
	virtual std::string input() = 0;
};


enum PropType
{
	PROP_NONE ,
	PROP_FLOAT ,
	PROP_INT ,
	PROP_UCHAR ,
	PROP_BOOL ,
	PROP_STRING ,
	PROP_CTRL   ,
};


class PorpTextCtrl : public GTextCtrl
{
public:
	PorpTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );
	~PorpTextCtrl();
	void     inputData();
	void     outputData();
	void     setData( float&  data ){ mData = &data; mTypeData = PROP_FLOAT; }
	void     setData( int&    data ){ mData = &data; mTypeData = PROP_INT; }
	void     setData( string& data ){ mData = &data; mTypeData = PROP_STRING;  }
	void     setData( bool&   data ){ mData = &data; mTypeData = PROP_BOOL; }
	void     setData( unsigned char& data ){ mData = &data; mTypeData = PROP_UCHAR; }
	void     setControl( IPropCtrl* propCtrl ){ mData = propCtrl; mTypeData = PROP_CTRL; }
	void*    mData; 
	PropType mTypeData;
};

class IntPropChioce : public GChoice
{
	typedef GChoice BaseClass;
public:
	IntPropChioce( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );

	void     init( int numSet , int valueSet[] , char const* strSet[] );
	void     inputData();
	void     outputData();
	void     setData( void* data , int dataSize ){ mData = data; mDataSize = dataSize; }

	void*    mData;
	int      mDataSize;
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
		UI_INT_PROP_CHIOCE ,
	};

	void  changeEdit( IEditable& obj );
	Vec2i getWidgetSize(){ return Vec2i( 130 , 20 ); }
	Vec2i calcWidgetPos();

	virtual void addProp( char const* name , Vec2f& value );
	virtual void addProp( char const* name , Vec3f& value );
#define ADD_TEXT_PROP( TYPE )\
	virtual void addProp( char const* name , TYPE& value )\
	{\
		PorpTextCtrl* textCtrl = new PorpTextCtrl( UI_PROP_TEXTCTRL , calcWidgetPos() , getWidgetSize() , this );\
		textCtrl->setData( value );\
		addPorpWidget( name , textCtrl );\
	}

	ADD_TEXT_PROP( int )
	ADD_TEXT_PROP( unsigned char )
	ADD_TEXT_PROP( float )
	ADD_TEXT_PROP( string )
	ADD_TEXT_PROP( bool )

	virtual void addProp( char const* name , void* value , int sizeValue , int numSet , int valueSet[] , char const* strSet[] );


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
	IEditable*  mEditObj;

};


//TODO : IMPL
class TileButton : public GButtonBase
{
	typedef GButtonBase BaseClass;




};

class TileEditFrame : public GFrame
{
	typedef GFrame BaseClass;
public:
	TileEditFrame( int id , Vec2f const& pos , GWidget* parent );
};



class ObjectEditFrame : public GFrame
{
	typedef GFrame BaseClass;
public:
	ObjectEditFrame( int id , Vec2f const& pos , GWidget* parent );
	static Vec2i getButtonSize(){ return Vec2i( 90 , 20 );}
	void setupObjectList( ObjectCreator& creator );




};



#endif // EditorWidget_h__
