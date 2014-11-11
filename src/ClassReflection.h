#ifndef ClassReflection_h__
#define ClassReflection_h__

enum ClassTypeFlag
{




};

class ClassContextRegister
{
public:
	template< class T >
	void addMember( char const* name , T& var , unsigned flag = 0 );
	template< class T >
	void addEnumMember( char const* name , T& var , int numSet , int const valueSet[] , char const* strSet[] , unsigned flag = 0 );
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

	PROP_CARRAY  = 0x100,
	PROP_CVECTOR = 0x200,
	PROP_CLIST   = 0x300,
	
	PROP_CONTAINER_MASK = 0xf00 ,
};

class CRClass
{
public:
	CRClass( CRClass* inParent , char const* inName )
		:parent( inParent ) , name( inName )
	{

	}
	char const* getName(){ return name; }
	
	char const*  name;
	CRClass*     parent;
};

class CRObject
{
public:
	virtual ~CRObject(){}
	virtual CRClass*   getClass(){ return NULL; }
};


class CRField
{





};


#define DECLARE_RC_CLASS_NOBASE( CLASS )\
private:\
	typedef CLASS ThisClass;

#define BEGIN_CLASS_PROP_NOBASE()\
	public:\
	template < class ClassContextRegister >\
	void reigsterContext( ClassContextRegister& ccr )\
{

#define BEGIN_CLASS_PROP()\
	BEGIN_CLASS_PROP_NOBASE()\
	BaseClass::reigsterContext( ccr );

#define MEMBER_PROP( NAME , MEMBER )\
	ccr.addMember( NAME , MEMBER );

#define MEMBER_PROP_F( NAME , MEMBER , FLAG )\
	ccr.addMember( NAME , MEMBER , FLAG );

#define MENBER_ENUM_PROP( NAME , MEMBER , NUM , VALUE_SET , STR_SET )\
	ccr.addEnumMember( NAME , MEMBER , NUM , VALUE_SET , STR_SET );

#define FUN_PROP( NAME , TYPE , SET , GET )

#define END_CLASS_PROP()\
	}

#endif // ClassReflection_h__