#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"globj.h"
#include"fountain.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	Fountain foun;
	GLObject ob;
	ob.vertexes(foun.pos);
	ob.colors(vector<Matrix<float>>{1000, {1,0,0}});
	ob.mode(GL_POINTS);

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
		
		foun.time_pass(0.01);

//		stage.transfer_data(foun.pos, "vertexes_", stage.vbo[0]);
		stage.matrix(KeyBindMatrix * stage[0]);
		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(50ms);
	}
	glfwTerminate();
}

