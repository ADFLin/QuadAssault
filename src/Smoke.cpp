#include "Smoke.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"
#include "Texture.h"


class SmokeRenderer : public IRenderer
{
public:
	virtual void init()
	{
		tex = getGame()->getTextureMgr()->getTexture("Dim1Diffuse.tga");
		texN= getGame()->getTextureMgr()->getTexture("Dim1Normal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		Smoke* smoke = object->cast< Smoke >();

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


DEFINE_RENDERER( Smoke , SmokeRenderer )

Smoke::Smoke( Vec2f const& pos ) 
	:BaseClass( pos )
{

}


void Smoke::init()
{
	BaseClass::init();
	maxZivot=45;
	zivot=maxZivot;
}

void Smoke::onSpawn()
{
	BaseClass::onSpawn();

	mPos.x += getLevel()->random(8,16)-8;
	mPos.y += getLevel()->random(8,16)-8;
}

void Smoke::tick()
{
	BaseClass::tick();
}
