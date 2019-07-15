/*
	Boilerplate for all OpenGL projects using OpenGLBook.com
	(Could probably put this in a class for future use)
*/

// STOP TELLING ME SPRINTF IS UNSAFE, DAMMIT
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Title of project goes here
#define WINDOW_TITLE_PREFIX "Ch 1"

// Globals for window vars (will be fields in a forthcoming class)
int
curWidth = 800,
curHeight = 600,
windowHandle = 0;

// For performance testing
unsigned framecount = 0;


// Prototypes
void init(int, char* []);
void initWindow(int, char* []);
void resize(int, int);
void render(void);
void timer(int);
void idle(void);


int main(int argc, char* argv[]) {
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}


void init(int argc, char* argv[]) {
	GLenum glewInitResult;

	initWindow(argc, argv);

	// Initialize API for OpenGL 4.0 functions
	glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(glewInitResult)
		);
		exit(EXIT_FAILURE);
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


void initWindow(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);

	glutInitWindowSize(curWidth, curHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	windowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (windowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window\n"
		);
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(resize);
	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutTimerFunc(0, timer, 0);
}


void resize(int x, int y) {
	curWidth = x;
	curHeight = y;
	glViewport(0, 0, curWidth, curHeight);
}


void render(void) {
	++framecount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
	glutPostRedisplay();
}


void timer(int val) {
	if (val) {
		char* tmpStr = (char*)malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			tmpStr,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			framecount * 4,
			curWidth,
			curHeight
		);

		glutSetWindowTitle(tmpStr);
		free(tmpStr);
	}

	framecount = 0;
	glutTimerFunc(250, timer, 1);
}


void idle(void) { glutPostRedisplay(); }