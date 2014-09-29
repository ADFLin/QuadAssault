#include "Player.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "SoundManager.h"

#include "Light.h"
#include "Bullet.h"
#include "Mob.h"
#include "Explosion.h"

#include "Laser.h"
#include "Plasma.h"
#include "Minigun.h"

#include "RenderUtility.h"
#include "Texture.h"


class PlayerRenderer : public IRenderer
{
public:
	virtual void init()
	{

		TextureManager* texMgr = getGame()->getTextureMgr();
		textura = texMgr->getTexture("tenkTorzoDiffuse.tga");	
		texturaN = texMgr->getTexture("tenkTorzoNormal.tga");

		podloga_tex = texMgr->getTexture("tenkPodlogaDiffuse.tga");
		podloga_normal = texMgr->getTexture("tenkPodlogaNormal.tga");

		tracnica_tex = texMgr->getTexture("tracnicaDiffuse.tga");	
		tracnica_normal = texMgr->getTexture("tracnicaNormal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		Player* player = object->cast< Player >();

		if( player->mIsDead )
			return;

		switch( pass )
		{
		case RP_DIFFUSE:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPushMatrix();
			glTranslatef(4,4,0);
			glColor4f( 0.0, 0.0, 0.0, 0.5 );	
			RenderPodlogu( pass , player );
			RenderTrack( pass , player );
			RenderTorzo( pass , player );	
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
			glDisable(GL_BLEND);
		case RP_NORMAL:
			glColor3f( 1.0, 1.0, 1.0 );	
			RenderPodlogu( pass , player );
			RenderTrack( pass , player );
			RenderTorzo( pass , player );	
			glColor3f(1.0, 1.0, 1.0 );
			break;

		}

		for(int i=0; i<4; i++)
		{
			Weapon* weapon = player->weaponSlot[i];
			if( weapon )
			{
				Vec2f centerPos = player->getCenterPos();

				glPushMatrix();
				glTranslatef( centerPos.x,centerPos.y,0 );
				glRotatef( Math::toDegree( player->rotationAim ) + 90 ,0,0,1 );
				glTranslatef(weapon->getPos().x ,weapon->getPos().y ,0 );
				weapon->render( pass );
				glPopMatrix();
			}
		}
	}

	void RenderPodlogu( RenderPass pass , Player* player )
	{
		Texture* tex;
		if(pass==RP_DIFFUSE)
			tex=podloga_tex;
		if(pass==RP_NORMAL)
			tex=podloga_normal;

		glPushMatrix();	
		drawSprite( player->getPos() , player->getSize() , player->getRotation() ,tex);	
		glPopMatrix();
	}
	void RenderTorzo(RenderPass pass , Player* player )
	{
		Texture* tex;	
		if(pass==RP_DIFFUSE)
			tex = textura;	
		if(pass==RP_NORMAL)
			tex = texturaN;	
		glPushMatrix();	
		drawSprite( player->getPos() , player->getSize(), player->rotationAim + Math::toRad(90) , tex );			
		glPopMatrix();	
	}

	void RenderTrack( RenderPass pass , Player* player )
	{
		float razmak_tracnica=8;
		float odmak=24;

		Texture* tex;
		if(pass==RP_DIFFUSE)
			tex=tracnica_tex;
		if(pass==RP_NORMAL)
			tex=tracnica_normal;

		glEnable(GL_TEXTURE_2D);

		tex->bind();

		Vec2f centerPos = player->getCenterPos();
		float shift = player->shiftTrack;
		Vec2f size = player->getSize();

		glPushMatrix();
		glTranslatef( centerPos.x, centerPos.y , 0 );
		glRotatef(Math::toDegree( player->getRotation()),0,0,1);
		glTranslatef( -odmak - razmak_tracnica, -size.y/2,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, shift); glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, shift); glVertex2f(size.x/4, 0.0);
		glTexCoord2f(1.0, shift + 1.0); glVertex2f(size.x/4, size.y);	
		glTexCoord2f(0.0, shift + 1.0); glVertex2f(0.0, size.y);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef( centerPos.x, centerPos.y , 0 );
		glRotatef(Math::toDegree( player->getRotation()),0,0,1);
		glTranslatef( odmak - razmak_tracnica, -size.y/2,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, shift); glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, shift); glVertex2f(size.x/4, 0.0);
		glTexCoord2f(1.0, shift + 1.0); glVertex2f(size.x/4, size.y);	
		glTexCoord2f(0.0, shift + 1.0); glVertex2f(0.0, size.y);
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}

	Texture* textura;
	Texture* texturaN;
	Texture* tracnica_tex;
	Texture* tracnica_normal;
	Texture* podloga_tex;
	Texture* podloga_normal;

};

DEFINE_RENDERER( Player , PlayerRenderer )

Player::Player()
{
	for(int i=0; i<4; i++)
		weaponSlot[i] = NULL;
}
Player::~Player()
{
	
}

void Player::Init(Vec2f poz)
{
	this->mPos=poz;

	akceleracija = 0;

	mSize.x=64;
	mSize.y=64;

	for(int i=0; i<4; i++)
		weaponSlot[i]=NULL;

	hp=100;
	energy=100.0;
	haveShoot=false;
	mIsDead=false;

	shiftTrack=0.0;
	rotation=0.0;

	brzina=200;
}

void Player::onSpawn()
{
	BaseClass::onSpawn();

	light=getLevel()->createLight( Vec2f(0.0, 0.0), 1024 , false );
	light->setColorParam(Vec3(1.0, 1.0, 1.0), 16);
	light->drawShadow = true;
}

void Player::onDestroy()
{
	clearWeapons();
}

void Player::Update(float deltaT, Vec2f mis)
{
	if( !mIsDead )
	{
		Vec2f moment;
		float rad = rotation - Math::toRad( 90 );
		moment.x=cos(rad)*akceleracija;
		moment.y=sin(rad)*akceleracija;	

		mPos.y+=moment.y*brzina*deltaT;
		if(checkCollision())
			mPos.y-=moment.y*brzina*deltaT;
		mPos.x+=moment.x*brzina*deltaT;
		if(checkCollision())
			mPos.x-=moment.x*brzina*deltaT;

		if(moment.x!=0 || moment.y!=0)
		{
			if(akceleracija>0)
				shiftTrack+=deltaT;
			else
				shiftTrack-=deltaT;
			if(shiftTrack>1.0)
				shiftTrack=0.0;
			if(shiftTrack<0.0)
				shiftTrack=1.0;
		}
		akceleracija=0;

		SudarProjektila();

		Vec2f dir = mis - getCenterPos();
		rotationAim = atan2( dir.y , dir.x );

		if(mIsDead==false)
		{
			for(int i=0; i<4; i++)
			{
				if( weaponSlot[i] )
					weaponSlot[i]->update(deltaT);
			}
		}

		updateHeadlight();

		if( !haveShoot )
		{
			energy += 10*deltaT;
			if(energy>100.0)
				energy=100.0;		
		}
		haveShoot=false;

		if(hp<=0.0)
		{
			hp=0.0;
			mIsDead=true;

			Explosion* e= getLevel()->createExplosion( getCenterPos(),512 );
			e->setParam(256,3000,200);

			getLevel()->playSound("explozija1.wav");		

			Message* gameOverMsg = new Message();
			gameOverMsg->init("Base", "Svim jedinicama, izgubili smo kod Q.", 4, "blip.wav" );
			getLevel()->addMessage( gameOverMsg );	

			light->setColorParam(Vec3(0,0,0), 0);
		}
	}
}
void Player::updateHeadlight()
{
	Vec2f dir;
	float angle = rotationAim + Math::toRad(180);
	dir.x = cos( angle );
	dir.y = sin( angle );
	Math::normalize( dir );	
	light->PostavkeKuta(dir,2);

	Vec2f offset;
	offset.x=cos( rotationAim );
	offset.y=sin( rotationAim );
	light->changePos(getCenterPos()+ 34 * offset);
}

void Player::DodajMoment(float x)
{
	akceleracija=x;
}


void Player::Shoot(Vec2f posTaget, float deltaT)
{
	if( !mIsDead )
	{
		Vec2f dir = posTaget - getCenterPos();
		Math::normalize( dir );

		for(int i=0; i<4; i++)
		{		
			if( weaponSlot[i] == NULL )
				continue;

			if(energy>=weaponSlot[i]->getEnergyCast())
			{
				Vec2f offset = weaponSlot[i]->getPos();

				float angle = rotationAim + atan2(offset.y,offset.x) + Math::toRad( 90 );

				Vec2f slotDir;
				slotDir.x = cos( angle );
				slotDir.y = sin( angle );
				weaponSlot[i]->fire(getCenterPos()+ sqrt( offset.length2() ) * slotDir ,dir,TEAM_PLAYER);
				haveShoot=true;
			}
		}
	}
}
bool Player::checkCollision()
{
	Rect bBox;
	bBox.min=mPos+Vec2f(4,4);
	bBox.max=mPos+mSize-Vec2f(4,4);

	TileMap& terrian = getLevel()->getTerrain();

	if ( getLevel()->testTerrainCollision( bBox ) )
		return true;

	for( MobList::iterator iter = getLevel()->getMobs().begin() , itEnd = getLevel()->getMobs().end();
		iter != itEnd ; ++iter )
	{
		Mob* mob = *iter;

		Rect k2;
		k2.min = mob->getPos();
		k2.max = mob->getPos()+mob->getSize();
		if( bBox.intersect(k2) )
			return true;		
	}

	return false;
}
void Player::SudarProjektila()
{
	Rect k1;
	k1.min=mPos+Vec2f(4,4);
	k1.max=mPos+mSize-Vec2f(4,4);
	for( BulletList::iterator iter = getLevel()->getBullets().begin() , itEnd = getLevel()->getBullets().end();
		iter != itEnd ; ++iter )
	{
		Bullet* bt = *iter;
		if( bt->team == TEAM_EMPTY )
		{
			Rect k2;
			k2.min = bt->getPos();
			k2.max = bt->getPos()+ bt->getSize();
			if(k1.intersect(k2))
			{
				takeDamage( bt );				
			}
		}
	}
}

void Player::takeDamage(Bullet* p)
{
	if(mIsDead==false)
	{
		hp -= p->getDamage();
		p->destroy();
	}
}

void Player::addWeapon( Weapon* weapon )
{
	Vec2f const posSlot[] = 
	{
		Vec2f(-8,-12) ,
		Vec2f(8,-12), 
		Vec2f(-24,0),
		Vec2f(24,0) ,
	};
	for(int i=0; i<4; i++ )
	{
		if( weaponSlot[i] )
			continue;

		weapon->init( this );
		weapon->changePos( posSlot[i] );

		weaponSlot[i] = weapon;
		weapon = NULL;
		break;
	}

	if ( weapon )
	{
		delete weapon;
	}
}

bool Player::isDead()
{
	return mIsDead;
}

void Player::loseEnergy(float e)
{
	energy-=e;
	if(energy<0.0)
		energy=0.0;
}
void Player::DodajHP(float kolicina)
{
	hp+=kolicina;
	if(hp>100.0)
		hp=100.0;
}

void Player::clearWeapons()
{
	for(int i=0; i<4; i++)
	{
		if(weaponSlot[i])
		{
			delete weaponSlot[i];	
			weaponSlot[i]=NULL;
		}
	}
}
