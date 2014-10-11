#include "RenderUtility.h"

#include "Texture.h"
#include "Base.h"

#include <cassert>

bool  gShowBoundBox = false;

void drawRect( Vec2f const& poz, Vec2f const& dim )
{
	glBegin(GL_QUADS);
	glVertex2f(poz.x, poz.y);
	glVertex2f(poz.x+dim.x, poz.y);
	glVertex2f(poz.x+dim.x, poz.y+dim.y);
	glVertex2f(poz.x, poz.y+dim.y);
	glEnd();
}

void drawSprite(Vec2f const& poz, Vec2f const& dim, Texture* tex)
{
	assert( tex );
	glEnable(GL_TEXTURE_2D);

	tex->bind();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(poz.x, poz.y);
	glTexCoord2f(1.0, 0.0); glVertex2f(poz.x+dim.x, poz.y);
	glTexCoord2f(1.0, 1.0); glVertex2f(poz.x+dim.x, poz.y+dim.y);
	glTexCoord2f(0.0, 1.0); glVertex2f(poz.x, poz.y+dim.y);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}

void drawSprite(Vec2f const& poz, Vec2f const& dim, float rot, Texture* tex )
{
	Vec2f hotspot = dim / 2;
	glPushMatrix();
	glTranslatef( poz.x + hotspot.x , poz.y + hotspot.y , 0);
	glRotatef( Math::toDeg( rot ),0,0,1);
	glTranslatef( -hotspot.x , -hotspot.y , 0 );
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL,1.0f);
	glEnable(GL_TEXTURE_2D);

	tex->bind();

	//float matrica[16];
	//glGetFloatv(GL_MODELVIEW, matrica);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex2f(dim.x, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex2f(dim.x, dim.y);
	glTexCoord2f(0.0, 1.0); glVertex2f(0.0, dim.y);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void drawRectLine( Vec2f const& pos , Vec2f const size )
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos.x         , pos.y       );
	glVertex2f(pos.x + size.x, pos.y       );
	glVertex2f(pos.x + size.x, pos.y+size.y);
	glVertex2f(pos.x         , pos.y+size.y);
	glEnd();	
}

