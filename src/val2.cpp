#include<chrono>
#include<thread>
#include<iostream>
#include<valarray>
#include"glutil.h"
#include"globj.h"
#include"plot.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main(int ac, char** av)
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	auto th = arange(0, 0.01, M_PI * 8);
	valarray<float> x = cos(th), y = sin(th), z = th / 9.2f;
	vector<Matrix<float>> v;
	for(int i=0; i<x.size(); i++) v.push_back({x[i], y[i], z[i]});

	Matrix<float> m{4,4};
	GLObject obj3d;
	obj3d.vertexes(v);
	obj3d.colors(vector<Matrix<float>>{x.size(), {0,1,0}});
	obj3d.mode(GL_LINE_STRIP);
	
	GLObjs stage;
	stage += obj3d;
	stage.transfer_all();
	Matrix<float> lt {
		{1, 1, 1, 1}, //ambient
		{1, 1, 1, 1}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0, 0, 0} //position 1 means a point 0 means a vector light
	};
	stage.light(lt);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		stage.matrix(m.glortho(-1.5, 1.5, -1.5, 1.5, -2,2) * KeyBindMatrix * stage[0]);
		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		//this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
