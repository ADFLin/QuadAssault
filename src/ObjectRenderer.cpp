#include "ObjectRenderer.h"

#include "RenderSystem.h"
#include "TextureManager.h"
#include "RenderUtility.h"
#include "Texture.h"

#include "Player.h"
#include "Mob.h"
#include "LaserMob.h"
#include "MinigunMob.h"
#include "PlasmaMob.h"

#include "LaserBullet.h"
#include "MinigunBullet.h"
#include "PlasmaBullet.h"

#include "SmokeParticle.h"
#include "DebrisParticle.h"

#include "KeyPickup.h"
#include "WeaponPickup.h"
#include "DebrisPickup.h"


IObjectRenderer::IObjectRenderer()
{
	mRenderOrder = 0;
}


void IObjectRenderer::renderGroup( RenderPass pass , int numObj, LevelObject* object )
{
	for( ; object ; object = nextObject( object ) )
	{
		render( pass , object );
	} 
}

class PlayerRenderer : public IObjectRenderer
{
public:
	PlayerRenderer()
	{
		mRenderOrder = 2;
	}
	virtual void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
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
			//Shadow
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
			//
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
			Weapon* weapon = player->mWeaponSlot[i];
			if( weapon )
			{
				Vec2f centerPos = player->getPos();

				glPushMatrix();
				glTranslatef( centerPos.x,centerPos.y,0 );
				glRotatef( Math::toDeg( player->rotationAim ) + 90 ,0,0,1 );
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
		drawSprite( player->getRenderPos() , player->getSize() , player->getRotation() ,tex);	
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
		drawSprite( player->getRenderPos() , player->getSize(), player->rotationAim + Math::toRad(90) , tex );			
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

		Vec2f centerPos = player->getPos();
		float shift = player->shiftTrack;
		Vec2f size = player->getSize();

		glPushMatrix();
		glTranslatef( centerPos.x, centerPos.y , 0 );
		glRotatef(Math::toDeg( player->getRotation()),0,0,1);
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
		glRotatef(Math::toDeg( player->getRotation()),0,0,1);
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

class MobRenderer : public IObjectRenderer
{
public:
	MobRenderer()
	{
		mRenderOrder = 1;
	}
	virtual void render( RenderPass pass , LevelObject* object );
	virtual void renderGroup( RenderPass pass , int numObj, LevelObject* object );

protected:
	Texture* mTextures[ NUM_RENDER_PASS ];
};


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


}

void MobRenderer::renderGroup( RenderPass pass , int numObj, LevelObject* object )
{

	Texture* tex = mTextures[ pass ];

	switch( pass )
	{
	case RP_DIFFUSE: 
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();		
		glColor4f(0.0, 0.0, 0.0, 0.6);

		for( LevelObject* cur = object; cur ; cur = nextObject( cur ) )
		{
			Mob* mob = static_cast< Mob* >( cur );
			drawSprite( mob->getRenderPos() + Vec2f(5,5), mob->getSize() , mob->getRotation() , tex );	
		}

		glColor4f(1.0, 1.0, 1.0, 1.0);
		glPopMatrix();
		glDisable(GL_BLEND);
		break;
	}

	glPushMatrix();	
	for( LevelObject* cur = object; cur ; cur = nextObject( cur ) )
	{
		Mob* mob = static_cast< Mob* >( cur );
		drawSprite( mob->getRenderPos() , mob->getSize() , mob->getRotation() , tex );	
	}
	glPopMatrix();
}

class PlasmaMobRenderer : public MobRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTextures[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTextures[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		mTextures[ RP_GLOW    ] = texMgr->getTexture("mob2Glow.tga");
	}
};

class LaserMobRenderer : public MobRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTextures[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTextures[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		mTextures[ RP_GLOW    ] = texMgr->getTexture("mob1Glow.tga");
	}
};

class MinigunMobRenderer : public MobRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTextures[ RP_DIFFUSE ] = texMgr->getTexture("mob1Diffuse.tga");
		mTextures[ RP_NORMAL  ] = texMgr->getTexture("mob1Normal.tga");
		mTextures[ RP_GLOW    ] = texMgr->getTexture("mob3Glow.tga");
	}
};

class BulletRenderer : public IObjectRenderer
{
public:
	BulletRenderer()
	{
		mRenderOrder = 3;
	}
};

class LaserBulletRenderer : public BulletRenderer
{
public:
	virtual void init()
	{
		texG = getRenderSystem()->getTextureMgr()->getTexture("laser1Glow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass !=RP_GLOW )
			return;

		LaserBullet* bullet = object->cast< LaserBullet >();

		Vec2f size = Vec2f(16,32);
		float rot= Math::atan2( bullet->mDir.y, bullet->mDir.x ) + Math::toRad( 90 );
		drawSprite( bullet->getPos() - size / 2 , size , rot , texG );		
	}
	Texture* texG;
};


class MinigunBulletRenderer : public BulletRenderer
{
public:
	virtual void init()
	{
		texG = getRenderSystem()->getTextureMgr()->getTexture("minigun1Glow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass !=RP_GLOW )
			return;

		MinigunBullet* bullet = object->cast< MinigunBullet >();

		Vec2f size = Vec2f(16,32);
		float rot= Math::atan2( bullet->mDir.y, bullet->mDir.x ) + Math::toRad( 90 );
		drawSprite( bullet->getPos() - size / 2 , size , rot , texG );

	}
	Texture* texG;
};

class PlasmaBulletRenderer : public BulletRenderer
{
public:
	virtual void init()
	{
		tex = getRenderSystem()->getTextureMgr()->getTexture("granata1.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass !=RP_DIFFUSE )
			return ;

		drawSprite( object->getRenderPos() , object->getSize() ,0,tex);
	}
	Texture* tex;
};

class SmokeRenderer : public IObjectRenderer
{
public:
	virtual void init()
	{
		tex = getRenderSystem()->getTextureMgr()->getTexture("Dim1Diffuse.tga");
		texN= getRenderSystem()->getTextureMgr()->getTexture("Dim1Normal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		SmokeParticle* smoke = object->cast< SmokeParticle >();

		if(pass==RP_DIFFUSE)// || pass==NORMAL)
		{
			Texture* t;
			if(pass==RP_DIFFUSE)
				t=tex;
			if(pass==RP_NORMAL)
				t=texN;	

			float faktorSkaliranja = 0.5+ 0.5 * ( smoke->maxZivot / smoke->zivot );
			if( faktorSkaliranja > 2 )
				faktorSkaliranja = 2;

			float faktorBoje=smoke->zivot/smoke->maxZivot;

			Vec2f center = smoke->getPos();
			glPushMatrix();
			glTranslatef( center.x , center.y , 0);	
			glScalef(faktorSkaliranja,faktorSkaliranja,0);
			glTranslatef( -smoke->getSize().x ,-smoke->getSize().y ,0 );	

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glColor3f(faktorBoje,faktorBoje,faktorBoje);

			glEnable(GL_TEXTURE_2D);
			t->bind();
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex2f(smoke->getSize().x, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex2f(smoke->getSize().x, smoke->getSize().y);
			glTexCoord2f(0.0, 1.0); glVertex2f(0.0, smoke->getSize().y);
			glEnd();
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);	

			glDisable(GL_BLEND);
			glPopMatrix();	

			glColor3f(1.0, 1.0, 1.0);	
		}
	}

	Texture* tex;
	Texture* texN;
};


class DebrisParticleRenderer : public IObjectRenderer
{
public:

	virtual void init()
	{
		tex = getRenderSystem()->getTextureMgr()->getTexture("Dim1Diffuse.tga");
		texN= getRenderSystem()->getTextureMgr()->getTexture("Dim1Normal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		DebrisParticle* particle = object->cast< DebrisParticle >();
		Texture* t;
		if(pass==RP_DIFFUSE)
			t=tex;
		if(pass==RP_NORMAL)
			t=texN;
		if(pass==RP_DIFFUSE)// || pass==NORMAL)
		{
			float factor = particle->zivot / particle->maxZivot;
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glColor3f( factor , factor , factor );
			drawSprite( particle->getRenderPos() , particle->getSize() , 0 ,t );
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_BLEND);
		}
	}

	virtual void renderGroup( RenderPass pass , int numObj , LevelObject* object )
	{
		Texture* t;
		if(pass==RP_DIFFUSE)
			t=tex;
		if(pass==RP_NORMAL )
			t=texN;
		if(pass==RP_DIFFUSE)// || pass==NORMAL)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			for( ; object ; object = nextObject( object ) )
			{
				DebrisParticle* particle = object->cast< DebrisParticle >();
				float factor = particle->zivot / particle->maxZivot;
				glColor3f( factor , factor , factor );
				drawSprite( particle->getRenderPos() , particle->getSize() , 0 ,t );
			}
			glColor3f(1.0, 1.0, 1.0);
			glDisable(GL_BLEND);
		}

	}

	Texture* tex;
	Texture* texN;
};


class DebrisPickupRenderer : public IObjectRenderer
{
public:
	virtual void init()
	{
		tex = getRenderSystem()->getTextureMgr()->getTexture("SmeceDiffuse.tga");
		texN= getRenderSystem()->getTextureMgr()->getTexture("SmeceNormal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		if( pass == RP_GLOW )
			return;

		Texture* t;
		if(pass==RP_DIFFUSE)
			t=tex;
		else if(pass==RP_NORMAL)
			t=texN;

		glColor3f(1.0, 1.0, 1.0);
		drawSprite( object->getRenderPos() , object->getSize() , 0 , t );
		glColor3f(1.0, 1.0, 1.0);
	}

	Texture* tex;
	Texture* texN;
};


class WeaponPickupRenderer : public IObjectRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTex[ WEAPON_LASER ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ WEAPON_LASER ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ WEAPON_LASER ][ RP_GLOW    ] = texMgr->getTexture("oruzje1Glow.tga");

		mTex[ WEAPON_PLAZMA ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ WEAPON_PLAZMA ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ WEAPON_PLAZMA ][ RP_GLOW    ] = texMgr->getTexture("oruzje2Glow.tga");

		mTex[ WEAPON_MINIGUN ][ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTex[ WEAPON_MINIGUN ][ RP_NORMAL  ] = texMgr->getTexture("weapon1Normal.tga");
		mTex[ WEAPON_MINIGUN ][ RP_GLOW    ] = texMgr->getTexture("oruzje3Glow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		WeaponPickup* pickup = static_cast< WeaponPickup* >( object );
		drawSprite( pickup->getRenderPos() + Vec2f( pickup->getSize().x/2-8,0),Vec2f(16,32), pickup->mRotation , mTex[ pickup->mId ][ pass ] );
	}
	Texture* mTex[ NUM_WEAPON_ID ][ NUM_RENDER_PASS ];
};


class KeyPickupRenderer : public IObjectRenderer
{
public:
	virtual void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTex[ RP_DIFFUSE ] = texMgr->getTexture("KeyDiffuse.tga");
		mTex[ RP_NORMAL  ] = texMgr->getTexture("KeyNormal.tga");	
		mTex[ RP_GLOW    ] = texMgr->getTexture("KeyGlow.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		KeyPickup* key = static_cast< KeyPickup* >( object );
		if ( pass == RP_GLOW )
		{
			glColor3fv( getDoorColor( key->mId ) );
		}
		drawSprite(key->getRenderPos(),key->getSize(),key->mRotation, mTex[ pass ] );
		glColor3f(1.0, 1.0, 1.0);
	}

	Texture* mTex[ NUM_RENDER_PASS ];
};




DEF_OBJ_RENDERER( Player , PlayerRenderer )

DEF_OBJ_RENDERER( PlasmaMob , PlasmaMobRenderer )
DEF_OBJ_RENDERER( LaserMob , LaserMobRenderer )
DEF_OBJ_RENDERER( MinigunMob , MinigunMobRenderer )

DEF_OBJ_RENDERER( LaserBullet , LaserBulletRenderer )
DEF_OBJ_RENDERER( MinigunBullet , MinigunBulletRenderer )
DEF_OBJ_RENDERER( PlasmaBullet , PlasmaBulletRenderer )

DEF_OBJ_RENDERER( SmokeParticle , SmokeRenderer )
DEF_OBJ_RENDERER( DebrisParticle , DebrisParticleRenderer )

DEF_OBJ_RENDERER( DebrisPickup , DebrisPickupRenderer )
DEF_OBJ_RENDERER( WeaponPickup , WeaponPickupRenderer )
DEF_OBJ_RENDERER( KeyPickup , KeyPickupRenderer )