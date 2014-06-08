#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <string>

#include <boost/thread.hpp>
using namespace boost;

static GLFWwindow* aWindow;
static int aWidth = 640;
static int aHeight = 480;
static std::string aTitle = "Cube";

static const GLdouble aCubeVertex[][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 },
};

static const int aCubeFace[][4] = {
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 },
};

static const GLdouble aCubeNormal[][3] = {
	{ 0.0, 0.0,-1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{-1.0, 0.0, 0.0 },
	{ 0.0,-1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
};

static const GLfloat aCubeMaterial[] = { 0.8, 0.2, 0.2, 1.0 };
static const GLfloat aLightColor[] = { 0.2, 0.2, 0.8, 1.0 };
static const GLfloat aLight0pos[] = { 0.0, 3.0, 5.0, 1.0 };
static const GLfloat aLight1pos[] = { 5.0, 3.0, 0.0, 1.0 };

static GLfloat aLookat[] = { 3.0, 4.0, 5.0 };

static void drawCube();

void thread_test() {
	while ( true ) {
		std::cout << "thread_test" << std::endl;
		aLookat[0] -= 0.1;
		this_thread::sleep(posix_time::milliseconds(1000));
	}  
}

int main() {
	if ( ! glfwInit() ) {
		std::cerr << "glfwInit failed." << std::endl;
		exit(EXIT_FAILURE);
	}
	aWindow = glfwCreateWindow(aWidth, aHeight, aTitle.c_str(), nullptr, nullptr);
	if ( ! aWindow ) {
		std::cerr << "glfwCreateWindow failed." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(aWindow);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glCullFace(GL_FRONT);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, aLightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, aLightColor);
	glClearColor(1, 1, 1, 1);

	thread aThread(thread_test);

	while ( ! glfwWindowShouldClose(aWindow) ) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		int width, height;
		glfwGetFramebufferSize(aWindow, &width, &height);
		glViewport(0, 0, width, height);
		gluPerspective(30.0, (double)width / (double)height, 1.0, 100.0);
		glTranslated(0.0, 0.0, -2.0);
		gluLookAt(aLookat[0], aLookat[1], aLookat[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, aLight0pos);
		glLightfv(GL_LIGHT1, GL_POSITION, aLight0pos);

		drawCube();

		glfwSwapBuffers(aWindow);
		glfwPollEvents();
	}

	aThread.interrupt();
	aThread.join();

	glfwTerminate();
	return EXIT_SUCCESS;
}

static void drawCube() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, aCubeMaterial);
	glBegin(GL_QUADS);

	for ( size_t i = 0; i < 6; ++ i ) {
		glNormal3dv(aCubeNormal[i]);
		for ( size_t j = 0; j < 4; ++ j ) {
			glVertex3dv(aCubeVertex[aCubeFace[i][j]]);
		}
	}

	glEnd();
}



