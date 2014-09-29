#include "Mob.h"

#include "Level.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Player.h"

#include "MinePickup.h"
#include "TextureManager.h"

void Mob::init( Vec2f const& poz )
{
	this->mPos=poz;

	mTarget = NULL;
	mSize.x=64;
	mSize.y=64;

	rotation=0;
	akceleracija=0;
	brzina=100;
	maxbrzina=100;	

	brzinaPunjenja=250;
	punjenje=0;
	domet=512;

	hp=100;	
}

void Mob::onSpawn()
{
	BaseClass::onSpawn();
}

void Mob::onDestroy()
{
	Explosion* e = getLevel()->createExplosion( getCenterPos(), 128 );
	e->setParam(128,3000,200);
	for(int i=0; i<4; i++)
	{
		MinePickup* c= new MinePickup;
		c->Init( getCenterPos() );
		getLevel()->addItem( c );
	}
	BaseClass::onDestroy();
}

void Mob::tick()
{
	BaseClass::tick();

	Player* player = getLevel()->getPlayer();
	if ( !getLevel()->rayTerrainTest( getCenterPos() , player->getCenterPos() ) )
	{
		mTarget = player;
		mTimeCantView = 0;
		mPosLastView = mTarget->getCenterPos();
	}
	else
	{
		mTimeCantView += TICK_TIME;

		if ( mTimeCantView > 3 )
			mTarget = NULL;
	}

	akceleracija=1;

	if ( mTarget )
	{
		Vec2f dir;
		dir = mPosLastView - getCenterPos();
		Math::normalize( dir );

		rotation = atan2(dir.y,dir.x) + Math::toRad( 90 );

		Vec2f moment;
		float angle = rotation - Math::toRad( 90 );
		moment.x = cos( angle )*akceleracija;
		moment.y = sin( angle )*akceleracija;

		Vec2f offset = ( brzina * TICK_TIME ) * moment;

		mPos.y += offset.y;
		if(checkCollision())
			mPos.y -= offset.y;
		mPos.x += offset.x;
		if(checkCollision())
			mPos.x -= offset.x;
	}

	SudarProjektila();	

	if( hp<=0 )
	{
		destroy();
	}

	akceleracija=0;
}


void Mob::spawnEffect()
{
	Explosion* e = getLevel()->createExplosion( getCenterPos(),512 );
	e->setParam(32,1000,100);
	e->setColor(Vec3(0.25,0.5,1.0));
}

void Mob::DodajMoment(float x)
{
	akceleracija=x;
}


bool Mob::checkCollision()
{
	Rect bBox;
	bBox.min=mPos+Vec2f(4,4);
	bBox.max=mPos+mSize-Vec2f(4,4);

	TileMap& terrain = getLevel()->getTerrain();

	if ( getLevel()->testTerrainCollision( bBox ) )
		return true;

	for( MobList::iterator iter = getLevel()->getMobs().begin() , itEnd = getLevel()->getMobs().end();
		 iter != itEnd ; ++iter )
	{
		Mob* mob = *iter;
		if( mob !=this )
		{
			Rect k2;
			k2.min= mob->getPos();
			k2.max= mob->getPos() + mob->getSize();
			if(bBox.intersect(k2))
				return true;
		}
	}
	Rect k2;
	Player* player = getLevel()->getPlayer();
	k2.min = player->getPos();
	k2.max = player->getPos()+ player->getSize();
	if(bBox.intersect(k2))
		return true;

	return false;
}

void Mob::SudarProjektila()
{
	Rect k1;
	k1.min=mPos+Vec2f(4,4);
	k1.max=mPos+mSize-Vec2f(4,4);

	for( BulletList::iterator iter = getLevel()->getBullets().begin() , itEnd = getLevel()->getBullets().end();
		 iter != itEnd ; ++iter )
	{
		Bullet* bt = *iter;
		if( bt->team == TEAM_PLAYER )
		{
			Rect k2;
			k2.min = bt->getPos();
			k2.max = bt->getPos()+bt->getSize();
			if(k1.intersect(k2))
			{
				takeDamage( bt );				
			}
		}
	}
}

void Mob::takeDamage(Bullet* bullet)
{
	hp -= bullet->getDamage();
	bullet->destroy();
}

void Mob::shoot( IBulletFactory const& creator , float deltaT )
{
	if ( !mTarget )
		return;

	if( mTarget->isDead() )
		return;

	if(punjenje<100)
	{
		punjenje+=brzinaPunjenja*deltaT;
	}
	else if(punjenje>=100)
	{
		Vec2f offset= mPosLastView - getCenterPos();
		if( offset.length2() < domet * domet )
		{
			Math::normalize( offset );
			Bullet* p = creator.create();
			p->init( getCenterPos() ,offset , TEAM_EMPTY );	
			getLevel()->addBullet(p);
		}	
		punjenje=0;
	}
}

void MobRenderer::render( RenderPass pass , LevelObject* object )
{
	Mob* mob = static_cast< Mob* >( object );

	Texture* tex = mTextures[ pass ];

	switch( pass )
	{
	case RP_DIFFUSE: 
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();		
		glColor4f(0.0, 0.0, 0.0, 0.6);			
		drawSprite( mob->getPos() + Vec2f(5,5), mob->getSize() , mob->getRotation() , tex );	
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPopMatrix();
		glDisable(GL_BLEND);
		break;
	}

	glPushMatrix();	
	drawSprite( mob->getPos() + Vec2f(5,5), mob->getSize() , mob->getRotation() , tex );	
	glPopMatrix();	
}
