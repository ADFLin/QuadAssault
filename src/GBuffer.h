#ifndef GBuffer_h__
#define GBuffer_h__

#include "Base.h"
#include "Dependence.h"

#include <iostream>

class GBuffer
{
public:
	GBuffer();

	~GBuffer();

	bool create( int w , int h );
	void bind()
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, mFBO );
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mFBODepth ); 

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
		glDrawBuffers( NUM_BUFFER_TYPE , DrawBuffers );

	}

	void unbind()
	{
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0 ); 
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	}

	GLuint getTexture( int idx ){ return mTexBuffers[idx]; }

	enum BufferType
	{
		eBASE_COLOR ,
		eNORMAL  ,
		eLIGHTING ,
		NUM_BUFFER_TYPE ,
	};

	GLuint mTexBuffers[ NUM_BUFFER_TYPE ];

	GLuint mFBO;
	GLuint mRBODepth;
};

#endif // GBuffer_h__
