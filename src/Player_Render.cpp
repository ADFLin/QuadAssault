#include "Player.h"
#include "RenderUtility.h"
#include "Level.h"
#include "Laser.h"
#include "Plasma.h"
#include "Light.h"


void RenderBar( float len , float h , float flac , float alpha , float colorAT[] , float colorAD[] , float colorBT[] , float colorBD[] )
{
	glEnable(GL_BLEND);
	
	if ( flac > 0 )
	{
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		float temp = len * flac;
		glBegin(GL_QUADS);
		glColor4f(colorAT[0],colorAT[1],colorAT[2],alpha); 
		glVertex2f( 0.0 , 0.0); glVertex2f( temp , 0.0);
		glColor4f(colorAD[0],colorAD[1],colorAD[2],alpha);
		glVertex2f( temp, h );  glVertex2f( 0.0  , h );
		glEnd();	
	}

	glBlendFunc(GL_ONE, GL_ONE);

	glBegin(GL_LINE_LOOP);
	glColor3fv(colorBT); glVertex2f( 0.0 , 0.0 );glVertex2f( len , 0.0 );
	glColor3fv(colorBD); glVertex2f( len , h );  glVertex2f( 0.0 , h );
	glEnd();

	glDisable(GL_BLEND);
	glColor3f(1.0, 1.0, 1.0);
}

void Player::RenderHP_Bar()
{
	float colorAT[] = { 0.0, 0.5, 0.0 };
	float colorAD[] = { 0.0, 0.1, 0.0 };
	float colorBT[] = { 0.0, 0.75, 0.0 };
	float colorBD[] = {	0.0, 0.25, 0.0 };
	RenderBar( 200 , 16 , hp  / 100.0 , 0.75 , colorAT , colorAD , colorBT , colorBD );

}

void Player::RenderEnergija_Bar()
{
	float colorAT[] = { 0.2, 0.2, 0.5 };
	float colorAD[] = { 0.05, 0.05, 0.1 };
	float colorBT[] = { 0.2, 0.3, 0.75 };
	float colorBD[] = {	0.1, 0.1, 0.25 };
	RenderBar( 200 , 16 , energy / 100.0 , 0.75 , colorAT , colorAD , colorBT , colorBD );
}