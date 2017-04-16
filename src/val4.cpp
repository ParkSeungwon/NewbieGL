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

	GLObject obj3d(shader_program);
	unsigned sz = obj3d.read_obj_file(av[1], "a_pos");
	vector<Matrix<float>> color{sz, Matrix<float>{1,0,0}};
	obj3d.colors(color, "a_color");
	Matrix<float> m{4,4};
	obj3d.matrixes_.push_front(m.glscale(0.5,0.5,0.5));
//	obj3d.matrixes_.push_front(m.glortho(-4,0,-2,2,-2,2));
//	obj3d.matrixes_.push_front(m.gltranslate(0.5,0.5,0.5));
	cout << m ;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		transfer_matrix(shader_program, KeyBindMatrix * obj3d, "KeyBindMatrix");
		obj3d();
		transfer_matrix(shader_program, KeyBindMatrix * m.gltranslate(0.5,0,0) * obj3d, "KeyBindMatrix");
		obj3d();

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
