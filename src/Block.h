#ifndef BLOK_H
#define BLOK_H

#include "Base.h"
#include "RenderUtility.h"

Vec2f const gSimpleBlockSize = Vec2f( BLOCK_SIZE , BLOCK_SIZE );

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
	BF_CAST_SHADOW   = 1 << 0 ,
	BF_MOVABLE       = 1 << 1 ,
	BF_FLYABLE       = 1 << 2 ,
	BF_NONSIMPLE     = 1 << 3 ,
	BF_PASS_VIEW     = 1 << 4 ,
	
};

struct Tile
{
	unsigned char type;
	int  meta;
	Vec2f pos;
};

class Level;
class Light;

class Block
{

public:

	virtual ~Block(){}
	bool  checkFlag( unsigned checkBits ){ return ( flag & checkBits) != 0; }

	virtual void  init( unsigned char tip );
	virtual void  onCollision( Tile& tile , Bullet* bullet );

	virtual void  render( Tile const& tile );
	virtual void  renderNormal( Tile const& tile );
	virtual void  renderGlow( Tile const& tile );

	//call when block is not simple
	virtual void  renderNoTexture( Tile const& tile );
	virtual void  renderShadow( Tile const& tile , Light& light ){}


	static void   initMap( Level* level );
	static void   cleanupMap();
	static Block* FromType( unsigned char type );
	
protected:
	unsigned char type;
	Texture* mTex[ NUM_RENDER_PASS ];
	unsigned flag;
};


class RockBlock : public Block
{
public:
	virtual void  onCollision( Tile& tile , Bullet* bullet );
	void render( Tile const& tile );
};

class DoorBlock : public Block
{
public:
	virtual void renderGlow( Tile const& tile );

};





#endif