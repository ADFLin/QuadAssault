#ifndef PORUKA_H
#define PORUKA_H

#include "Dependence.h"
#include "Object.h"

class Level;
class Sound;

class Message
{
private:	
	sf::Text p_text;
	sf::Text text;
	Vec2f  mPos;
	float  mDurstion; //trajanje poruke u sekundama

	float    timer; //pocinje od nule	
	Texture* portret;
	string   mSoundName;	
	bool     playSound;
	Sound*   sound;

public:
	void init( string const& sender, string const& content, float durstion, string const& soundName );
	void Update(float deltaT);
	void RenderOkvir();
	void Render();
	bool unisten;
};

#endif