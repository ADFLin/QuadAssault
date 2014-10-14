#ifndef GameEdit_h__
#define GameEdit_h__

#include "Base.h"
#include "FixString.h"

class IPropEditor
{
public:
	virtual void addProp( char const* name , Vec2f& value ) = 0;
	virtual void addProp( char const* name , Vec3f& value ) = 0;
	virtual void addProp( char const* name , int& value ) = 0;
	virtual void addProp( char const* name , unsigned char& value ) = 0;
	virtual void addProp( char const* name , float& value ) = 0;
	virtual void addProp( char const* name , String& value ) = 0;
	virtual void addProp( char const* name , bool& value ) = 0;
	virtual void addProp( char const* name , void* value , int sizeValue , int numSet , int valueSet[] , char const* strSet[] ) = 0;

	template< class T >
	void addEnumProp( char const* name , T& value , int numSet , int valueSet[] , char const* strSet[] )
	{
		addProp( name , &value , sizeof( T ) , numSet , valueSet , strSet );
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


enum PropType
{
	PROP_NONE ,
	PROP_INT ,
	PROP_UINT ,
	PROP_FLOAT ,
	PROP_DOUBLE ,
	PROP_BOOL  ,
	PROP_VEC3F ,
	PROP_VEC2I ,
	PROP_VEC2F ,
	PROP_COLOR ,
	PROP_STRING ,
	PROP_CTRL   ,
};


typedef FixString< 512 > FString;
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
	T&       castValueT(){ return *static_cast< T* >( mData ); }
	bool     getString( FString& str );
	bool     setValue( char const* str );

private:
	void*    mData;
	uint8    mDataSize;
	PropType mType;
};

class TextPropBase : public IPropEditor
{
public:
	struct Prop
	{
		char const* name;
		PropData    data;
	};
	Prop* findProp( char const* name );

#define ADD_PROP( TYPE )\
	virtual void addProp( char const* name , TYPE& value )\
	{\
		Prop prop;\
		prop.name = name;\
		prop.data.setData( value );\
		mProps.push_back( prop );\
	}

	ADD_PROP( Vec2f )
	ADD_PROP( Vec3f )
	ADD_PROP( int )
	ADD_PROP( unsigned char )
	ADD_PROP( float )
	ADD_PROP( String )
	ADD_PROP( bool )

#undef  ADD_PROP

	virtual void addProp( char const* name , void* value , int sizeValue , int numSet , int valueSet[] , char const* strSet[] )
	{
		Prop prop;
		prop.name = name;
		prop.data.setEnumData( value , sizeValue );
		mProps.push_back( prop );
	}



	typedef std::vector< Prop > PropVec;
	PropVec mProps;
};

//TODO
class TextPropImport : public TextPropBase
{
public:
};

class TextPropExport : public TextPropBase
{
public:


};

#endif // GameEdit_h__
