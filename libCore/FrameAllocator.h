#ifndef FrameAllocator_h__
#define FrameAllocator_h__

#include "IntegerType.h"
#include <new>

class FrameAllocator
{
public:
	FrameAllocator( size_t size )
	{
		mUsageSize = 0;
		mUsage = allocPage( size );
		mUsage->link = NULL;
	}

	~FrameAllocator()
	{
		Page* page = mUsage;
		while( page )
		{ 
			Page* next = page->link;  
			::free( page );
			page = next;
		};
	}

	void* alloc( size_t size )
	{
		if ( mUsage->size < mUsageSize + size )
		{
			Page* page = allocPage( mUsage->size * 2 );
			page->link = mUsage;
			mUsage = page;
			mUsageSize = 0;
		}

		uint8* out = &mUsage->storage[0] + mUsageSize;
		mUsageSize += size;
		return out;
	}

	void clearFrame()
	{
		Page* page = mUsage->link;
		while( page )
		{ 
			Page* next = page->link;  
			::free( page );
			page = next;
		};
		mUsage->link = NULL;
		mUsageSize = 0;
	}

	struct Page
	{
		size_t size;
		Page*  link;
		uint8  storage[0];
	};


	Page* allocPage( size_t size )
	{
		Page* page = (Page*)::malloc( size + sizeof( Page ) );
		page->size = size;
		return page;
	}

	Page*  mUsage;
	int    mUsageSize;

};

inline void* operator new ( size_t size , FrameAllocator& allocator  )
{
	void* out = allocator.alloc( size );
	if ( !out )
		throw std::bad_alloc();

	return out;
}

inline void* operator new[] ( size_t size , FrameAllocator& allocator  )
{
	void* out = allocator.alloc( size );
	if ( !out )
		throw std::bad_alloc();

	return out;
}




#endif // FrameAllocator_h__
