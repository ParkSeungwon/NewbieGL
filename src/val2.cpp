#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"readobj.h"
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

	ObjReader obj3d(av[1]);
	vector<Matrix<float>> color{obj3d.vertexes.size(), Matrix<float>{1,0,0}};
	unsigned fv = gl_transfer_data(obj3d.vertexes);
	unsigned fc = gl_transfer_data(color);
	unsigned fe = gl_transfer_index(obj3d.indices);
	float mid[3] = {obj3d.range[3] - obj3d.range[0], 
					obj3d.range[4] - obj3d.range[1],
					obj3d.range[5] - obj3d.range[2]};
	Matrix<float> scale{4,4}, translate{4,4};
	scale.glscale(1.0/mid[0], 1.0/mid[1], 1.0/mid[2]);
	translate.gltranslate(mid[0]/2, mid[1]/2, mid[2]/2);
	gl_bind_data(fv, fc, fe);
	cout << "location is " << glGetAttribLocation(shader_program, "a_pos") << endl;
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto tm = KeyBindMatrix * scale;
		transfer_matrix(shader_program, tm, "KeyBindMatrix");

		glDrawElements(GL_TRIANGLES, obj3d.indices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
