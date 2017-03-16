//#include <glew.h>
#include"matrix.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include<cmath>
#include"glutil.h"
#include"circle.h"
using namespace std;

const int wt = 640;
const int ht = 480;

extern Matrix<float> translate;
int main(void)
{
	translate.gltranslate(0,0,0);
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;
	
	const float theta = 2.0 * M_PI / 20;
	Matrix<float> m{4,4};
	m.glrotateZ(theta);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glortho(2);
	glColor3f(1,0,0);

	Matrix<float> center{0,0,0};
	Circle bcircle{center.data(), 1};
	Circle ycircle{center.data(), 0.95};
	float leye[3], reye[3];
	memcpy(leye, center.data(), sizeof(float) * 3);
	memcpy(reye, center.data(), sizeof(float) * 3);
	leye[0] -= 0.3; reye[0] += 0.3;
	leye[1] += 0.5; reye[1] += 0.5;
	Circle leyec{leye, 0.2};
	Circle reyec{reye, 0.2};
	Circle mouth{center.data(), 0.7};
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//TODO: draw here
		glBegin(GL_TRIANGLE_FAN);
		glVertex3fv(bcircle.ctr());
		glColor3f(0,0,0);
		for(auto& a : bcircle) {
			a = translate * a;
			glVertex3fv(a.data());
		}
		glVertex3fv(bcircle.begin()->data());
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1,1,0);
		for(auto& a : ycircle) {
			a = translate * a;
			glVertex3fv(a.data());
		}
		glVertex3fv(ycircle.begin()->data());
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0,0,0);
		for(auto& a : leyec) {
			a = translate * a;
			glVertex3fv(a.data());
		}
		glEnd();
		
		glBegin(GL_TRIANGLE_FAN);
		for(auto& a : reyec) {
			a = translate * a;
			glVertex3fv(a.data());
		}
		glEnd();
		
		glLineWidth(20);
		glBegin(GL_LINE_STRIP);
		auto it = mouth.begin();
		it += 50;
		for(int i=50; i<99; i++) {
			*it = translate * *it;
			glVertex3fv(it++->data());
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();//glfwWaitEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	return 0;
}


