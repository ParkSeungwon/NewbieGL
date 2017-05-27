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
	unsigned sz = obj3d.read_obj_file("BuddhaSculpture.obj");
	obj3d.texture_file("brick.png");
	Matrix<float> m{4,4};
	obj3d.matrix(m.gltranslate(-0.5,0,0)*m.glrotateY(M_PI/2)*m.glrotateX(M_PI/2) * m.glrotateX(M_PI) * m.glscale(0.3,0.3,0.3));

	GLObject ironman;
	sz = ironman.read_obj_file("ironman.obj");
	ironman.texture_file("google.jpg");
	ironman.matrix(m.glrotateX(-M_PI/2) * m.gltranslate(0.3,-0.2,0) * m.glscale(0.8,0.8,0.8));

	GLObject cube;
	Matrix<float> ve[8] = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
		{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}};
	vector<Matrix<float>> v, c;
	int idx[24] = {3,2,1,0, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 3,0,4,7};
	for(auto a : idx) v.push_back(ve[a]);
	for(int i=0; i<8; i++) for(int j=0; j<4; j++) {
		if(i==0 || i==6) continue;
		c.push_back(ve[i]);
	}
	vector<unsigned> id;
	for(int i=0; i<24; i++) id.push_back(i);
	cube.vertexes(v);
	//cube.colors(c);
	cube.texture_file("6.png");
	cube.indices(id);
	cube.matrix(m.glscale(0.1,0.1,0.1) * m.glortho(0,1,0,1,0,1));
	cube.mode(GL_QUADS);

	GLObject ship;
	ship.read_obj_file("space_frigate_6.obj");
	ship.texture_file("steel.png");

	GLObjs objs;
	objs += ironman;
	objs += obj3d;
	objs += ship;
	objs += cube;
	objs.transfer_all();

	Matrix<float> proj{4,4};
	proj.glprojection(-1,1,-1,1,-1,1);

	float k = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(int i=0; i<3; i++) {
			objs.matrix(proj * KeyBindMatrix * objs[i]);
			objs(i);
		}
		objs.matrix(proj * KeyBindMatrix * m.glrotateY(k) * m.gltranslate(0,0.5,0.4) * m.glrotateX(k) * objs[3]);
		objs(3);
		objs.matrix(KeyBindMatrix * m.gltranslate(0,0.5,0.4) * objs[3]);
		objs(3);

		k+= 0.1;
		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(50ms);
	}
	glfwTerminate();
}
