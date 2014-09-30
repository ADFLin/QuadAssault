#include "MineParticle.h"

#include "GameInterface.h"
#include "Level.h"
#include "TextureManager.h"

class MineParticleRenderer : public IRenderer
{
public:

	virtual void init()
	{
		tex = getGame()->getTextureMgr()->getTexture("Dim1Diffuse.tga");
		texN= getGame()->getTextureMgr()->getTexture("Dim1Normal.tga");
	}

	virtual void render( RenderPass pass , LevelObject* object )
	{
		MineParticle* particle = object->cast< MineParticle >();
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

	Texture* tex;
	Texture* texN;
};

DEFINE_RENDERER( MineParticle , MineParticleRenderer )


void MineParticle::Init(Vec2f poz)
{
	BaseClass::Init(poz);
	maxZivot=45;
	zivot=maxZivot;	
}

void MineParticle::onSpawn()
{
	BaseClass::onSpawn();

}

void MineParticle::tick()
{
	BaseClass::tick();
}

