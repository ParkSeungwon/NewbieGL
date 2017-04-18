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
	unsigned sz = obj3d.read_obj_file("monkey.obj");
	vector<Matrix<float>> color{sz, Matrix<float>{1,0,0}};
	obj3d.colors(color);
	Matrix<float> m{4,4};
	obj3d.matrix(m.glscale(0.3,0.3,0.3));
	obj3d.normals();
	GLObject ironman;
	sz = ironman.read_obj_file("ironman.obj");
	vector<Matrix<float>> col{sz, Matrix<float>{1,1,0}};
	ironman.colors(col);
	ironman.matrix(m.glrotateX(-M_PI/2) * m.gltranslate(0.3,-0.2,0) * m.glscale(0.01,0.01,0.01));
	ironman.normals();

	GLObjs objs(shader_program);
	objs += ironman;
	objs += obj3d;
	objs.transfer_all("a_pos", "a_color", "norm");

	Matrix<float> light = {
		{0.1, 0.1, 0.1, 1}, //ambient
		{1, 1, 1, 1}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0.3, 0, 1} //position 1 means a point 0 means a vector light
	};
	Matrix<float> material = {
		{1, 0.1, 0.1, 1}, //ambient
		{1, 0.1, 0.1, 1}, //diffuse
		{0.2, 0.2, 0.2, 1}, //specular
		{0, 0, 0, 1} //emission
	};

//	set_light(light);
//	set_material(material, 10);

	float k = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(int i=0; i<2; i++) {
			transfer_matrix(shader_program, KeyBindMatrix * objs[i], "KeyBindMatrix");
			objs(i);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
