#ifndef GameEdit_h__
#define GameEdit_h__

#include "Base.h"
#include "ClassReflection.h"
#include <vector>

class IPropCtrl
{
public:
	virtual bool output( char const* str ) = 0;
	virtual bool input( FString& str ) = 0;
};


class PropData
{
public:
	PropData()
	{
		mData     = NULL;
		mDataSize = 0;
		mType     = PROP_NONE;
	}
	template< class T >
	PropData( T& data ){ setData( data ); }

	PropType getType() const {  return PropType( mType & ~PROP_CONTAINER_MASK );  }
	bool     isContainer() const {  return ( mType & PROP_CONTAINER_MASK ) != 0;  }

	void     setData( char&   data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_INT; }
	void     setData( short&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_INT; }
	void     setData( int&    data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_INT; }

	void     setData( unsigned char&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_UINT; }
	void     setData( unsigned short& data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_UINT; }
	void     setData( unsigned int&   data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_UINT; }

	void     setData( float&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_FLOAT; }
	void     setData( Vec2i&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_VEC2I; }
	void     setData( Vec2f&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_VEC2F; }
	void     setData( Vec3f&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_VEC3F; }
	void     setData( Color&  data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_COLOR; }
	void     setData( String& data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_STRING; }
	void     setData( bool&   data ){ mData = &data; mDataSize = sizeof(data); mType = PROP_BOOL; }

	void     setEnumData( void* data , int dataSize )
	{
		mData = data;
		mDataSize = dataSize;
		mType = PROP_INT;
	}
	void     setControl( IPropCtrl* propCtrl ){ mData = propCtrl; mType = PROP_CTRL; }

	template< class T >
	T&       cast() const { return *static_cast< T* >( mData ); }
	bool     getString( FString& str );
	bool     setValue( char const* str );

private:
	void*    mData;
	uint16   mType;
	uint16   mDataSize;
	
};

class IPropEditor
{
public:
	virtual void addPropData( char const* name , PropData const& data , unsigned flag ) = 0;
	virtual void addProp( char const* name , void* value , int sizeValue , int numSet , int const valueSet[] , char const* strSet[] , unsigned flag ) = 0;

	template< class T >
	void addProp( char const* name , T& value , unsigned flag = 0 )
	{
		addPropData( name , PropData( value ) , flag );
	}
	template< class T >
	void addEnumProp( char const* name , T& value , int numSet , int const valueSet[] , char const* strSet[], unsigned flag = 0  )
	{
		addProp( name , &value , sizeof( T ) , numSet , valueSet , strSet , flag );
	}
};

class IEditable
{
public:
	virtual ~IEditable(){}
	virtual void enumProp( IPropEditor& editor ){}
	virtual void updateEdit(){}
	virtual void setupDefault(){}
};


class ClassEditReigster
{
public:
	ClassEditReigster( IPropEditor& editor ):mEditor( editor ){}

	template< class T >
	void addMember( char const* name , T& var , unsigned flag = 0 )
	{
		mEditor.addProp( name , var , flag );
	}
	template< class T >
	void addEnumMember( char const* name , T& var , int numSet , int const valueSet[] , char const* strSet[] , unsigned flag = 0 )
	{
		mEditor.addEnumProp( name , var , numSet , valueSet , strSet , flag );
	}
	IPropEditor& mEditor;
};

class TextPropEditor : public IPropEditor
{
public:
	struct Prop
	{
		char const* name;
		PropData    data;
	};
	Prop* findProp( char const* name );
	void  setupPorp( IEditable& editable );
	virtual void addPropData(char const* name , PropData const& data , unsigned flag );
	virtual void addProp( char const* name , void* value , int sizeValue , int numSet , int const valueSet[] , char const* strSet[] , unsigned flag );
	void  exportString( String& out );
	void  importString( char const* str );

	typedef std::vector< Prop > PropVec;
	PropVec mProps;
};


#endif // GameEdit_h__
