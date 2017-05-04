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
	obj3d.read_obj_file(av[1]);
	GLObjs stage{shader_program};
	stage += obj3d;
	stage.transfer_all("a_pos", "a_color", "norm");
	Matrix<float> light = {
		{0.2, 0.2, 0.2, 1}, //ambient
		{1, 1, 1, 1}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0, 2, 1} //position 1 means a point 0 means a vector light
	};
	transfer_matrix(shader_program, light.transpose(), "LIGHT");
	Matrix<float> scale{4,4}, translate{4,4};
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto tm = KeyBindMatrix * scale;
		transfer_matrix(shader_program, tm, "KeyBindMatrix");

		stage(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
