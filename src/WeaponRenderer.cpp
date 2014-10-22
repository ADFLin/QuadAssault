#include "WeaponRenderer.h"

#include "RenderSystem.h"
#include "TextureManager.h"
#include "Texture.h"

#include "Minigun.h"
#include "Laser.h"
#include "Plasma.h"

void WeaponRenderer::render( RenderPass pass , Weapon* weapon )
{
	float factor = std::min( 1.0f , weapon->mFireTimer / weapon->mCDTime );

	Vec2f rPos = weapon->getPos() - weapon->mSize / 2;

	float off;
	float len = 20;
	if ( factor < 0.5 )
	{
		off = len * factor;
	}
	else
	{
		off = len * ( 1 - factor );
	}

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);
	glEnable(GL_TEXTURE_2D);

	mTextues[ pass ]->bind();

	glPushMatrix();
	glTranslatef( rPos.x , rPos.y + off , 0 );

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(0,0);	
	glTexCoord2f(1.0, 0.0); glVertex2f(weapon->mSize.x,0);	
	glTexCoord2f(1.0, 1.0); glVertex2f(weapon->mSize.x,weapon->mSize.y);	
	glTexCoord2f(0.0, 1.0); glVertex2f(0,weapon->mSize.y);	
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

}


class LaserRenderer : public WeaponRenderer
{
public:
	void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTextues[ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTextues[ RP_NORMAL ]  = texMgr->getTexture("weapon1Normal.tga");
		mTextues[ RP_GLOW ]    = texMgr->getTexture("oruzje1Glow.tga");
	}
};


class MinigunRenderer : public WeaponRenderer
{
public:
	void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTextues[ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTextues[ RP_NORMAL ]  = texMgr->getTexture("weapon1Normal.tga");
		mTextues[ RP_GLOW ]    = texMgr->getTexture("oruzje3Glow.tga");
	}
};

class PlasmaRenderer : public WeaponRenderer
{
public:
	void init()
	{
		TextureManager* texMgr = getRenderSystem()->getTextureMgr();
		mTextues[ RP_DIFFUSE ] = texMgr->getTexture("weapon1.tga");
		mTextues[ RP_NORMAL ]  = texMgr->getTexture("weapon1Normal.tga");
		mTextues[ RP_GLOW ]    = texMgr->getTexture("oruzje2Glow.tga");
	}
};


DEF_WEAPON_RENDERER( Laser , LaserRenderer )
DEF_WEAPON_RENDERER( Minigun , MinigunRenderer )
DEF_WEAPON_RENDERER( Plasma , PlasmaRenderer )