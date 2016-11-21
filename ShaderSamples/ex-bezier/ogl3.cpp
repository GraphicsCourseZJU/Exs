/*


Simple Demo for GLSL

www.lighthouse3d.com

*/
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "shader.h"
#include <math.h>

GLuint p, hNum=-1;
int gw, gh;
int num = 64;

void changeSize(int w, int h)
{
	gw = w, gh = h;

	glViewport(0,0,w,h);
	//Go to projection mode and load the identity
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Orthographic projection, stretched to fit the screen dimensions 
	gluOrtho2D(0, w, 0, h);
	//Go back to modelview and load the identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void idle(void)
{
	glutPostRedisplay();
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glUniform1i(hNum, num);

	glUseProgram(0);
	glBegin(GL_LINE_STRIP);
		glVertex2i(0, 0);
		glVertex2i(gw*2/3, gh/8);
		glVertex2i(gw/3, gh*7/8);
		glVertex2i(gw, gh);
	glEnd();

	//Draw a single line
	// it will stretch 1/2 the width and the full vertical
	//We will use a geometry shader to draw the same line, but with the x and y values swapped!
	// i.e. we will get a cross on the screen
	glUseProgram(p);
	glBegin(GL_LINES_ADJACENCY_EXT);
		glVertex2i(0, 0);
		glVertex2i(gw*2/3, gh/8);
		glVertex2i(gw/3, gh*7/8);
		glVertex2i(gw, gh);
	glEnd();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	static int animating = 0, wireframe = 0, shader=1;

	if (key == 27) 
		exit(0);

	if (key == 's') {
		shader = !shader;

		if (shader)
			glUseProgram(p);
		else
			glUseProgram(0);

		glutPostRedisplay();
	}

	if (key == ' ') {
		animating = !animating; /* Toggle */
		if (animating) {
			glutIdleFunc(idle);
		} else {
			glutIdleFunc(NULL);
		}    
	}

	if (key == '=') {
		num *= 2;
		glutPostRedisplay();
	}

	if (key == '-') {
		num /= 2;
		if (num < 4) num = 4;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Geoemtry Shader - pass through try press s");

	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glClearColor(0.0,0.0,0.0,1.0);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_1"))
		printf("Ready for OpenGL 2.1\n");
	else {
		printf("OpenGL 2.1 not supported\n");
		exit(1);
	}
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");
	else {
		printf("Not totally ready :( \n");
		exit(1);
	}

	p = setShaders("gs.vert", "gs.frag", "gs.geom");

	glProgramParameteriEXT(p,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES_ADJACENCY_EXT);
	glProgramParameteriEXT(p,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);

	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
	glProgramParameteriEXT(p,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

	glLinkProgram(p);
	hNum = glGetUniformLocation(p,"Num");

	glUseProgram(p);

	glutMainLoop();

	return 0;
}

