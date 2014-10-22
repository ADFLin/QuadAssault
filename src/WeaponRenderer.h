#ifndef WeaponRenderer_h__
#define WeaponRenderer_h__

#include "Renderer.h"

class Weapon;
class WeaponRenderer : public IRenderer
{
public:
	virtual void render( RenderPass pass , Weapon* weapon );
	Texture* mTextues[ NUM_RENDER_PASS ];
};

#define DEF_WEAPON_RENDERER( CLASS , RENDERER )\
	static RENDERER g##CLASS##Renderer;\
	WeaponRenderer* CLASS::getRenderer(){  return &g##CLASS##Renderer;  }



#endif // WeaponRenderer_h__
