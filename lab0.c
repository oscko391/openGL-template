// Ingemar's psychedelic teapot 2
// Rewritten for modern OpenGL
// Teapot model generated with the OpenGL Utilities for Geometry Generation
// (also written by Ingemar) based on the original teapot data.

#ifdef __APPLE__
#include <OpenGL/gl3.h>
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#ifndef NULL
#define NULL 0L
#endif

mat4 projectionMatrix, viewMatrix;

mat4 objectExampleMatrix = { { 1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0 } };

Model *theModel;
// Reference to shader program
GLuint program;
GLuint texture;

void init(void)
{
	dumpInfo();
	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	glUseProgram(program);

	projectionMatrix = perspective(90, 1.0, 0.1, 1000);
	viewMatrix = lookAt(0, 0, 1.5, 0, 0, 0, 0, 1, 0);
	// Upload geometry to the GPU:
	theModel = LoadModelPlus("objects/stanford-bunny.obj");
	// End of upload of geometry
	
	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga", &texture);
	printError("load textures");
}

GLfloat a, b = 0.0;

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//activate the program, and set its variables
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	mat4 m = Mult(viewMatrix, objectExampleMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);

	//draw the model
	DrawModel(theModel, program, "in_Position", "in_Normal", NULL);

	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow("lab0");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display);
	glutRepeatingTimer(20);
	init();
	glutMainLoop();
	exit(0);
}