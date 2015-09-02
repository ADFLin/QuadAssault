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
	void bind();

	void unbind();

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
