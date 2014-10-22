#ifndef Shader_h__
#define Shader_h__

#include "Dependence.h"
#include "MathCore.h"

class Shader
{
public:
	Shader();
	~Shader();

	bool create( char const* vertex_program_file, char const* fragment_program_file );

	void bind();
	void unbind();

	void setParam( char const* name , Vec2f const& v )
	{
		int loc = glGetUniformLocation( ID , name );
		glUniform2f( loc , v.x , v.y );	
	}

	void setParam( char const* name , Vec3f const& v )
	{
		int loc = glGetUniformLocation( ID , name );
		glUniform3f( loc , v.x, v.y , v.z );	
	}

	void setParam( char const* name , float v1 )
	{
		int loc = glGetUniformLocation( ID , name );
		glUniform1f( loc , v1 );	
	}
	void setParam( char const* name , float v1 , float v2 )
	{
		int loc = glGetUniformLocation( ID , name );
		glUniform2f( loc , v1, v2 );	
	}

	void setParam( char const* name , int v1 )
	{
		int loc = glGetUniformLocation( ID , name );
		glUniform1i( loc , v1 );	
	}

	void setTexture2D( char const* name , GLuint idTex , int idx )
	{
		glActiveTexture( GL_TEXTURE0 + idx );
		glBindTexture( GL_TEXTURE_2D, idTex );
		setParam( name , idx );
		glActiveTexture( GL_TEXTURE0 );
	}


	void setParam( int loc , Vec2f const& v ){  glUniform2f( loc , v.x , v.y );	 }
	void setParam( int loc , Vec3f const& v ){  glUniform3f( loc , v.x, v.y , v.z );  }
	void setParam( int loc , float v1 ){  glUniform1f( loc , v1 );	}
	void setParam( int loc, float v1 , float v2 ){  glUniform2f( loc , v1, v2 );  }
	void setParam( int loc , int v1 ){  glUniform1i( loc , v1 );	}
	void setTexture2D( int loc , GLuint idTex , int idx )
	{
		glActiveTexture( GL_TEXTURE0 + idx );
		glBindTexture( GL_TEXTURE_2D, idTex );
		setParam( loc , idx );
		glActiveTexture( GL_TEXTURE0 );
	}
	void setTexture1D( int loc , GLuint idTex , int idx )
	{
		glActiveTexture( GL_TEXTURE0 + idx );
		glBindTexture( GL_TEXTURE_1D, idTex );
		setParam( loc , idx );
		glActiveTexture( GL_TEXTURE0 );
	}

	int  getParamLoc( char const* name ){ return glGetUniformLocation( ID , name ); }

private:

	void Log(GLuint obj);

	bool compileShader( GLuint shader , char const* path );

	GLuint ID;
	GLuint vertex_program;
	GLuint fragment_program;
};


#endif // Shader_h__