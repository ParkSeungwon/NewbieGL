#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"tetris.h"

using namespace std;
extern Matrix<float> KeyBindMatrix;

int main()
{
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(640, 1000, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	Tetris tetris(15, 30);

	unsigned fv = gl_transfer_data(tetris.vertexes);
	unsigned fc = gl_transfer_data(tetris.colors);
	vector<unsigned> element;
	for(int j=0,k=1; j<30; j++) for(int i=0; i<15; i++) {
		auto array = tetris.get_cube_element(i, j, k++ % 6);
		for(auto a : array) element.push_back(a);
	}
	unsigned fe = gl_transfer_index(element);
	
	Matrix<float> m{4,4};
	///compile shaders
	unsigned shader_program = 
		make_shader_program("src/vertex_shader.glsl", "src/fragment_shader.glsl");
	if(!shader_program) return 0;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);
		auto tm = m.glscale(0.1,0.064,0.1) * KeyBindMatrix;
		transfer_matrix(shader_program, tm, "KeyBindMatrix");

		gl_bind_data(fv, fc, fe);
		glDrawElements(GL_QUADS, element.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
