#include "KeyPickup.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Player.h"
#include "Explosion.h"

class KeyPickupRenderer : public IRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getGame()->getTextureMgr();
		mTex[ RP_DIFFUSE ] = texMgr->getTexture("KljucDiffuse.tga");
		mTex[ RP_NORMAL  ] = texMgr->getTexture("KljucNormal.tga");	
		mTex[ RP_GLOW    ] = texMgr->getTexture("KljucGlow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		KeyPickup* pickup = static_cast< KeyPickup* >( object );
		if ( pass == RP_GLOW )
		{
			if( pickup->id==KEY_RED)
				glColor3f(1.0, 0.1, 0.1);
			else if( pickup->id==KEY_BLUE )
				glColor3f(0.1, 0.1, 1.0);
			else if( pickup->id==KEY_GREEN )
				glColor3f(0.1, 1.0, 0.1);
		}
		drawSprite(pickup->getRenderPos(),pickup->getSize(),pickup->rotation, mTex[ pass ] );
		glColor3f(1.0, 1.0, 1.0);
	}

	Texture* mTex[ NUM_RENDER_PASS ];
};

DEFINE_RENDERER( KeyPickup , KeyPickupRenderer )

void KeyPickup::Init(Vec2f poz, int id)
{
	ItemPickup::Init(poz);

	mSize.x=32;
	mSize.y=32;
	this->id=id;
	
	rotation=0;

}

void KeyPickup::onSpawn()
{
	BaseClass::onSpawn();
	s = getLevel()->createLight( getPos() , 128 , false);
	if(id==KEY_RED)
		s->setColorParam(Vec3(1.0,0.1,0.1),4);	
	if(id==KEY_BLUE)
		s->setColorParam(Vec3(0.1,0.25,1.0),4);	
	if(id==KEY_GREEN)
		s->setColorParam(Vec3(0.1,1.0,0.1),4);	
}

void KeyPickup::onDestroy()
{
	s->destroy();
	BaseClass::onDestroy();
}

void KeyPickup::tick()
{
	BaseClass::tick();
	rotation += Math::toRad( 100*TICK_TIME );
	if(rotation> 2 * PI )
		rotation -= 2 * PI;
}

void KeyPickup::onPick(Player* igrac)
{
	getLevel()->playSound("pickup.wav");		

	int door;

	if(id==KEY_RED)	
		door = DOOR_RED;
	else if(id==KEY_BLUE)	
		door = DOOR_BLUE;
	else if(id==KEY_GREEN)
		door = DOOR_GREEN;


	TileMap& terrain = getLevel()->getTerrain();

	for(int x=0; x< terrain.getSizeX() ; ++x )
	for(int y=0; y< terrain.getSizeY() ; ++y )
	{
		Tile& tile = terrain.getData( x , y );

		if( tile.type == TID_DOOR && tile.meta == door )
		{
			tile.type = TID_FLAT;
			tile.meta = 0;

			Explosion* e=getLevel()->createExplosion( Vec2f(x*BLOCK_SIZE+BLOCK_SIZE/2, y*BLOCK_SIZE+BLOCK_SIZE/2), 128 );
			e->setParam(20,1000,50);
			if(id==KEY_RED)
				e->setColor(Vec3(1.0, 0.1, 0.1));
			else if(id==KEY_BLUE)
				e->setColor(Vec3(0.1, 0.1, 1.0));
			else if(id==KEY_GREEN)
				e->setColor(Vec3(0.1, 1.0, 0.1));
		}
	}

	destroy();
	
}
