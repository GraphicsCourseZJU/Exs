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

GLuint p;
GLint lightPosition, ambient;

static float myLightColor[3] = { 0.95, 0.95, 0.95 };  /* White */
static float myLightAngle = -0.4;   /* Angle light rotates around scene. */
static float myTime = 0.0;  /* Timing of bulge. */

int mdl = 0;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);


}

static void idle(void)
{
	static float lightVelocity = 0.008;
	static float timeFlow = 0.01;
	static const double myPi = 3.14159265358979323846;

	/* Repeat rotating light around front 180 degrees. */
	if (myLightAngle > myPi/2) {
		myLightAngle = myPi/2;
		lightVelocity = -lightVelocity;
	} else if (myLightAngle < -myPi/2) {
		myLightAngle = -myPi/2;
		lightVelocity = -lightVelocity;
	}
	myLightAngle += lightVelocity;  /* Add a small angle (in radians). */

	/* Repeatedly advance and rewind time. */
	if (myTime > 10) {
		myTime = 10;
		timeFlow = -timeFlow;
	} else if (myTime < 0) {
		myTime = 0;
		timeFlow = -timeFlow;
	}
	myTime += timeFlow;  /* Add time delta. */

	glutPostRedisplay();
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(-3.0,-4.0,9.0, 
		0.0,0.0,-1.0,
		0.0f,1.0f,0.0f);

	glRotatef(myTime*5, 1, 1, 1);


	if (mdl == 0)
		glutSolidTeapot(3);

	if (mdl == 1) {
	glPushMatrix();
	glScalef(0.8f, 0.8f, 0.8f);
	
	glPushMatrix();
	glTranslatef(-2.1, 0, 0);
	glutSolidSphere(1.0, 40, 40);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.2, 1.0, 0.2);
	glutSolidTorus(0.15, 1.7, 40, 40);
	glPopMatrix();
	glPopMatrix();
	}

	if (mdl == 2) {
		glutSolidCone(3.0, 3.0, 40, 40);
	}

	if (mdl == 3) {
		glutSolidCube(4.0);
	}

	if (mdl == 4) {
	glPushMatrix();
	glScalef(2.8f, 2.8f, 2.8f);
		glutSolidOctahedron();
	glPopMatrix();
	}

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

	if (key == 'm') {
		mdl = (mdl+1)%5;
		glutPostRedisplay();
	}

	if (key == 'w') {
		wireframe = !wireframe;
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glutPostRedisplay();
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("GLSL-twisting try press s/w/space");

	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0,1.0,1.0,1.0);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	p = setShaders("ivory.vt", "ivory.fg");
	//p = setShaders("gooch.vt", "gooch.fg");

	lightPosition = glGetUniformLocation(p,"lightPosition");
	ambient = glGetUniformLocation(p,"ambient");

	glUseProgram(p);

	const float myEyePosition[4] = { 0, 0, 13, 1 };
	const float myLightPosition[4] = { 5*sin(myLightAngle), 
		1.5,
		5*cos(myLightAngle), 1 };

	glUniform3fv(lightPosition, 1, myLightPosition);
	glUniform3f(ambient, 0.6, 0.6, 0.0);


	glutMainLoop();

	return 0;
}

