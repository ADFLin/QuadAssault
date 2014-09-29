#ifndef GameStage_h__
#define GameStage_h__

namespace sf
{
	class Event;
}

enum StateTransition
{
	ST_NONE    ,
	ST_FADEIN  , 
	ST_FADEOUT ,
};


class Game;
class GWidget;

class GameStage
{
public:		
	virtual bool init()=0;
	virtual void exit()=0;	
	virtual void update(float deltaT)=0;	
	virtual void render()=0;

	virtual void onSystemEvent( sf::Event const& event ){}
	virtual void onWidgetEvent( int event , int id , GWidget* sender ){}
		
	bool  isEnd(){ return mNeedExit; }

protected:
	bool  mNeedExit;	
};

#endif // GameStage_h__