#include "GBuffer.h"


GBuffer::GBuffer()
{
	mFBO = 0;
	std::fill_n( mTexBuffers , (int)NUM_BUFFER_TYPE , GLuint(0) );
	mRBODepth = 0;
}

GBuffer::~GBuffer()
{
	if ( mTexBuffers[0] )
		glDeleteTextures( NUM_BUFFER_TYPE , mTexBuffers );
	if ( mRBODepth )
		glDeleteRenderbuffers( 1 , &mRBODepth );
	if ( mFBO )
		glDeleteFramebuffers( 1 , &mFBO );
}

bool GBuffer::create(int w , int h)
{
	glGenFramebuffers( 1, &mFBO ); 
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER , mFBO );

	glGenTextures( NUM_BUFFER_TYPE , mTexBuffers );
	for( int i = 0 ; i < NUM_BUFFER_TYPE ; ++i )
	{
		glBindTexture(GL_TEXTURE_2D, mTexBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w , h , 0, GL_RGBA , GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTexBuffers[i], 0);
	}

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) 
	{
		std::cerr << "FB error, status = " << Status << std::endl;
		return false;
	}

#if 0
	glGenRenderbuffers(1, &mRBODepth );  
	glBindRenderbuffer(GL_RENDERBUFFER, mRBODepth );  
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h );
#endif
	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::bind()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, mFBO );
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mFBODepth ); 

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	glDrawBuffers( NUM_BUFFER_TYPE , DrawBuffers );
}

void GBuffer::unbind()
{
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0 ); 
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}
