#ifndef GLSLMANAGER_H
#define	GLSLMANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <QOpenGLFunctions_3_3_Core>

class GLSLManager
{
public:
	void LoadFromConstChar(const char* verSource, const char* fragSource="", const char* geoSource="", const char* tessControl="", const char* tessEval="");
	void UseProgram1();
	GLuint GetProgram1();
	
	void UniformMatrix4fv(GLfloat matrix[16], const char *name);
	void Uniform3fv(GLfloat vector[3], const char *name);
	void Uniform4fv(GLfloat vector[4], const char *name);
	void Uniform1f(GLfloat scalar, const char *name);
	void Uniform1i(GLint integer, const char *name);
		
	GLSLManager(QOpenGLFunctions_3_3_Core *openGLCore);
	virtual ~GLSLManager();
	
private:
	QOpenGLFunctions_3_3_Core *openGLCore;

	GLuint program1;
	
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint geometryShader;
	GLuint tessControlShader;
	GLuint tessEvalShader;
	
	void CompileShader(GLuint whichShader);
};

#endif	/* GLSLMANAGER_H */

