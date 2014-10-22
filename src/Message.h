#ifndef Message_h__
#define Message_h__

#include "Base.h"
#include "RenderSystem.h"

class Level;
class Sound;
class Texture;
class IText;

class Message
{

public:
	~Message();
	void init( String const& sender, String const& content, float durstion, String const& soundName );
	void nodifyShow();
	void tick();
	void updateRender( float dt );


	void renderFrame();
	void render();
	bool unisten;
private:	
	FObjectPtr< IText >  p_text;
	FObjectPtr< IText >  text;
	Vec2f   mPos;
	float   mDurstion;
	float    timer;	
	Texture* portrait;
	String   mSoundName;	
	Sound*   sound;

};


#endif // Message_h__