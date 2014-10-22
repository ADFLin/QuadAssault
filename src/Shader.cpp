#include "Shader.h"

#include <iostream>
#include <iomanip>
#include <fstream>

Shader::Shader()
{
	ID = -1;
	vertex_program = -1;
	fragment_program = -1;
}

Shader::~Shader()
{
	if ( ID != -1 )
	{
		glDetachShader(ID,vertex_program);
		glDetachShader(ID,fragment_program);
		glDeleteProgram(ID);
	}

	if ( vertex_program != -1 )
		glDeleteShader(vertex_program);
	if ( fragment_program != -1 )
		glDeleteShader(fragment_program);
}

bool Shader::create( char const* vertex_program_file, char const* fragment_program_file )
{
	vertex_program  = glCreateShader(GL_VERTEX_SHADER);
	if ( !compileShader( vertex_program , vertex_program_file) )
		return false;

	fragment_program = glCreateShader(GL_FRAGMENT_SHADER);
	if ( !compileShader( fragment_program , fragment_program_file )) 
		return false;

	ID=glCreateProgram();

	glAttachShader(ID,vertex_program);
	glAttachShader(ID,fragment_program);
	glLinkProgram(ID);

	Log(ID);	
	return true;
}

void Shader::bind()
{
	glUseProgram(ID);
}

void Shader::unbind()
{
	glUseProgram(0);
}

bool Shader::compileShader( GLuint shader , char const* path)
{
	std::ifstream fs( path );
	if ( !fs.is_open() )
		return false;

	std::string contents((std::istreambuf_iterator<char>(fs)),
		std::istreambuf_iterator<char>());

	GLchar const * source = (GLchar const*)contents.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if ( !status )
	{
		Log( shader );
		return false;
	}

	return true;
}

void Shader::Log(GLuint obj)
{
	int infologLength = 0;
	int maxLength;
 
	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
 
	char* infoLog=(char*)malloc(sizeof(char)*maxLength);
 
	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
 
	if ( infologLength > 0)
	{
		infoLog[ infologLength - 2 ] = '\0';
		std::cout << "Shader Id = " << std::setw( 2 ) << obj << " : " << infoLog << std::endl;
	}
	::free( infoLog );
}
