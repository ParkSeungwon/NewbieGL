//#include <glew.h>
#include"matrix.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include "math.h"
#include"vector.h"
#include"glutil.h"
using namespace std;

const int wt = 640;
const int ht = 480;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

int main(void)
{
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(wt, ht, "Hello World", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}
	
	const float theta = 2.0 * M_PI / 20;
	Matrix<float> m{4,4};
	m.glrotateZ(theta);
	
	// callbacks here
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glortho(2);
	glColor3f(1,0,0);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		Vec4D<float> pt {1.0f, 0.0f, 0.0f, 0.0f};

		//TODO: draw here
		glBegin(GL_TRIANGLE_FAN);
		for(int i=0; i<20; i++) {
			pt = m * pt;
			glColor3fv(pt.v);
			glVertex3fv(pt.v);
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();//glfwWaitEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	return 0;
}


