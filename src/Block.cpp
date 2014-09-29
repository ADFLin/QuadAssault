#include "Block.h"

#include "Level.h"
#include "GameInterface.h"
#include "TextureManager.h"

#include "Bullet.h"
#include "Explosion.h"

static Block* gBlockMap[ 256 ] = { 0 };

struct BlockInfo
{
	BlockType   type;
	unsigned    flag;
	char const* texDiffuse;
	char const* texNormal;
	char const* texGlow;
};

static BlockInfo const gInfo[] = 
{
	{ TID_FLAT , BF_MOVABLE     , "pod1Diffuse.tga" , "prazninaNormal2.tga" , NULL } ,
	//{ TID_FLAT , BF_MOVABLE     , "synthetic_metal_diffuse.png" , "synthetic_metal_normal.png" , NULL } ,
	{ TID_WALL , BF_CAST_SHADOW | BF_COLLISION , "Block.tga" , "zid1Normal.tga" , NULL } ,
	{ TID_GAP  , 0              , "prazninaDiffuse.tga" , "prazninaNormal.tga" , NULL } ,
	{ TID_DOOR , BF_CAST_SHADOW | BF_COLLISION , "vrataDiffuse.tga" , "vrataNormal.tga" , "vrataGlow.tga" } ,
	{ TID_ROCK , BF_CAST_SHADOW | BF_COLLISION , "vrataDiffuse.tga" , "vrataNormal.tga" , "vrataGlow.tga" } ,
};

void Block::Init( unsigned char tip )
{
	BlockInfo const& info = gInfo[ tip ];

	assert( info.type == tip );

	type = info.type;

	mSize  = Vec2f(BLOCK_SIZE,BLOCK_SIZE);
	flag = info.flag;
	
	TextureManager* texMgr = getGame()->getTextureMgr();
	
	mTex[ RP_DIFFUSE ] = ( info.texDiffuse ) ? texMgr->getTexture( info.texDiffuse ) : NULL;
	mTex[ RP_NORMAL  ] = ( info.texNormal ) ? texMgr->getTexture( info.texNormal ) : NULL;
	mTex[ RP_GLOW    ] = ( info.texGlow ) ? texMgr->getTexture( info.texGlow ) : NULL;
}

void Block::render( Tile const& tile )
{	
	glColor3f(1,1,1);	
	drawSprite( tile.pos , mSize , mTex[ RP_DIFFUSE ] );
}

void Block::renderNormal( Tile const& tile )
{	
	glColor3f(1,1,1);
	drawSprite( tile.pos , mSize , mTex[ RP_NORMAL ] );	
}

void Block::renderGlow( Tile const& tile )
{
	if( type == TID_DOOR )
	{
		switch ( tile.meta )
		{
		case DOOR_RED:   glColor3f(1.0f, 0.2f, 0.2f); break;
		case DOOR_BLUE:  glColor3f(0.2f, 0.2f, 1.0f); break;
		case DOOR_GREEN: glColor3f(0.2f, 1.0f, 0.2f); break;
		default:
			glColor3f(1,1,1);
		}
		drawSprite( tile.pos, mSize , mTex[ RP_GLOW ] );
		glColor3f(1,1,1);
	}
}

void Block::renderNoTexture( Tile const& tile )
{
	drawRect( tile.pos , mSize );
}

Block* Block::FromType( unsigned char type )
{
	return gBlockMap[ type ];
}

void Block::initMap( Level* level )
{
	gBlockMap[ TID_FLAT ] = new Block;
	gBlockMap[ TID_WALL ] = new Block;
	gBlockMap[ TID_GAP  ] = new Block;
	gBlockMap[ TID_DOOR ] = new Block;
	gBlockMap[ TID_ROCK ] = new Rock;

	for( int i = 0 ; i < NUM_BLOCK_TYPE ; ++i )
	{
		gBlockMap[i]->Init( i );
	}
}

void Block::cleanupMap()
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

void Rock::onCollision( Tile& tile , Bullet* bullet )
{
	tile.meta -= 100 * bullet->getDamage();
	if ( tile.meta < 0 )
	{
		tile.type = TID_FLAT;
		tile.meta = 0;

		Explosion* e = bullet->getLevel()->createExplosion( tile.pos + 0.5 * mSize , 128 );
		e->setParam(128,3000,200);

		bullet->getLevel()->playSound("explozija1.wav");		
	}
	bullet->destroy();
}

void Rock::render( Tile const& tile )
{
	glColor3f(1,0,0);	
	drawSprite( tile.pos , mSize , mTex[ RP_DIFFUSE ] );
	glColor3f(1,1,1);
}
