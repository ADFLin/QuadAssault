#include "Renderer.h"

static IRenderer* gLink;
static bool beInitilized  = false;

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
	if ( beInitilized )
		return;

	IRenderer* link = gLink;
	while( link )
	{
		link->init();
		link = link->mLink;
	}
	beInitilized = true;
}
