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

	auto vv = polygon(3, 1);
	GLObject obj3d;
//	obj3d.vertexes(vv);
	auto sz = obj3d.read_obj_file(av[1]);
//	obj3d.colors({sz, {.5,1,.5}});
	obj3d.texture_file("google.jpg");
	GLObjs stage;
	stage += obj3d;
	stage.transfer_all();
	Matrix<float> light = {
		{0.2, 0.2, 0.2, 1}, //ambient
		{1, 1, 1, 1}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0, 2, 1} //position 1 means a point 0 means a vector light
	};
	stage.light(light);
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		stage.matrix(KeyBindMatrix);

		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
