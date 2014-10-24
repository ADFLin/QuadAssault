#ifndef EditorWidget_h__
#define EditorWidget_h__

#include "Base.h"

#include "GameEdit.h"
#include "GUISystem.h"
#include "Dependence.h"
#include "ObjectFactory.h"
#include "Trigger.h"

enum
{
	UI_PROP_FRAME = UI_EDIT_ID ,

	UI_CREATE_LIGHT ,  
	UI_CREATE_TRIGGER ,
	
	UI_NEW_MAP ,
	UI_SAVE_MAP  ,
	UI_OBJECT_EDIT ,
	UI_TILE_EDIT ,

	UI_TILE_SELECT ,
	UI_ACTION_SELECT ,

	UI_OBJECT_DESTROY ,

	UI_OBJECT_LISTCTRL ,
	UI_ACTION_LISTCTRL ,
};



class PorpTextCtrl : public GTextCtrl
{
public:
	PorpTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );
	~PorpTextCtrl();
	void      inputData();
	void      outputData();
	void      setData( PropData const& data ){ mPorpData = data; }
private:
	PropData mPorpData;
};

class IntPropChioce : public GChoice
{
	typedef GChoice BaseClass;
public:
	IntPropChioce( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );

	void     init( int numSet , int const valueSet[] , char const* strSet[] );
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

	void   changeEdit( IEditable& obj );
	void   removeEdit();

	void     inputData();
	void     outputData();

	using IPropEditor::addProp;
	virtual void addPropData( char const* name , PropData const& data , unsigned flag );
	virtual void addProp( char const* name , void* value , int sizeValue , int numSet , int const valueSet[] , char const* strSet[] , unsigned flag );

	void addPorpWidget( char const* name , GWidget* widget );

	Vec2i getWidgetSize(){ return Vec2i( 130 , 20 ); }
	Vec2i calcWidgetPos();

	virtual bool onChildEvent( int event , int id , GWidget* ui );
	virtual void onRender();

private:
	void     cleanupAllPorp();

	struct PropInfo
	{
		IText*    name;
		GWidget*  widget;
	};
	typedef std::vector< PropInfo >  PropInfoVec;
	PropInfoVec mPorps;
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
	static int const ButtonLength = 40;
};

class ObjectEditFrame : public GFrame
{
	typedef GFrame BaseClass;
public:
	ObjectEditFrame( int id , Vec2f const& pos , GWidget* parent );
	static Vec2i ButtonSize(){ return Vec2i( 90 , 20 ); }
	void setupObjectList( ObjectCreator& creator );

	GListCtrl* mObjectListCtrl;
};


class ActionEditFrame : public GFrame
{
	typedef GFrame BaseClass;

public:

	ActionEditFrame( int id , Vec2i const& pos , GWidget* widget );

	void setTrigger( TriggerBase* trigger );

	static int const ListCtrlWidth = 100; 
	static Vec2i ButtonSize(){ return Vec2i( 90 , 20 ); }
	void setupActionList( ActionCreator& creator );
	void refreshList();


	TriggerBase*  mTrigger;
	GListCtrl*    mListCtrl;
};



#endif // EditorWidget_h__
