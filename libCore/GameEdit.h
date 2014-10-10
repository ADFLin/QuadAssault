#ifndef GameEdit_h__
#define GameEdit_h__

class IPropEditor
{
public:
	virtual void addProp( char const* name , int& value ) = 0;
	virtual void addProp( char const* name , unsigned char& value ) = 0;
	virtual void addProp( char const* name , float& value ) = 0;
	virtual void addProp( char const* name , string& value ) = 0;
	virtual void addProp( char const* name , bool& value ) = 0;
};



class IEditable
{
public:
	virtual void enumProp( IPropEditor& editor ){}
	virtual void updateEdit(){}
};

#endif // GameEdit_h__
