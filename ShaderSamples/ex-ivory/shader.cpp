// textfile.cpp
//
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}



GLuint setShaders(const char *vfile, const char *ffile)
{
	GLuint v,f,f2,p;

	p = glCreateProgram();

	if (vfile) {
		v = glCreateShader(GL_VERTEX_SHADER);
		char *vs = textFileRead(vfile);
		const char * vv = vs;
		glShaderSource(v, 1, &vv,NULL);
		free(vs);	
		glCompileShader(v);
		printShaderInfoLog(v);
		glAttachShader(p,v);
	}

	if (ffile) {
		f = glCreateShader(GL_FRAGMENT_SHADER);
		char *fs = textFileRead(ffile);
		const char * ff = fs;
		glShaderSource(f, 1, &ff,NULL);
		free(fs);
		glCompileShader(f);
		printShaderInfoLog(f);
		glAttachShader(p,f);
	}

	glLinkProgram(p);
	printProgramInfoLog(p);

	return p;
}
