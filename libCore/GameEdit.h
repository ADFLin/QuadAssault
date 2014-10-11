#ifndef GameEdit_h__
#define GameEdit_h__

class IPropEditor
{
public:
	virtual void addProp( char const* name , Vec2f& value ) = 0;
	virtual void addProp( char const* name , Vec3f& value ) = 0;
	virtual void addProp( char const* name , int& value ) = 0;
	virtual void addProp( char const* name , unsigned char& value ) = 0;
	virtual void addProp( char const* name , float& value ) = 0;
	virtual void addProp( char const* name , string& value ) = 0;
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

#endif // GameEdit_h__
