#include "Object.h"

Object::Object()
{

}

Object::~Object()
{	

}

void Object::changePos(Vec2f const& poz)
{
	mPos = poz;
}

Vec2f Object::getCenterPos()
{
	return mPos+Vec2f(mSize.x/2,mSize.y/2);
}

void Object::changeSize(Vec2f const& dim)
{
	this->mSize=dim;
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
