#include "Block.h"

#include "Level.h"
#include "GameInterface.h"
#include "TextureManager.h"

#include "RenderUtility.h"
#include "Bullet.h"
#include "Explosion.h"

static Block* gBlockMap[ 256 ] = { 0 };

struct BlockInfo
{
	BlockType   type;
	unsigned    colMask;
	unsigned    flag;
	char const* texDiffuse;
	char const* texNormal;
	char const* texGlow;
};

static void createBlockClass();

static BlockInfo const gInfo[] = 
{
	{ BID_FLAT , 0 , 0 , "pod1Diffuse.tga" , "prazninaNormal2.tga" , NULL } ,
	{ BID_WALL , COL_OBJECT | COL_VIEW , BF_CAST_SHADOW , "Block.tga" , "zid1Normal.tga" , NULL } ,
	{ BID_GAP  , COL_SOILD | COL_TRIGGER | COL_VIEW , 0 , "prazninaDiffuse.tga" , "prazninaNormal.tga" , NULL } ,
	{ BID_DOOR , COL_OBJECT | COL_VIEW , BF_CAST_SHADOW , "vrataDiffuse.tga" , "vrataNormal.tga" , "vrataGlow.tga" } ,
	{ BID_ROCK , COL_OBJECT | COL_VIEW , BF_CAST_SHADOW , "vrataDiffuse.tga" , "vrataNormal.tga" , "vrataGlow.tga" } ,
};

void Block::init( BlockType type )
{
	BlockInfo const& info = gInfo[ type ];

	assert( info.type == type );

	mType = info.type;
	mFlag = info.flag;
	mColMask = info.colMask;
	
	TextureManager* texMgr = getGame()->getTextureMgr();
	
	mTex[ RP_DIFFUSE ] = ( info.texDiffuse ) ? texMgr->getTexture( info.texDiffuse ) : NULL;
	mTex[ RP_NORMAL  ] = ( info.texNormal ) ? texMgr->getTexture( info.texNormal ) : NULL;
	mTex[ RP_GLOW    ] = ( info.texGlow ) ? texMgr->getTexture( info.texGlow ) : NULL;
}

void Block::render( Tile const& tile )
{	
	glColor3f(1,1,1);	
	drawSprite( tile.pos , gSimpleBlockSize , mTex[ RP_DIFFUSE ] );
}

void Block::renderNormal( Tile const& tile )
{	
	glColor3f(1,1,1);
	drawSprite( tile.pos , gSimpleBlockSize , mTex[ RP_NORMAL ] );	
}

void Block::renderGlow( Tile const& tile )
{

}

void Block::renderNoTexture( Tile const& tile )
{
	drawRect( tile.pos , gSimpleBlockSize );
}

Block* Block::FromType( unsigned char type )
{
	return gBlockMap[ type ];
}


void Block::initialize()
{
	createBlockClass();

	for( int i = 0 ; i < NUM_BLOCK_TYPE ; ++i )
	{
		gBlockMap[i]->init( i );
	}
}


void Block::cleanup()
{
	for(int i=0; i< NUM_BLOCK_TYPE; i++)
	{
		delete gBlockMap[i];
		gBlockMap[i] = NULL;
	}

}

void Block::onCollision( Tile& tile , Bullet* bullet )
{
	bullet->destroy();
}

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

void RockBlock::onCollision( Tile& tile , Bullet* bullet )
{
	tile.meta -= 100 * bullet->getDamage();
	if ( tile.meta < 0 )
	{
		tile.type = BID_FLAT;
		tile.meta = 0;

		Explosion* e = bullet->getLevel()->createExplosion( tile.pos + 0.5 * gSimpleBlockSize , 128 );
		e->setParam(128,3000,200);

		bullet->getLevel()->playSound("explosion1.wav");		
	}
	bullet->destroy();
}

void RockBlock::render( Tile const& tile )
{
	glColor3f(1,0,0);	
	drawSprite( tile.pos , gSimpleBlockSize , mTex[ RP_DIFFUSE ] );
	glColor3f(1,1,1);
}

void DoorBlock::renderGlow( Tile const& tile )
{
	switch ( tile.meta )
	{
	case DOOR_RED:   glColor3f(1.0f, 0.2f, 0.2f); break;
	case DOOR_BLUE:  glColor3f(0.2f, 0.2f, 1.0f); break;
	case DOOR_GREEN: glColor3f(0.2f, 1.0f, 0.2f); break;
	default:
		glColor3f(1,1,1);
	}
	drawSprite( tile.pos, gSimpleBlockSize , mTex[ RP_GLOW ] );
	glColor3f(1,1,1);
}


static void createBlockClass()
{
	gBlockMap[ BID_FLAT ] = new Block;
	gBlockMap[ BID_WALL ] = new Block;
	gBlockMap[ BID_GAP  ] = new Block;
	gBlockMap[ BID_DOOR ] = new DoorBlock;
	gBlockMap[ BID_ROCK ] = new RockBlock;
}

