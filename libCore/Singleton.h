#ifndef Singleton_h__
#define Singleton_h__

template< class T >
class SingletonT
{
public:
	static T& getInstance()
	{
		if ( !_instance )
			_instance = new T;
		return *_instance;
	}

	static void releaseInstance()
	{
		if ( _instance )
		{
			delete _instance;
			_instance = 0;
		}
	}
protected:
	SingletonT(){}
	SingletonT( SingletonT const& sing ){}
private:
	static T* _instance;
};

template< class T >
T* SingletonT< T >::_instance = 0;

#endif // Singleton_h__
