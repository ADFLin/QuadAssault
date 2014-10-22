#include "Object.h"

Object::Object()
	:mPos( 0 , 0 )
{

}

Object::Object( Vec2f const& pos ) 
	:mPos( pos )
{

}

LevelObject::LevelObject() 
	:mSize( 0 , 0 )
{
	mNeedDestroy = false;
	mLevel = NULL;
}

LevelObject::LevelObject( Vec2f const& pos ) 
	:BaseClass( pos )
	,mSize( 0 , 0 )
{
	mNeedDestroy = false;
	mLevel = NULL;
}


void LevelObject::calcBoundBox( Rect& bBox )
{
	Vec2f half = mSize / 2;
	bBox.min = mPos - half;
	bBox.max = mPos + half;
}

void LevelObject::enumProp( IPropEditor& editor )
{
	editor.addProp( "Pos" , mPos );
}