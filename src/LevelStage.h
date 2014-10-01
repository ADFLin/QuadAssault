#ifndef LevelState_h__
#define LevelState_h__

#include "GameStage.h"
#include "Level.h"

#include "Object.h"
#include "Dependence.h"
#include "RenderEngine.h"
#include "Tween.h"
#include <vector>

class SoundManager;
class GUIManager;


class LevelStage : public GameStage
	             , public Level
{
public:
	
	bool init();
	void update(float deltaT);	
	void UpdateDev(float deltaT);
	void render();
	void exit();

	void tick();
	void updateRender( float dt );


	void LoadLevel();
	bool saveMap( char const* path );

	void UpdateajNizove(float deltaT);



	Message*   addMessage(Message* p);


	void generateEmptyLevel();
	void createShader( char const* vsName , char const* fsName );

	void onWidgetEvent( int event , int id , GWidget* sender );
	void onSystemEvent( sf::Event const& event );
	void onSystemEventDev( sf::Event const& event );
private:

	Vec2f  convertToWorldPos( Vec2i const& sPos )
	{
		return mCamera->getPos() + mWorldScaleFactor * Vec2f( sPos.x  , sPos.y  );
	}

	Vec2i convertToTilePos( Vec2i const& sPos )
	{
		Vec2f wPos = convertToWorldPos( sPos );
		return Vec2i( Math::floor( wPos.x / BLOCK_SIZE ) , Math::floor( wPos.y  / BLOCK_SIZE ) );
	}

	enum
	{
		UI_MAP_TOOL = 100 ,
		UI_CREATE_LIGHT ,
		UI_CREATE_TRIGGER ,
		UI_EMPTY_MAP ,
		UI_SAVE_MAP  ,

		UI_MENU_PANEL ,
		UI_BACK_GAME ,
		UI_EXIT_GAME ,
		UI_GO_MENU ,
	};

	bool DEVMODE;

	Light* mEditLight;
	float sr,sg,sb, si, srad; //boja postavljenog svjetla

	int   mEditTileType; //vrsta bloka koji se postavlja
	int   mEditTileMeta;
	bool  postavljaLight;

	int      mStepEdit;
	AreaTrigger* mEditTrigger;



	Vec2f t1, t2, t3; //pocetak, kraj i spawnpoint triggera


	float gameOverTimer; //dok dosegne nulu, igra odlazi na glavni menu

	bool        mPause;
	RenderParam mRenderParam;
	Object*     mCamera;
	float       mWorldScaleFactor;


	sf::Music  mMusic;

	Texture* mTexCursor;
	

	std::vector<Message*> mMsgQueue;

	typedef Tween::GroupTweener< float > CTweener;

	CTweener mTweener;
	unsigned char tranzicija; //prelazak na tamno (fade to black, ne znak kak se to zove na hrvatskom)
	float tBoja; //boja tranzicije
	float brzinaFadeanja;

	float tickTimer;

	

};

#endif // LevelState_h__