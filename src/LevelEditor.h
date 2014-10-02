#ifndef LevelEditor_h__
#define LevelEditor_h__


enum EditMethod
{
	EM_TEXT ,
};

class PropEditor
{
public:
	virtual void addProp( char const* name , float& value ) = 0;
	virtual void addProp( char const* name , int& value ) = 0;
	virtual void addProp( char const* name , string& value ) = 0;
	virtual void addProp( char const* name , bool& value ) = 0;
};

class LevelEditor
{











	Level* mLevel;
};

#endif // LevelEditor_h__
