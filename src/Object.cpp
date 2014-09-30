#include "Object.h"

Object::Object()
{
	mPos  = Vec2f(0,0);
	mSize = Vec2f(0,0);
}

void Object::calcBoundBox( Rect& bBox )
{
	Vec2f half = mSize / 2;
	bBox.min = mPos - half;
	bBox.max = mPos + half;
}


IRenderer* gLink;

struct RendererInitHelper
{
	RendererInitHelper(){ gLink = NULL; }
};

IRenderer::IRenderer()
{
	static RendererInitHelper helper;
	mLink = gLink;
	gLink = this;
}

void IRenderer::cleanup()
{

}

void IRenderer::initialize()
{
	IRenderer* link = gLink;
	while( link )
	{
		link->init();
		link = link->mLink;
	}
}
