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
};

class ObjectCreator
{
public:
	typedef IFactoryT< LevelObject > ObjectFactory;


	template< class T >
	class CFactory : public ObjectFactory
	{
	public:
		T* create(){ return new T; }
		string name;
	};

	template< class T >
	bool registerClass( char const* name )
	{
		CFactory< T > factory = new CFactory< T >( name );
		if ( !mNameMap.insert( std::make_pair( factory.name.c_str() , factory ) ).second  )
		{
			delete factory;
			return false;
		}
		return true;
	}

	typedef std::map< char const* , ObjectFactory* > FactoryMap;
	FactoryMap mNameMap;
};

#endif // ObjectFactory_h__
