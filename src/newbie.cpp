#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"globj.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main(int ac, char** av)
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	GLObject obj3d;
	obj3d.vertexes({ {1,0,0},{0,1,0},{0,0,0}, {0,0,1} });
	obj3d.colors({ {1,0,0}, {0,1,0}, {0,0,1}, {1,1,0} });
	obj3d.indices({ 0, 1, 2, 3,1,2, 0,2,3, 0,1,3});

	GLObjs stage;
	stage += obj3d;
	stage.transfer_all();
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		stage.matrix(KeyBindMatrix * stage[0]);
		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(50ms);
	}
	glfwTerminate();
}

