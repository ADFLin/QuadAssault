#ifndef ObjectFactory_h__
#define ObjectFactory_h__

#include <map>
class LevelObject;

template< class T >
class IFactoryT
{
public:
	virtual ~IFactoryT(){}
	virtual T* create() const = 0;

	template < class Q >
	class CFactory : public IFactoryT< T >
	{
	public:
		Q* create() const
		{ 
			Q* obj = new Q;
			obj->init();
			return obj;
		}
	};

	template < class Q >
	static CFactory< Q >  Make(){ return CFactory< Q >(); }
	template < class Q >
	static CFactory< Q >* Create(){ return new CFactory< Q >(); }
};

typedef IFactoryT< LevelObject > ObjectFactory;

class ObjectCreator
{
public:
	
	template< class T >
	class CFactory : public ObjectFactory
	{
	public:
		CFactory( char const* name ):name( name ){}
		T* create() const
		{ 
			T* obj = new T;
			obj->init();
			return obj;
		}
		string name;
	};

	LevelObject* createObject( char const* name )
	{
		FactoryMap::iterator iter = mNameMap.find( name );
		if ( iter == mNameMap.end() )
			return NULL;
		LevelObject* obj = iter->second->create();
		return obj;
	}

	template< class T >
	bool registerClass( char const* name )
	{
		CFactory< T >* factory = new CFactory< T >( name );
		if ( !mNameMap.insert( std::make_pair( factory->name.c_str() , factory ) ).second  )
		{
			delete factory;
			return false;
		}
		return true;
	}

	struct StrCmp
	{
		bool operator()( char const* a , char const* b ) const
		{
			return strcmp( a , b ) < 0;
		}
	};

	typedef std::map< char const* , ObjectFactory* , StrCmp > FactoryMap;
	FactoryMap& getFactoryMap(){ return mNameMap; }
protected:
	FactoryMap mNameMap;
};

typedef ObjectCreator::FactoryMap ObjectFactoryMap;

#endif // ObjectFactory_h__
