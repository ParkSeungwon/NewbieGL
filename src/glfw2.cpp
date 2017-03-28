//#include <glew.h>
#include"matrix.h"
#include<cstring>
#include<stdlib.h>		  // srand, rand
#include<thread>         // std::this_thread::sleep_for
#include<chrono>         // std::chrono::seconds
#include<iostream>
#include<cmath>
#include"glutil.h"
using namespace std;

const int wt = 640;
const int ht = 480;
extern Matrix<float> KeyBindMatrix;

void draw(valarray<Matrix<float>>& circle) {
	circle = KeyBindMatrix * circle;
	glBegin(GL_TRIANGLE_FAN);
	for(auto& a : circle) glVertex2fv(a.data());
	glEnd();
}

int main(void)
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glortho(2);

	auto bcircle = polygon(100);//generate circle points
	auto ycircle = polygon(100, 0.95);//100 edge polygon
	auto leye = polygon(100, 0.2);// which to human eyes looks like a circle
	auto reye = polygon(100, 0.2);
	auto mouth = polygon(100, 0.7);

	Matrix<float> m{4,4};
	m = m.gltranslate(-0.3,0.5,0) * m.glscale(0.7,1.1,1);
	leye = m * leye;//move eye to position, extend vertically
	m = m.gltranslate(0.3,0.5,0) * m.glscale(0.7,1.1,1);
	reye = m * reye;
	ycircle = m.gltranslate(0, 0, 0.1) * ycircle;
	leye = m.gltranslate(0, 0, 1) * leye;
	reye = m.gltranslate(0, 0, -1) * reye;
	mouth = m.gltranslate(0, 0, 0.2) * mouth;
	glClearColor(1,1,1,1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO: draw here
		glColor4f(0,0,0,0.5);
		draw(bcircle);
		glColor3f(1,1,0);
		draw(ycircle);
		glColor3f(0,0,0);
		draw(leye);
		draw(reye);
		
		glLineWidth(30);
		glBegin(GL_LINE_STRIP);
		for(auto it = begin(mouth) + 50; it != end(mouth); it++) {
			*it = KeyBindMatrix * *it;
			glVertex2fv(it->data());
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();//glfwWaitEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	glfwTerminate();
	return 0;
}
