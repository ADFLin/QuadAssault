#ifndef RenderUtility_h__
#define RenderUtility_h__

#include "MathCore.h"
class Texture;

void drawRect( Vec2f const& poz , Vec2f const& dim );
void drawSprite( Vec2f const& poz , Vec2f const& dim , Texture* tex);
void drawSprite( Vec2f const& poz , Vec2f const& dim , float rot, Texture* tex);

#endif // RenderUtility_h__