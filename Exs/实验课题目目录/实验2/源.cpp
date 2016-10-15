// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <glut.h>

float fTranslate;
float fRotate;
float fScale     = 1.0f;								// set inital scale value to 1.0f

void Draw_Triangle()									// This function draws a triangle with RGB colors
{
	glBegin(GL_TRIANGLES);
		 glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		 glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		 glColor3f(0.0f, 0.0f,1.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);			
	glEnd();
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{
// If want display in wireframe mode
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glPushMatrix();
		glTranslatef(-2.0f, 0.0f,-6.0f);				// Place the triangle Left
		glTranslatef(0.0f, fTranslate, 0.0f);			// Translate in Y direction
		Draw_Triangle();								// Draw triangle					
	glPopMatrix();

    glPushMatrix();
		glTranslatef(0.0f, 0.0f,-6.0f);					// Place the triangle at Center
		glRotatef(fRotate, 0, 1.0f, 0);					// Rotate around Y axis
		Draw_Triangle();								// Draw triangle
	glPopMatrix();


	glPushMatrix();
		glTranslatef(2.0f,0.0f,-6.0f);			// Place the triangle Right
		glScalef(fScale, fScale, fScale);		// Scale with the same value in x,y,z direction
		Draw_Triangle();						// Draw triangle						
	glPopMatrix();


	fTranslate += 0.005f;
	fRotate    += 0.5f;
	fScale     -= 0.005f;

	if(fTranslate > 0.5f) fTranslate = 0.0f;
	if(fScale < 0.5f)     fScale     = 1.0f;
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);               ///////////////////////////////////////
	int windowHandle
		= glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);					///////////////////////////////////////
	glutIdleFunc(idle);							///////////////////////////////////////

	glutMainLoop();

	return 0;
}


