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

	///compile shaders
	unsigned shader_program = 
		make_shader_program("src/vertex_shader.glsl", "src/fragment_shader.glsl");
	if(!shader_program) return 0;
	glUseProgram(shader_program);

	GLObject obj3d;
	unsigned sz = obj3d.read_obj_file("BuddhaSculpture.obj");
	obj3d.texture_file("brick.png");
//	obj3d.colors(color);
	Matrix<float> m{4,4};
	obj3d.matrix(m.glrotateX(M_PI/2) * m.glrotateX(M_PI) * m.glscale(0.3,0.3,0.3));

	GLObject ironman;
	sz = ironman.read_obj_file("ironman.obj");
	ironman.texture_file("google.jpg");
//	vector<Matrix<float>> col{sz, {1,1,0}};
//	ironman.colors(col);
	ironman.matrix(m.glrotateX(-M_PI/2) * m.gltranslate(0.3,-0.2,0) * m.glscale(0.8,0.8,0.8));

	GLObject cube;
	Matrix<float> ve[8] = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
		{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}};
	vector<Matrix<float>> v, c;
	int idx[24] = {0,1,2,3, 4,5,6,7, 0,4,5,1, 1,5,6,2, 2,6,7,3, 0,4,7,3};
	for(auto a : idx) v.push_back(ve[a]);
	for(int i=0; i<8; i++) for(int j=0; j<4; j++) {
		if(i==0 || i==6) continue;
		c.push_back(ve[i]);
	}
	vector<unsigned> id;
	for(int i=0; i<24; i++) id.push_back(i);
	cube.vertexes(v);
	cube.colors(c);
	cube.indices(id);
	cube.matrix(m.glscale(0.1,0.1,0.1) * m.glortho(0,1,0,1,0,1));
	cube.mode(GL_QUADS);

	GLObjs objs(shader_program);
	objs += ironman;
	objs += obj3d;
	objs += cube;
	objs.transfer_all();


	Matrix<float> light = {
		{0.2, 0.2, 0.2, 1}, //ambient
		{1, 1, 1, 1}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0, 2, 1} //position 1 means a point 0 means a vector light
	};
	transfer_matrix(shader_program, light.transpose(), "LIGHT");
	Matrix<float> proj{4,4};
	proj.glprojection(-1,1,-1,1,-1,1);

	float k = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		transfer_matrix(shader_program, KeyBindMatrix*objs[0], "KeyBindMatrix");
		objs(0);
		transfer_matrix(shader_program, KeyBindMatrix*objs[1], "KeyBindMatrix");
		objs(1);
		transfer_matrix(shader_program, proj * KeyBindMatrix * m.glrotateY(k) * m.gltranslate(0,0.5,0.4) * m.glrotateX(k) * objs[2], "KeyBindMatrix");
		objs(2);

		k+= 0.1;
		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(50ms);
	}
	glfwTerminate();
}
