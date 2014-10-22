#ifndef RenderSystem_h__
#define RenderSystem_h__

#include "Base.h"
#include "Platform.h"

#include "TVector2.h"

class Shader;
class TextureManager;
typedef TVector2< float > Vec2f;

class IFont
{
public:
	virtual void release() = 0;
public:
	static IFont* loadFont( char const* path );
};

class IText
{
public:
	virtual Vec2f getBoundSize() const = 0;
	virtual void  setString( char const* str ) = 0;
	virtual void  setColor( Color const& color ) = 0;
	virtual void  setFont( IFont* font ) = 0;
	virtual void  setCharSize( int size ) = 0;
	virtual void  release() = 0;
public:
	static IText* create( IFont* font , int size , Color const& color );
	static IText* create();
};

enum
{
	TEXT_SIDE_LEFT   = BIT(0),
	TEXT_SIDE_RIGHT  = BIT(1),
	TEXT_SIDE_TOP    = BIT(2),
	TEXT_SIDE_BOTTOM = BIT(3),
};

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	bool init( GameWindow& window );
	void cleanup();
	bool prevRender();
	void postRender();

	Shader* createShader( char const* vsName , char const* fsName );
	void    removeShader( Shader* shader );

	TextureManager* getTextureMgr(){ return mTextureMgr; }
	void drawText(  IText* text , Vec2f const& pos ,unsigned sideFlag = 0 );

private:

	typedef std::vector<Shader*> ShaderVec;

	ShaderVec mShaders;
	TextureManager* mTextureMgr;

#if USE_SFML_WINDOW
	sf::RenderWindow* mRenderWindow;
#else
	GameWindow* mRenderWindow;
#endif
	GLContext*  mContext;
};

RenderSystem* getRenderSystem();

#endif // RenderSystem_h__
