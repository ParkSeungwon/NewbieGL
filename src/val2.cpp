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
	auto sz = obj3d.read_obj_file(av[1]);
	obj3d.texture_file(av[2]);
	GLObjs stage;
	stage += obj3d;
	stage.transfer_all();
	
	Matrix<float> m{4,4};
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(int i=0; i<10; i++) for(int j=0; j<10; j++) {
			stage.matrix(KeyBindMatrix * m.gltranslate(-1+0.2*i, -1+0.2*j, 0) * m.glscale(0.1,0.1,0.1));
			stage(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
