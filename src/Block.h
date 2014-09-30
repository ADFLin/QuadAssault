#ifndef BLOK_H
#define BLOK_H

#include "Base.h"
#include "RenderUtility.h"


enum BlockType
{
	TID_FLAT , 
	TID_WALL , 
	TID_GAP  ,
	TID_DOOR ,
	TID_ROCK ,

	NUM_BLOCK_TYPE ,
};

enum DoorType
{
	DOOR_RED   = 0 ,
	DOOR_GREEN = 1 ,
	DOOR_BLUE  = 2 ,
};

enum BlockFlag
{
	BF_CAST_SHADOW      = 1 << 0 ,
	BF_MOVABLE          = 1 << 1 ,
	BF_FLYABLE          = 1 << 2 ,
	BF_NONSIMPLE        = 1 << 3 ,
	BF_PASS_VIEW       = 1 << 4 ,
	
};

struct Tile
{
	unsigned char type;
	int  meta;
	Vec2f pos;
};

class Level;

class Block
{
protected:
	Vec2f   mSize; //dimenzije
	unsigned char type;
	Texture* mTex[ NUM_RENDER_PASS ];
	unsigned flag;
public:
	virtual ~Block(){}
	bool  checkFlag( unsigned checkBits ){ return ( flag & checkBits) != 0; }

	virtual void  Init( unsigned char tip );
	virtual void  onCollision( Tile& tile , Bullet* bullet );
	virtual void  render( Tile const& tile );

	void  renderNormal( Tile const& tile );
	void  renderGlow( Tile const& tile );
	void  renderNoTexture( Tile const& tile );


	static void   initMap( Level* level );
	static void   cleanupMap();
	static Block* FromType( unsigned char type );
	
};


class Rock : public Block
{
public:
	virtual void  onCollision( Tile& tile , Bullet* bullet );
	void render( Tile const& tile );
};





#endif