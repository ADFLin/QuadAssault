#include "Mob.h"

#include "Level.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Player.h"

#include "DebrisPickup.h"
#include "TextureManager.h"
#include "RenderUtility.h"

void Mob::init( Vec2f const& poz )
{
	setPos( poz );

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

	mHP=100;	
}

void Mob::onSpawn()
{
	BaseClass::onSpawn();
}

void Mob::onDestroy()
{
	Explosion* e = getLevel()->createExplosion( getPos(), 128 );
	e->setParam(128,3000,200);
	for(int i=0; i<4; i++)
	{
		DebrisPickup* c= new DebrisPickup;
		c->Init( getPos() );
		getLevel()->addItem( c );
	}
	BaseClass::onDestroy();
}

void Mob::tick()
{
	BaseClass::tick();

	Player* player = getLevel()->getPlayer();
	if ( !getLevel()->rayTerrainTest( getPos() , player->getPos() , BF_PASS_VIEW ) )
	{
		mTarget = player;
		mTimeCantView = 0;
		mPosLastView = mTarget->getPos();
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
		dir = mPosLastView - getPos();
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

	if(punjenje<100)
	{
		punjenje+=brzinaPunjenja* TICK_TIME;
	}

	if( mHP<=0 )
	{
		destroy();
	}

	akceleracija=0;

}


void Mob::spawnEffect()
{
	Explosion* e = getLevel()->createExplosion( getPos(),512 );
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
	calcBoundBox( bBox );
	bBox.min += Vec2f(4,4);
	bBox.max -= Vec2f(4,4);

	TileMap& terrain = getLevel()->getTerrain();

	if ( getLevel()->testTerrainCollision( bBox , BF_MOVABLE ) )
		return true;

	for( MobList::iterator iter = getLevel()->getMobs().begin() , itEnd = getLevel()->getMobs().end();
		 iter != itEnd ; ++iter )
	{
		Mob* mob = *iter;
		if( mob == this )
			continue;

		Rect bBoxOther;
		mob->calcBoundBox( bBoxOther );
		if( bBox.intersect(bBoxOther) )
			return true;
	}

	Rect bBoxOther;
	Player* player = getLevel()->getPlayer();
	player->calcBoundBox( bBoxOther );
	if( bBox.intersect(bBoxOther) )
		return true;

	return false;
}

void Mob::SudarProjektila()
{
	Rect bBox;
	calcBoundBox( bBox );
	bBox.min += Vec2f(4,4);
	bBox.max -= Vec2f(4,4);

	for( BulletList::iterator iter = getLevel()->getBullets().begin() , itEnd = getLevel()->getBullets().end();
		 iter != itEnd ; ++iter )
	{
		Bullet* bt = *iter;
		if( bt->team == TEAM_PLAYER )
		{
			Rect bBoxOther;
			bt->calcBoundBox( bBoxOther );

			if( bBox.intersect(bBoxOther) )
			{
				takeDamage( bt );				
			}
		}
	}
}

void Mob::takeDamage(Bullet* bullet)
{
	mHP -= bullet->getDamage();
	bullet->destroy();
}

void Mob::shoot( IBulletFactory const& creator )
{
	if ( !mTarget )
		return;

	if( mTarget->isDead() )
		return;

	if(punjenje>=100)
	{
		Vec2f offset= mPosLastView - getPos();
		if( offset.length2() < domet * domet )
		{
			Math::normalize( offset );
			Bullet* p = creator.create();
			p->init( getPos() ,offset , TEAM_EMPTY );	
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
		drawSprite( mob->getRenderPos() + Vec2f(5,5), mob->getSize() , mob->getRotation() , tex );	
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPopMatrix();
		glDisable(GL_BLEND);
		break;
	}

	glPushMatrix();	
	drawSprite( mob->getRenderPos() , mob->getSize() , mob->getRotation() , tex );	
	glPopMatrix();


	if ( pass == RP_GLOW && gShowBoundBox )
	{
		Vec2f size = mob->getSize();
		glPushMatrix();
		glTranslatef( mob->getRenderPos().x , mob->getRenderPos().y , 0 );
		glColor3f( 0 , 1 , 0 );
		glBegin( GL_LINE_LOOP );
		glVertex3f( 0 , 0 , 0 );
		glVertex3f( size.x , 0 , 0 );
		glVertex3f( size.x , size.y , 0 );
		glVertex3f( 0 , size.y , 0 );
		glEnd();
		glColor3f(1.0, 1.0, 1.0 );
		glPopMatrix();
	}
}
