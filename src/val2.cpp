#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main()
{
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(640, 480, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	Matrix<float> mm{4,4};

	vector<Matrix<float>> color = {{1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}};
	vector<Matrix<float>> vertex = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}};
	unsigned element[4] = {0,1,2,3};

	unsigned fc = gl_transfer_data(color);
	unsigned fv = gl_transfer_data(vertex);
	unsigned fe = gl_transfer_data(element, element + 4, GL_ELEMENT_ARRAY_BUFFER);
	
	///compile shaders
	unsigned shader_program = 
		make_shader_program("src/vertex_shader.glsl", "src/fragment_shader.glsl",
				"a_pos", "a_color");
	if(!shader_program) return 0;
	Matrix<float> m{4,4};
	float k = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);
		auto tm = m.glscale(0.2,0.2,0.2) * KeyBindMatrix * m.glrotateX(k);
		transfer_matrix(shader_program, tm, "KeyBindMatrix");
		k += 0.1;

		gl_bind_data(fv, fc, fe);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
