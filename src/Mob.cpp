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
	mBody.setSize( getSize()- Vec2f(4,4) );
	mBody.setMask( COL_SOILD );
	mBody.setMaskCheck( COL_BULLET | COL_SOILD | COL_TERRAIN );
	getLevel()->getColManager().addBody( *this , mBody );
}

void Mob::onDestroy()
{
	getLevel()->getColManager().removeBody( mBody );

	Explosion* e = getLevel()->createExplosion( getPos(), 128 );
	e->setParam(128,3000,200);
	for(int i=0; i<4; i++)
	{
		DebrisPickup* c = new DebrisPickup( getPos() );
		c->init();
		getLevel()->addItem( c );
	}
	BaseClass::onDestroy();
}

void Mob::tick()
{
	BaseClass::tick();

	Player* player = getLevel()->getPlayer();
	if ( !getLevel()->getColManager().rayTerrainTest( getPos() , player->getPos() , COL_VIEW ) )
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

		Vec2f off = ( brzina * TICK_TIME ) * moment;

		Vec2f offset = Vec2f( 0 , 0 );

		offset.y += off.y;
		if( testCollision( offset ) )
			offset.y = 0;
		offset.x += off.x;
		if( testCollision( offset ) )
			offset.x = 0;

		mPos += offset;
	}

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


bool Mob::testCollision( Vec2f const& offset )
{
	ColInfo info;
	return getLevel()->getColManager().testCollision( info , offset , mBody , COL_SOILD | COL_TERRAIN );
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

void Mob::onBodyCollision( ColBody& self , ColBody& other )
{
	LevelObject* obj = other.getClient();
	switch( obj->getType() )
	{
	case OT_BULLET:
		{
			Bullet* bullet = obj->cast< Bullet >();
			if ( bullet->team == TEAM_PLAYER )
			{
				takeDamage( bullet );
			}
		}
		break;
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
