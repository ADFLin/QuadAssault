#ifndef THolder_h__
#define THolder_h__


namespace Detail
{
	template< class T >
	struct ManagePolicy
	{
		class ManageType;
		void release( T* ptr );
	};

	template< class T >
	struct PtrPolicy
	{
		typedef T* ManageType; 
		void destroy( ManageType ptr ){  delete ptr;  }
		void setZero( ManageType& ptr ){ ptr = 0; }
	};

	template< class T >
	struct ArrayPtrPolicy
	{
		typedef T* ManageType; 
		void destroy( ManageType ptr ){  delete [] ptr;  }
		void setZero( ManageType& ptr ){ ptr = 0; }
	};

	template< class T , template< class T > class ManagePolicy >
	class HolderImpl : private ManagePolicy< T >
	{
		typedef ManagePolicy< T > MP;
	public:
		typedef typename MP::ManageType ManageType;

		HolderImpl(){ MP::setZero( m_ptr ); }
		explicit HolderImpl( ManageType ptr):m_ptr(ptr){}
		~HolderImpl(){	MP::destroy( m_ptr );  }

		void reset( ManageType  ptr )
		{
			MP::destroy( m_ptr );
			m_ptr = ptr;
		}

		void clear()
		{
			MP::destroy( m_ptr );
			MP::setZero( m_ptr );
		}
		ManageType release()
		{ 
			ManageType temp = m_ptr; 
			MP::setZero( m_ptr );
			return temp;
		}
		ManageType get() const { return m_ptr; }

	protected:
		ManageType m_ptr;
	private:
		template < template < class > class M >
		HolderImpl<T , M >& operator=( ManageType ptr);
		template < template < class > class M >
		HolderImpl( HolderImpl<T , M > const& );
		template < template < class > class M >
		HolderImpl<T , M >& operator=(HolderImpl<T , M > const&);
	};
}

template< class T >
class TPtrHolder : public Detail::HolderImpl< T , Detail::PtrPolicy >
{
public:
	TPtrHolder(){}
	explicit TPtrHolder(T* ptr):Detail::HolderImpl< T , Detail::PtrPolicy  >(ptr){}
	
	T&       operator*()        { return *m_ptr; }
	T const& operator*()  const { return *m_ptr; }
	T*       operator->()       { return m_ptr; }
	T const* operator->() const { return m_ptr; }
	operator T*()               { return m_ptr; }
	operator T const*() const   { return m_ptr; }
};

template< class T >
class TArrayHolder : public Detail::HolderImpl< T , Detail::ArrayPtrPolicy >
{
public:
	TArrayHolder(){}
	explicit TArrayHolder(T* ptr):Detail::HolderImpl< T , Detail::ArrayPtrPolicy >(ptr){}

	operator T*()             { return m_ptr;  }
	operator T const*() const { return m_ptr;  }
};

#endif // THolder_h__
