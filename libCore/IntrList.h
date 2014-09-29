#ifndef IntrList_h__
#define IntrList_h__

#include "TypeCast.h"

#include <cassert>


class ListNode
{
public:
	ListNode* getPrev() const { return mPrev; }
	ListNode* getNext() const { return mNext; }
protected:
	ListNode* mPrev;
	ListNode* mNext;
	friend struct ListNodeTraits;
};

struct NodeTraits
{
	class NodePtr;
	static NodePtr getPrev( NodePtr const& n );
	static NodePtr getNext( NodePtr const& n );
	static void setNext( NodePtr const& n , NodePtr const& next );
	static void setPrev( NodePtr const& n , NodePtr const& prev );
};

struct ListNodeTraits
{
	typedef ListNode* NodePtr;
	typedef ListNode const* ConstNodePtr;

	static NodePtr getPrev( NodePtr const& n ){ return n->mPrev; }
	static NodePtr getNext( NodePtr const& n ){ return n->mNext; }
	static ConstNodePtr getPrev( ConstNodePtr const& n ){ return n->mPrev; }
	static ConstNodePtr getNext( ConstNodePtr const& n ){ return n->mNext; }
	static void setNext( NodePtr const& n , NodePtr const& next ){ n->mNext = next; }
	static void setPrev( NodePtr const& n , NodePtr const& prev ){ n->mPrev = prev; }

};

template< class NodeTraits >
class CycleListAlgorithm
{
	typename NodeTraits NodeTraits;
	typedef typename NodeTraits::NodePtr NodePtr;
	typedef typename NodeTraits::ConstNodePtr ConstNodePtr;

public:
	static void initHeader( NodePtr const& n )
	{
		NodeTraits::setNext( n , n );
		NodeTraits::setPrev( n , n );
	}
	static void initNode( NodePtr const& n )
	{
		NodeTraits::setNext( n , NodePtr(0) );
		NodeTraits::setPrev( n , NodePtr(0) );
	}
	static bool isInited( ConstNodePtr const& n )
	{ 
		return !NodeTraits::getNext( n );
	}

	static void linkBefore( NodePtr const& node , NodePtr const& where )
	{
		NodePtr prev = NodeTraits::getPrev( where );

		NodeTraits::setPrev( node , prev );
		NodeTraits::setNext( node , where );

		NodeTraits::setPrev( where , node );
		NodeTraits::setNext( prev , node );
	}

	static void linkAfter( NodePtr const& node , NodePtr const& where )
	{
		NodePtr next = NodeTraits::getNext( where );

		NodeTraits::setPrev( node , where );
		NodeTraits::setNext( node , next );

		NodeTraits::setNext( where , node );
		NodeTraits::setPrev( next  , node );
	}

	static size_t count( ConstNodePtr const& from , ConstNodePtr const& end )
	{
		ConstNodePtr node = from;
		size_t result = 0;
		while ( node != end )
		{
			++result;
			node = NodeTraits::getNext( node );
		}
		return result;
	}

	static void unlink( NodePtr const& node )
	{
		NodePtr prev = NodeTraits::getPrev( node );
		NodePtr next = NodeTraits::getNext( node );
		NodeTraits::setNext( prev , next );
		NodeTraits::setPrev( next , prev );
		initNode( node );
	}
};



class HookNode : public ListNode
{
public:
	typedef ListNodeTraits NodeTraits;
	typedef CycleListAlgorithm< NodeTraits > Algorithm;

	HookNode() {  Algorithm::initNode( this );  }
	~HookNode()
	{ 
		if ( isLinked() ) 
			unlink();  
	}
	bool      isLinked() const { return !Algorithm::isInited( this );  }
	void      unlink(){  Algorithm::unlink( this );  }

	template< class T , HookNode T::*Member >
	static T& cast( HookNode& node ){ return *TypeCast::memberToClass( &node , Member ); }
};

struct HookTraits
{
	class NodeType;
	class NodeTraits;
};

template< class Base , HookNode Base::*Member >
struct MemberHook
{
	typedef ListNode NodeType;
	typedef HookNode::NodeTraits NodeTraits;
	
	template< class T >
	static T& castValue( NodeType& node )
	{ 
		return static_cast< T& >( 
			HookNode::cast< Base , Member >( static_cast< HookNode& >( node ) ) 
		); 
	}
	template< class T >
	static NodeType& castNode( T& value ){ return  value.*Member; }
};

template< class T >
struct DefaultType
{
	typedef T  Type;
	typedef T& InType;
	typedef T& RType;

	static T&    fix( InType value ){ return value; }
	static RType fixRT( T& value )  { return value; }
};

template< class T >
struct PointerType
{
	typedef T* Type;
	typedef T* InType;
	typedef T* RType;
	static T&    fix( InType value ){ return *value; }
	static RType fixRT( T& value )  { return &value; }
};


template< class T , class HookTraits , template< class > class TypePolicy = DefaultType >
class IntrList
{
	typedef typename HookTraits::NodeType   NodeType;
	typedef typename HookTraits::NodeTraits NodeTraits;
	typedef CycleListAlgorithm< NodeTraits > Algorithm;
	typedef typename NodeTraits::NodePtr NodePtr;
	typedef TypePolicy< T > TP;
	typedef typename TP::RType  RType;
	typedef typename TP::InType InType;
	typedef typename TP::Type   BaseType;
public:
	IntrList()
	{
		Algorithm::initHeader( &mHeader );
	}

	~IntrList()
	{
		clear();
	}

	RType  front(){ return TP::fixRT( HookTraits::castValue< T >( NodeTraits::getNext( mHeader ) ) ); }
	RType  back() { return TP::fixRT( HookTraits::castValue< T >( NodeTraits::getPrev( mHeader ) ) ); }

	bool   empty() const { return NodeTraits::getNext( mHeader ) != &mHeader; }
	size_t size() const {  return Algorithm::count( NodeTraits::getNext( &mHeader ) , &mHeader );  }


	void  clear()
	{
		NodePtr node = NodeTraits::getNext( &mHeader );
		while( node != &mHeader )
		{
			NodePtr next = NodeTraits::getNext( node );
			Algorithm::initNode( node );
			node = next;
		}
		Algorithm::initHeader( &mHeader );
	}



	void   remove( InType value )
	{
		assert( haveLink( value ) );
		NodePtr node = &HookTraits::castNode( TP::fix( value ) );
		Algorithm::unlink( node );
	}


	void   push_front( InType value ){ insertAfter( TP::fix( value ) , mHeader ); }
	void   push_back( InType value ) { insertBefore( TP::fix( value ) , mHeader ); }

	void   insertBefore( InType value , InType where ){ insertBefore( TP::fix( value ) , HookTraits::castNode( TP::fix( where ) ) ); }
	void   insertAfter( InType value , InType where ) { insertAfter( TP::fix( value ) , HookTraits::castNode( TP::fix( where ) ) ); }

	bool   haveLink( InType value )
	{
		NodePtr node = &HookTraits::castNode( TP::fix( value ) );
		if ( Algorithm::isInited( node ) )
			return false;

		NodePtr cur = NodeTraits::getNext( &mHeader );
		while ( cur != &mHeader )
		{
			if ( cur == node )
				return true;
			cur = NodeTraits::getNext( cur );
		}
		return false;
	}

private:
	

	template< class Node >
	class Iterator
	{
	public:
		typedef RType     reference;
		typedef BaseType* pointer;

		Iterator( Node* node ):mNode( node ){}

		Iterator  operator++(){ Node* node = mNode; mNode = NodeTraits::getNext( mNode ); return Iterator( node ); }
		Iterator  operator--(){ Node* node = mNode; mNode = NodeTraits::getPrev( mNode ); return Iterator( node ); }
		Iterator& operator++( int ){ mNode = NodeTraits::getNext( mNode ); return *this; }
		Iterator& operator--( int ){ mNode = NodeTraits::getPrev( mNode ); return *this; }

		reference operator*(){ return TP::fixRT( HookTraits::castValue< T >( *mNode ) );  }
		pointer   operator->(){ return &TP::fixRT( HookTraits::castValue< T >( *mNode ) ); }
		bool operator == ( Iterator other ){ return mNode == other.mNode; }
		bool operator != ( Iterator other ){ return mNode != other.mNode; }
	private:
		friend class IntrList;
		NodePtr mNode;
	};
public:

	typedef Iterator< NodeType >       iterator;
	iterator end()   { return iterator( &mHeader ); }
	iterator begin() { return iterator( NodeTraits::getNext( &mHeader ) ); }
	iterator erase( iterator iter )
	{
		NodePtr next = NodeTraits::getNext( iter.mNode );
		Algorithm::unlink( iter.mNode );
		return iterator( next );
	}


private:

	void insertBefore( T& value , NodeType& nodeWhere )
	{
		NodeType& nodeValue = HookTraits::castNode( value );
		assert( Algorithm::isInited( &nodeValue ) );
		assert( !Algorithm::isInited( &nodeWhere ) );
		Algorithm::linkBefore( &nodeValue , &nodeWhere );
	}
	void insertAfter( T& value , NodeType& nodeWhere )
	{
		NodeType& nodeValue = HookTraits::castNode( value );
		assert( Algorithm::isInited( &nodeValue ) );
		assert( !Algorithm::isInited( &nodeWhere ) );
		Algorithm::linkAfter( &nodeValue , &nodeWhere );
	}

	NodeType mHeader;
};



#endif // IntrList_h__
