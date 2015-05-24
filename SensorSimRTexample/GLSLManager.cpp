#include "GLSLManager.h"

GLSLManager::GLSLManager(QOpenGLFunctions_3_3_Core *openGLCore)
{
	program1=vertexShader=fragmentShader=geometryShader=0;
	this->openGLCore=openGLCore;
}

GLSLManager::~GLSLManager()
{
}

void GLSLManager::LoadFromConstChar(const char* verSource, const char* fragSource, const char* geoSource, const char* tessControlSource, const char* tessEvalSource)
{
	if (verSource && strlen(verSource)>1)
	{
		vertexShader = openGLCore->glCreateShader(GL_VERTEX_SHADER);
		openGLCore->glShaderSource(vertexShader, 1, &verSource, NULL);
		
		CompileShader(vertexShader);
	}
	
	if (fragSource && strlen(fragSource)>1)
	{
		fragmentShader = openGLCore->glCreateShader(GL_FRAGMENT_SHADER);
		openGLCore->glShaderSource(fragmentShader, 1, &fragSource, NULL);
		
		CompileShader(fragmentShader);
	}
	
	if (geoSource && strlen(geoSource)>1)
	{
		geometryShader = openGLCore->glCreateShader(GL_GEOMETRY_SHADER);
		openGLCore->glShaderSource(geometryShader, 1, &geoSource, NULL);
		
		CompileShader(geometryShader);
	}	
		
	if (tessControlSource && strlen(tessControlSource)>1)
	{
		tessControlShader = openGLCore->glCreateShader(GL_TESS_CONTROL_SHADER);
		openGLCore->glShaderSource(tessControlShader, 1, &tessControlSource, NULL);
		
		CompileShader(tessControlShader);
	}
	
	if (tessEvalSource && strlen(tessEvalSource)>1)
	{
		tessEvalShader = openGLCore->glCreateShader(GL_TESS_EVALUATION_SHADER);
		openGLCore->glShaderSource(tessEvalShader, 1, &tessEvalSource, NULL);
		
		CompileShader(tessEvalShader);
	}
	
	program1 = openGLCore->glCreateProgram();
	
	if (geometryShader!=0)
		openGLCore->glAttachShader(program1,geometryShader);
	
	if (fragmentShader!=0)
		openGLCore->glAttachShader(program1,fragmentShader);

	if (vertexShader!=0)
		openGLCore->glAttachShader(program1,vertexShader);
		
	if (tessEvalShader!=0)
		openGLCore->glAttachShader(program1,tessEvalShader);

	if (tessControlShader!=0)
		openGLCore->glAttachShader(program1,tessControlShader);
	
	openGLCore->glLinkProgram(program1);
}


void GLSLManager::CompileShader(GLuint whichShader)
{
	printf("Compiling shader %d\n",whichShader);

	openGLCore->glCompileShader(whichShader);
	
	int status=0;
	
	openGLCore->glGetShaderiv(whichShader,GL_COMPILE_STATUS,&status);
	
	if(status!=GL_TRUE)
		printf("ERRORS:\n");
	
	GLint bufferlen = 0;	
	GLsizei slen = 0;
	
	openGLCore->glGetShaderiv(whichShader, GL_INFO_LOG_LENGTH , &bufferlen);       
	
	if (bufferlen > 1)
	{
		GLchar* compiler_log = (GLchar*)malloc(bufferlen);
		
		openGLCore->glGetShaderInfoLog(whichShader, bufferlen, &slen, compiler_log);
		
		printf("%s\n", compiler_log);
		
		free(compiler_log);
		system("pause");
		exit(-1);
	}
}

void GLSLManager::UseProgram1()
{
	openGLCore->glUseProgram(program1);
}


GLuint GLSLManager::GetProgram1()
{
	return program1;	
}

void GLSLManager::UniformMatrix4fv(GLfloat matrix[16], const char *name)
{
	GLint loc;
	
	if (program1)
	{
		openGLCore->glUseProgram(program1);

		loc = openGLCore->glGetUniformLocation(program1,name);

		openGLCore->glUniformMatrix4fv(loc, 1, false, matrix);
	}
}

void GLSLManager::Uniform3fv(GLfloat vector[3], const char *name)
{
	GLint loc;
	
	if (program1)
	{
		openGLCore->glUseProgram(program1);

		loc = openGLCore->glGetUniformLocation(program1,name);

		openGLCore->glUniform3fv(loc,1,vector);
	}
}

void GLSLManager::Uniform4fv(GLfloat vector[4], const char *name)
{
	GLint loc;
	
	if (program1)
	{
		openGLCore->glUseProgram(program1);

		loc = openGLCore->glGetUniformLocation(program1,name);

		openGLCore->glUniform4fv(loc,1,vector);
	}
}

void GLSLManager::Uniform1f(GLfloat scalar, const char *name)
{
	GLint loc;
	
	if (program1)
	{
		openGLCore->glUseProgram(program1);

		loc = openGLCore->glGetUniformLocation(program1,name);

		openGLCore->glUniform1f(loc,scalar);
	}
}

void GLSLManager::Uniform1i(GLint integer, const char *name)
{
	GLint loc;
	
	if (program1)
	{
		openGLCore->glUseProgram(program1);

		loc = openGLCore->glGetUniformLocation(program1,name);

		openGLCore->glUniform1i(loc,integer);
	}
}
