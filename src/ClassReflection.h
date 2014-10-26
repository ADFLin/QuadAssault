#ifndef ClassReflection_h__
#define ClassReflection_h__

enum ClassTypeFlag
{




};

class ClassContextRegister
{



};


enum ClassPropFlag
{
	CPF_COLOR  = BIT(0) ,
	CPF_NET    = BIT(1) ,
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
	PROP_OBJECT ,
	PROP_CTRL   ,
};




#endif // ClassReflection_h__