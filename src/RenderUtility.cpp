#include "RenderUtility.h"

#include "Texture.h"
#include "Base.h"

#include <cassert>

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
	glBindTexture(GL_TEXTURE_2D,tex->id );
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
	glPushMatrix();
	glTranslatef(poz.x+dim.x/2, poz.y+dim.y/2, 0);
	glRotatef( Math::toDegree( rot ),0,0,1);
	glTranslatef(-dim.x/2,-dim.y/2,0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->id );

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

