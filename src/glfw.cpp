//#include <glew.h>
#include"matrix.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include <cmath>
#include"glutil.h"
using namespace std;

const int wt = 1024;
const int ht = 1024;

int main(void)
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Hello World", NULL, NULL);
	if(!glinit(window)) return -1;;

	const float theta = 2.0 * M_PI / 20;
	Matrix<float> m{4,4};
	m.glrotateZ(theta);
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	glOrtho(256,0,0,256,1,-1);
	unsigned char z = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);
		for(int s : {0, width/2}) for(int e : {0, width/2}) {
			glViewport(s, e, width/2, width/2);

			//TODO: draw here
			glBegin(GL_QUADS);
			for(int x=0; x<256; x++) for(int y=0; y<256; y++) {
				glcolor(x, y, z, 255);
				glVertex3i(x, y, 0);
				glVertex3i(x+1, y, 0);
				glVertex3i(x+1, y+1, 0);
				glVertex3i(x, y+1, 0);
			}
			glEnd();

			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			z++;
		}
		glfwPollEvents();//glfwWaitEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}


