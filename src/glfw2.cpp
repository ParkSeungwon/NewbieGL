//#include <glew.h>
#include"matrix.h"
#include<GLFW/glfw3.h>
#include<cstring>
#include<stdlib.h>		  // srand, rand
#include<thread>         // std::this_thread::sleep_for
#include<chrono>         // std::chrono::seconds
#include<iostream>
#include<cmath>
#include"glutil.h"
#include"circle.h"
using namespace std;

const int wt = 640;
const int ht = 480;
extern Matrix<float> translate;

void draw(Circle& circle) {
	glBegin(GL_TRIANGLE_FAN);
	for(auto& a : circle) {
		a = translate * a;
		glVertex2fv(a.data());
	}
	glEnd();
}

int main(void)
{
	translate.gltranslate(0,0,0);
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;
	
	const float theta = 2.0 * M_PI / 20;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glortho(2);
	glColor3f(1,0,0);

	float center[3] = {0,0,0};
	Circle bcircle{center, 1};
	Circle ycircle{center, 0.95};
	Circle leye{center, 0.2};
	Circle reye{center, 0.2};
	Circle mouth{center, 0.7};

	Matrix<float> m{4,4};
	m = m.gltranslate(-0.3,0.5,0) * m.glscale(0.7,1.1,1);
	for(auto& a : leye) a = m * a;
	m = m.gltranslate(0.3,0.5,0) * m.glscale(0.7,1.1,1);
	for(auto& a : reye) a = m * a;


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//TODO: draw here
		glColor3f(0,0,0);
		draw(bcircle);
		glColor3f(1,1,0);
		draw(ycircle);
		glColor3f(0,0,0);
		draw(leye);
		draw(reye);
		
		glLineWidth(30);
		glBegin(GL_LINE_STRIP);
		for(auto it = mouth.begin() + 50; it != mouth.end(); it++) {
			*it = translate * *it;
			glVertex2fv(it->data());
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();//glfwWaitEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();
	return 0;
}
