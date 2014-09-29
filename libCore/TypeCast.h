#ifndef TypeCast_h__
#define TypeCast_h__

#include "CompilerConfig.h"
#include "IntegerType.h"
#include <cstddef>

class TypeCast
{
public:
	//Impl By Boost
	template< class T , class Member >
	static T* memberToClass( Member* member , Member T::*ptrMember )
	{
		ptrdiff_t memberOffset;
		//The implementation of a pointer to member is compiler dependent.
#if defined( CPP_COMPILER_MSVC )
		//msvc compliant compilers use their the first 32 bits as offset (even in 64 bit mode)
		union caster_union
		{
			const Member T::* ptr_to_member;
			int32 offset;
		} caster;
		caster.ptr_to_member = ptrMember;
		memberOffset = ptrdiff_t(caster.offset);
		//This works with gcc, msvc, ac++, ibmcpp
#elif defined( CPP_COMPILER_GCC )
		const T* const parent = 0;
		const char *const memberPos = static_cast<const char*>(static_cast<const void*>(&(parent->*ptr_to_member)));
		memberOffset = std::ptrdiff_t(memberPos - static_cast<const char*>(static_cast<const void*>(parent)));
#else
		//This is the traditional C-front approach: __MWERKS__, __DMC__, __SUNPRO_CC
		union caster_union
		{
			const Member Parent::* ptr_to_member;
			std::ptrdiff_t offset;
		} caster;
		caster.ptr_to_member = ptr_to_member;
		memberOffset = caster.offset - 1;
#endif
		return static_cast<T*>(
			static_cast<void*>(
			static_cast<char*>(static_cast<void*>(member)) - memberOffset
			)
		);
	}




};

#endif // TypeCast_h__
