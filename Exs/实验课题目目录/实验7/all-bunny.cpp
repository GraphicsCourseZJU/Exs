// glutEx1.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glut.h"


float fTranslate;
float fRotate = 156.5f;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = true;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

GLint dl = 0;

void Draw_Leg();

int  drawMode = 0;

extern void drawNaive();
extern void drawVA();
extern GLint Gen3DObjectList();

void drawDL() {
	glCallList(dl);
}


void drawBunny()
{
	glRotatef(90, 1, 0, 0);
	glScalef(3, 3, 3);
	if (drawMode == 0)
		drawNaive();
	else if (drawMode == 1)
		drawVA();
	else
		drawDL();
}

void Draw_Desk();

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	glPushMatrix();
	glTranslatef(-1, -1, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -1, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 1, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 1, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 0, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 5.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 0, 5.5);
	drawBunny();
	glPopMatrix();

//###################################

	glPushMatrix();
	glTranslatef(-1, -1, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -1, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 1, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 1, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 0, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 7.5);
	drawBunny();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 0, 7.5);
	drawBunny();
	glPopMatrix();

	Draw_Desk();
}

void Draw_Desk()
{
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
	    //glFrustum(-3, 3, -3, 3, 3,100);
	} else {
	    glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float center[] = {0, -0.8, -6};
float eye[] = {0, 1.2, 2};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}
	case '0': {drawMode++; drawMode %= 3; break;}

	case 'a': {
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'd': {
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'w': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': {
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': {
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': {
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	}

	updateView(wHeight, wWidth);
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	char mode[64];
	if (drawMode == 0)
		strcpy_s(mode, "naive");
	else if (drawMode == 1)
		strcpy_s(mode, "vertex array");
	else
		strcpy_s(mode, "display list");

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(buffer,"FPS:%4.2f %s",
			frame*1000.0/(time-timebase), mode);
		timebase = time;		
		frame = 0;
	}

	glutSetWindowTitle(buffer);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate    += 0.5f;

	getFPS();
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	dl = Gen3DObjectList();

	glutMainLoop();
	return 0;
}


