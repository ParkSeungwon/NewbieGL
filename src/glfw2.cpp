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
	valarray<Matrix<float>> black{Matrix<float>{0,0,0}, 100};
	valarray<Matrix<float>> yellow{Matrix<float>{1,1,0}, 100};
	Matrix<float> m{4,4};
	m = m.gltranslate(-0.3,0.5,0) * m.glscale(0.7,1.1,1);
	leye = m * leye;//move eye to position, extend vertically
	m = m.gltranslate(0.3,0.5,0) * m.glscale(0.7,1.1,1);
	reye = m * reye;
	ycircle = m.gltranslate(0, 0, 0.05) * ycircle;
	leye = m.gltranslate(0, 0, 0.1) * leye;
	reye = m.gltranslate(0, 0, 0.1) * reye;
	mouth = m.gltranslate(0, 0, 0.1) * mouth;
	glClearColor(1,1,1,1);
	valarray<Matrix<float>>* p[5];
	p[0] = &bcircle;
	p[1] = &ycircle;
	p[2] = &leye;
	p[3] = &reye;
	p[4] = &mouth;
	unsigned vbo[7];
	for(int i=0; i<5; i++) vbo[i] = gl_transfer_data(*p[i]);
	vbo[5] = gl_transfer_data(black);
	vbo[6] = gl_transfer_data(yellow);

	glLineWidth(20);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO: draw here
		for(int i=0; i<5; i++) *p[i] = KeyBindMatrix * *p[i];
		for(int i=0; i<5; i++) gl_transfer_data(*p[i], vbo[i]);
		for(int i=0; i<5; i++) {
			if(i == 1) glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
			else glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, nullptr);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, nullptr);//3 float is 1 vertex stride 0, 

			if(i == 4) glDrawArrays(GL_LINE_STRIP, 50, 50);
			else glDrawArrays(GL_TRIANGLE_FAN, 0, 100);//mode, first, count
			
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();//glfwWaitEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	glfwTerminate();
	return 0;
}
