#ifndef RenderUtility_h__
#define RenderUtility_h__

#include "MathCore.h"
class Texture;


void drawRect( Vec2f const& pos , Vec2f const& size );
void drawSprite( Vec2f const& pos , Vec2f const& size , Texture* tex);
void drawSprite( Vec2f const& pos , Vec2f const& size , float rot, Texture* tex);
void drawSprite(Vec2f const& pos, Vec2f const& size, float rot );
void drawRectLine( Vec2f const& pos , Vec2f const size );

#endif // RenderUtility_h__