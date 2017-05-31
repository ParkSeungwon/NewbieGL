#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"globj.h"
#include"spring.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main(int ac, char** av)
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	Matrix<float> m{4,4};
	GLObject obj3d;
	obj3d.read_obj_file(av[1]);
	obj3d.matrix(m.glrotateY(-M_PI/2) * m.glrotateX(-M_PI/2) * m.glscale(0.8,0.8,0.8));
	
//	obj3d.subdiv_triangle();
//	obj3d.subdiv_triangle();
//	obj3d.butterfly();
//	obj3d.butterfly();
	obj3d.texture_file(av[2]);
	GLObjs stage;
	stage += obj3d;
	stage.transfer_all();
	float th = 0;
	Matrix<float> lt {
		{0.1, 0.1, 0.1, 1}, //ambient
		{0.5, 0.5, 0.5, 0.5}, //diffuse
		{1, 1, 1, 1}, //specular
		{3, 3, -3, 1} //position 1 means a point 0 means a vector light
	};
	SpringModel3D sp;
	float t = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sp.time_pass(0.3 * sin(0.2*t), 0, 0.1 * cos(0.2*t));
		t+= 0.1;
		cout << sp.x << ' ' << sp.y << ' ' << sp.z << endl;
		stage.matrix(m.glprojection(-1,1,-1,1,-1,1)*KeyBindMatrix *  m.gltranslate(sp.x, sp.y, sp.z) * stage[0]);
		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		//this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
