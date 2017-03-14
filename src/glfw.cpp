#include <GLFW/glfw3.h>
#include <glut.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include "math.h"

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,0,0);

	glBegin(GL_TRIANGLES);
	glVertex3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(0.5,0,0);
	glEnd();

	glutSwapBuffers();
}
const int width_window = 640;
const int height_window = 480;

int main(int c, char** v)
{
	glutInit(&c, v);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width_window, height_window);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL");

	glClearColor(102.0/255,0.8,0,0);
	glOrtho(-2, 2, -2, 2, -2, 2);
	glutDisplayFunc(MyDisplay);

//	Menu m3{menu, "fjdk", "fjfj"};
//	Menu m2{menu2, "jfjl", "fjdk", m3, "fjdk"};
//	Menu m{menu, "Korean no now?", "exit", m2, "submenu", "Quit"};
	glutMainLoop();
}



