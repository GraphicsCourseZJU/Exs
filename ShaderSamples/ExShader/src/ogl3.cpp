/*

Simple Demo for GLSL

www.lighthouse3d.com

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/GL/glew.h"
#include "../include/GL/glut.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "../include/shader.h"
#include "../include/stanford_bunny.h"

GLuint spotLightPosLoc, spotLightTarLoc, spotLightAngleLoc;
GLuint lightPosLoc, viewPosLoc, lightColorLoc;
GLint projectionLoc, viewLoc, modelLoc;
GLint lprojectionLoc, lviewLoc, lmodelLoc;
GLuint objectColorLoc, ambientStrenthLoc, specularStrengthLoc, shinessLoc;
GLuint VAO[2], VBO[2], EBO[2];
GLuint p, pl;

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::mat4 projection;
glm::vec3 lightColor(1.0, 1.0, 1.0);
glm::vec3 lightPos(0, 0, 3);
glm::vec3 viewPos(0, 3, 3);

glm::vec3 spotLightPos(0, 3, 3);
glm::vec3 spotLightTar(0, 0, 0);
float spotLightAngle = 30.f;

float rotAngle = 0.f;

int bunnyVerNum;

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) h = 1;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set projection Matrix
	projection = glm::perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
	glUseProgram(p);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(pl);
	glUniformMatrix4fv(lprojectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

static void idle(void)
{
	glutPostRedisplay();
}


void drawLight()
{
	glBindVertexArray(VAO[0]);
	glm::mat4 model;
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(lmodelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void drawBunny()
{
	glBindVertexArray(VAO[1]);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
	model = glm::rotate(model, rotAngle/180.f*3.1415926f, glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform3f(objectColorLoc, 1, 0.6, 0.12);
	glUniform1f(ambientStrenthLoc, 0.3);
	glUniform1f(specularStrengthLoc, 1.3);
	glUniform1i(shinessLoc, 64);

	glDrawArrays(GL_TRIANGLES, 0, bunnyVerNum);
	glBindVertexArray(0);
}

void drawTable(void) {
	glBindVertexArray(VAO[0]);
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
		model = glm::scale(model, glm::vec3(1, 0.2, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(objectColorLoc, 0.27, .5, 0.705);
		glUniform1f(ambientStrenthLoc, 0.3);
		glUniform1f(specularStrengthLoc, 1);
		glUniform1i(shinessLoc, 32);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-0.25, 0.0, -0.25));
		model = glm::scale(model, glm::vec3(0.2, 1, 0.2));
		GLint modelLoc = glGetUniformLocation(p, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(objectColorLoc, 0.27, .5, 0.705);
		glUniform1f(ambientStrenthLoc, 0.1);
		glUniform1f(specularStrengthLoc, 1);
		glUniform1i(shinessLoc, 32);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-0.25, 0.0, 0.25));
		model = glm::scale(model, glm::vec3(0.2, 1, 0.2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(objectColorLoc, 0.27, .5, 0.705);
		glUniform1f(ambientStrenthLoc, 0.1);
		glUniform1f(specularStrengthLoc, 1);
		glUniform1i(shinessLoc, 32);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.25, 0.0, 0.25));
		model = glm::scale(model, glm::vec3(0.2, 1, 0.2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		glUniform3f(objectColorLoc, 0.27, .5, 0.705);
		glUniform1f(ambientStrenthLoc, 0.1);
		glUniform1f(specularStrengthLoc, 1);
		glUniform1i(shinessLoc, 32);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.25, 0.0, -0.25));
		model = glm::scale(model, glm::vec3(0.2, 1, 0.2));
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(objectColorLoc, 0.27, .5, 0.705);
		glUniform1f(ambientStrenthLoc, 0.1);
		glUniform1f(specularStrengthLoc, 1);
		glUniform1i(shinessLoc, 32);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set View Matrix
	glm::mat4 view;
	view = glm::lookAt(viewPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// Draw Objects
	glUseProgram(p);
	
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	glUniform3fv(spotLightPosLoc, 1, glm::value_ptr(spotLightPos));
	glUniform3fv(spotLightTarLoc, 1, glm::value_ptr(spotLightTar));
	glUniform1f(spotLightAngleLoc, spotLightAngle);
	
	drawTable();
	drawBunny();
	
	glUseProgram(pl);
	
	glUniformMatrix4fv(lviewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	drawLight();
	
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == '=')
	{
		rotAngle += 3;
	}
	else if (key == '-')
	{
		rotAngle -= 3;
	}
	else if (key == 'w')
	{
		lightPos.z -= 0.1;
	}
	else if (key == 's')
	{
		lightPos.z += 0.1;
	}
	else if (key == 'a')
	{
		lightPos.x -= 0.1;
	}
	else if (key == 'd')
	{
		lightPos.x += 0.1;
	}
	else if (key == 'q')
	{
		lightPos.y += 0.1;
	}
	else if (key == 'z')
	{
		lightPos.y -= 0.1;
	}
	else if (key == 'i')
	{
		spotLightTar.z -= 0.1;
	}
	else if (key == 'k')
	{
		spotLightTar.z += 0.1;
	}
	else if (key == 'j')
	{
		spotLightTar.x -= 0.1;
	}
	else if (key == 'l')
	{
		spotLightTar.x += 0.1;
	}
	else if (key == 'u')
	{
		spotLightTar.y += 0.1;
	}
	else if (key == 'm')
	{
		spotLightTar.y -= 0.1;
	}
	else if (key == '.')
	{
		spotLightAngle += .3;
	}
	else if (key == ',')
	{
		if (spotLightAngle > 0)
		spotLightAngle -= .3;
	}
	else if (key == 'r')
	{
		if (lightColor.x < 1)
			lightColor.x += 0.05;
	}
	else if (key == 'f')
	{
		if (lightColor.x > 0)
			lightColor.x -= 0.05;
	}
	else if (key == 't')
	{
		if (lightColor.y < 1)
			lightColor.y += 0.05;
	}
	else if (key == 'g')
	{
		if (lightColor.y > 0)
			lightColor.y -= 0.05;
	}
	else if (key == 'y')
	{
		if (lightColor.z < 1)
			lightColor.z += 0.05;
	}
	else if (key == 'h')
	{
		if (lightColor.z > 0)
			lightColor.z -= 0.05;
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
	glClearColor(0,0,0,1.0);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	// Create VAO, VBO, EBO
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	//glGenBuffers(1, &EBO);

	// Cube VAO
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Note that this is allowed, the call to glVertexAttribPointer registered VBO 
	// as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), 
	// remember: do NOT unbind the EBO, keep it bound to this VAO
	glBindVertexArray(0); 

	// Bunny VAO
	float* data; int dataSize;
	GetBunny(&data, dataSize, bunnyVerNum);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Note that this is allowed, the call to glVertexAttribPointer registered VBO 
	// as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), 
	// remember: do NOT unbind the EBO, keep it bound to this VAO
	glBindVertexArray(0);
	

	// Set Shader
	p = setShaders("../shader/custom.vert", "../shader/custom.frag");
	pl = setShaders("../shader/light.vert", "../shader/light.frag");
	
	glUseProgram(p);
	// Set lightl
	lightColorLoc = glGetUniformLocation(p, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	lightPosLoc = glGetUniformLocation(p, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	spotLightPosLoc = glGetUniformLocation(p, "spotLightPos");
	glUniform3fv(spotLightPosLoc, 1, glm::value_ptr(spotLightPos));
	spotLightTarLoc = glGetUniformLocation(p, "spotLightTar");
	glUniform3fv(spotLightTarLoc, 1, glm::value_ptr(spotLightTar));
	spotLightAngleLoc = glGetUniformLocation(p, "spotLightAngle");
	glUniform1f(spotLightAngleLoc, spotLightAngle);

	// Get Loc
	projectionLoc = glGetUniformLocation(p, "projection");
	viewLoc = glGetUniformLocation(p, "view");
	modelLoc = glGetUniformLocation(p, "model");

	viewPosLoc = glGetUniformLocation(p, "viewPos");

	objectColorLoc = glGetUniformLocation(p, "objectColor");
	ambientStrenthLoc = glGetUniformLocation(p, "ambientStrength");
	specularStrengthLoc = glGetUniformLocation(p, "specularStrength");
	shinessLoc = glGetUniformLocation(p, "shiness");

	glUseProgram(pl);
	lprojectionLoc = glGetUniformLocation(pl, "projection");
	lviewLoc = glGetUniformLocation(pl, "view");
	lmodelLoc = glGetUniformLocation(pl, "model");

	glutMainLoop();

	return 0;
}

