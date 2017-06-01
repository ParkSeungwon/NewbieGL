#include<iostream>
#include<chrono>
#include<thread>
#include"glutil.h"
#include"globj.h"
#include"chunk.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

bool sphere(int x, int y, int z, int cx, int cy, int cz, int r) 
{//inside sphere?
	return (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) < r*r ? true : false;
}

bool cube(int x, int y, int z, int a, int b, int c, int d)
{
	return a*x + b*y + c*z + d < 0 ? true : false;
}

int main()
{
	using namespace std::placeholders;
	Chunk ch{30,30,30};
	ch.subtract(bind(sphere, _1, _2, _3, 0,0,0,20));

	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	Matrix<float> m{4,4};
	GLObject obj3d;
	obj3d.vertexes(ch.vertexes_);
	ch.indices();
	obj3d.indices(ch.indices_);
	obj3d.texture_file("brick.png");
	obj3d.mode(GL_QUADS);

	GLObject ship;
	ship.read_obj_file("space_frigate_6.obj");
	ship.texture_file("google.jpg");

	GLObjs stage;
	stage += obj3d;
	stage += ship;
	stage.transfer_all();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		stage.matrix(KeyBindMatrix * stage[0]);
	//	stage.indices(ch.indices_, stage.vbo[3]);
		stage(0);
		stage(1);

		glfwSwapBuffers(window);
		glfwPollEvents();
		//this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}

