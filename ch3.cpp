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
#define WINDOW_TITLE_PREFIX "Ch 3"

typedef struct {
	float XYZW[4];
	float RGBA[4];
} Vertex;

// Globals for window vars (will be fields in a forthcoming class)
int
curWidth = 600,
curHeight = 600,
windowHandle = 0;

// For performance testing
unsigned framecount = 0;

GLuint
	vertexShaderId,
	fragmentShaderId,
	programId,
	vaoId,
	bufferId,
	indexBufferId;

const GLchar* vertexShader = {
	"#version 400\n"\

	"layout(location=0) in vec4 in_Position;\n"\
	"layout(location=1) in vec4 in_Color;\n"\
	"out vec4 ex_Color;\n"\

	"void main(void) {\n"\
	"	gl_Position = in_Position;\n"\
	"	ex_Color = in_Color;\n"\
	"}"
};

const GLchar* fragmentShader = {
	"#version 400\n"\

	"in vec4 ex_Color;\n"\
	"out vec4 out_Color;\n"\

	"void main(void) {\n"\
	"	out_Color = ex_Color;\n"\
	"}"
};


// Prototypes (Ch1)
void init(int, char* []);
void initWindow(int, char* []);
void resize(int, int);
void render(void);
void timer(int);
void idle(void);

// Ch2
void cleanup(void);
void createVBO(void);
void destroyVBO(void);
void createShaders(void);
void destroyShaders(void);


int main(int argc, char* argv[]) {
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}


void init(int argc, char* argv[]) {
	GLenum glewInitResult;

	initWindow(argc, argv);

	// Initialize API for OpenGL 4.0 functions
	glewExperimental = GL_TRUE;
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
		"INFO: OpenGL Version: %s\nRenderer Version: %s\n",
		glGetString(GL_VERSION),
		glGetString(GL_RENDERER)
	);

	createShaders();
	createVBO();
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
	glutCloseFunc(cleanup);
}


void resize(int x, int y) {
	curWidth = x;
	curHeight = y;
	glViewport(0, 0, curWidth, curHeight);
}


void render(void) {
	++framecount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Tells GPU to draw indices in the active GL_ARRAY_BUFFER that are specified by
	// the active GL_ELEMENT_ARRAY_BUFFER.
	// Mode of drawing, number of vertexes to draw, what datatype GL_ELEMENT_ARRAY_BUFFER holds, starting index to draw 
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, (GLvoid*)0);

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


void cleanup(void) {
	destroyShaders();
	destroyVBO();
}

/*
	Create vector buffer objects.
	Essentially, tell GPU where significant points are, and what to color those points
*/
void createVBO(void) {
	Vertex vertices[] = {
		// Middle
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		
		// Top
		{ { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		
		// Bottom
		{ { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		
		// Left
		{ { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		
		// Right
		{ { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
	};

	GLubyte indices[] = {
		// Top
		0, 1, 3,
		0, 3, 2,
		3, 1, 4,
		3, 4, 2,

		// Bottom
		0, 5, 7,
		0, 7, 6,
		7, 5, 8,
		7, 8, 6,

		// Left
		0, 9, 11,
		0, 11, 10,
		11, 9, 12,
		11, 12, 10,

		// Right
		0, 13, 15,
		0, 15, 14,
		15, 13, 16,
		15, 16, 14
	};

	// Called to "unset" any erstwhile errors; disregard the ret val
	GLenum errCheckVal = glGetError();

	const size_t buffSize = sizeof(vertices);
	const size_t vertSize = sizeof(vertices[0]);
	const size_t rgbOffset = sizeof(vertices[0].XYZW);

	// Creates a VertexArray obj in the GPU's memory
	// Just contains metadata about v-array, not actual data
	glGenBuffers(1, &bufferId);
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	// Copies actual data of vertices into buffer that was just bound
	// Tells it where it is, how large it is, and how it will be used
	// Static_draw == we aren't going to ever change it
	glBufferData(GL_ARRAY_BUFFER, buffSize, vertices, GL_STATIC_DRAW);

	// Provide necessary metadata so GPU knows how to use the raw binary it just copied
	// The first number is its index that is used in the shader functions/to reference it here/everywhere
	// Second is dimensionality of each vector
	// Second to last is stride of each datum (how far to move in memory between reads)
	// Last is starting point in memory
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertSize, (GLvoid*)rgbOffset);

	// Uses the vertex data "for drawing purposes" (I think it just prepares the GPU to draw it, but doesn't actually yet)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Makes this the active GL_ELEMENT_ARRAY_BUFFER
	// Essentially says draw the triangles specified by the values in this array
	// The contents of the array are pointers to the coordinates in the vertex buffer
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	errCheckVal = glGetError();
	if (errCheckVal != GL_NO_ERROR) {
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s\n",
			gluErrorString(errCheckVal)
		);

		exit(EXIT_FAILURE);
	}
}

void destroyVBO(void) {
	GLenum errCheckVal = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &bufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &indexBufferId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);

	errCheckVal = glGetError();
	if (errCheckVal != GL_NO_ERROR) {
		fprintf(
			stderr,
			"ERROR: Could not destroy the VBO: %s\n",
			gluErrorString(errCheckVal)
		);

		exit(EXIT_FAILURE);
	}
}

/*
	Shaders are like little C programs that you can input as strings
	They use a language called GLSL to do this, and can reference objects in
	the GPU via their index
*/
void createShaders(void) {
	GLenum errCheckVal = glGetError();

	// Compile the vert shader
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);			// Create a new shader object
	glShaderSource(vertexShaderId, 1, &vertexShader, NULL);		// Tell OpenGL where to find the string to be compiled / where to put the pointer (arg 1)
	glCompileShader(vertexShaderId);							// Then compile that string

	// Compile the frag shader
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	// Now combine the two into a single program object
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Makes this the program used until this function is called again
	glUseProgram(programId);

	errCheckVal = glGetError();
	if (errCheckVal != GL_NO_ERROR) {
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(errCheckVal)
		);

		exit(EXIT_FAILURE);
	}
}

void destroyShaders(void) {
	GLenum errCheckVal = glGetError();

	glUseProgram(0);
	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteProgram(programId);

	errCheckVal = glGetError();
	if (errCheckVal != GL_NO_ERROR) {
		fprintf(
			stderr,
			"ERROR: Could not destroy the shaders: %s \n",
			gluErrorString(errCheckVal)
		);

		exit(EXIT_FAILURE);
	}
}