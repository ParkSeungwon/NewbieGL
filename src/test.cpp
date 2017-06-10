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

bool odd(int x, int y, int z) {
	return !(x % 5 && y % 5 && z % 5);
}

int main()
{
	using namespace std::placeholders;
	Chunk ch{100,100,100};//stack size limit warning

	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	Matrix<float> m{4,4};
	GLObject obj3d;
	obj3d.vertexes(ch.vertexes_);
//	obj3d.colors({ch.vertexes_.size(), {1,0,0}});
//	ch.subtract(odd);//set bool
	obj3d.indices(ch.indices());
	obj3d.texture_file("marble.jpg");
	obj3d.mode(GL_QUADS);

	GLObjs stage;
	stage += obj3d;
	stage.transfer_all();
	ch.subtract(bind(sphere, _1, _2, _3, 0,0,0,100));//set bool
	stage.update(ch.indices());
//	stage.index_chunks_[0] = 2000000;
	
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

