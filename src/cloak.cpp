#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"globj.h"
#include"spring.h"
#define W 20
#define H 40
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	SpringConnection cloak{20, 20};
	GLObject ob;
	ob.vertexes(cloak);
	ob.indices(cloak.indices);
	ob.texture_file("brick.png");

	GLObjs stage;
	stage += ob;
	stage.transfer_all();
	stage.light({//default light
		{0.1, 0.1, 0.1, 1}, //ambient
		{0.5, 0.5, 0.5, 0.5}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0, -3, 1} //position 1 means a point 0 means a vector light
	});

	Matrix<float> m{4,4};
	float th = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
//		for(int i=0; i<20; i++) {
//			cloak[i+1][1].y0 = 0.1*sin(th);
//			cloak[i+1][1].z0 = 0.1*cos(th);
//		}
//		th += 0.1;
		cloak.time_pass(0.005);
//		stage.transfer_data(cloak, "vertexes_", stage.vbo[0]);
		stage.matrix(KeyBindMatrix * stage[0]);
		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(50ms);
	}
	glfwTerminate();
}
