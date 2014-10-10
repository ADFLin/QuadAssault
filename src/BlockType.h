#ifndef BLOCK_INFO
#	define DEFINE_ENUM__
#	define BLOCK_INFO( ID , NAME ) ID ,
#else
#	undef BlockType_h__
#endif

#ifndef BlockType_h__
#define BlockType_h__

#ifdef DEFINE_ENUM__
enum BlockTypeEnum
{
#endif
	BLOCK_INFO( BID_FLAT , "Flat" )
	BLOCK_INFO( BID_WALL , "Wall" )
	BLOCK_INFO( BID_GAP  , "Gap" )
	BLOCK_INFO( BID_DOOR , "Door" )
	BLOCK_INFO( BID_ROCK , "Rock" )

#ifdef DEFINE_ENUM__
	NUM_BLOCK_TYPE ,
};
#endif

#endif // BlockType_h__

#undef DEFINE_ENUM__
#undef BLOCK_INFO