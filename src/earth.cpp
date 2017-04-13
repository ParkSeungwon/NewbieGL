#include<thread>
#include"matrix.h"
#include"glutil.h"
using namespace std;

const int wt =  640, ht = 480;
extern Matrix<float> KeyBindMatrix;
int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;

	glortho(2);
	vector<Matrix<float>> v;
	auto pl = polygon(20);

	Matrix<float> m{4,4};
	m.glrotateY(M_PI/20);
	for(int i=0; i<20; i++) {
		for(auto& a : pl) {
			v.push_back(a);
			a = m * a;
			v.push_back(a);
		}
	}
	vector<Matrix<float>> color{v.size(), Matrix<float>{1,1,0}};
	vector<unsigned> index;
	for(int i=0; i<v.size(); i++) index.push_back(i);


	unsigned fv = gl_transfer_data(v);
	unsigned fc = gl_transfer_data(color);
	unsigned fe = gl_transfer_index(index);
	unsigned prog =
		make_shader_program("src/vertex_shader.glsl", "src/fragment_shader.glsl");

	float k = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(prog);
		gl_bind_data(fv, fc, fe);
		auto tm = KeyBindMatrix * m.glscale(0.2,0.2,0.2) * m.glrotateY(k);
		transfer_matrix(prog, tm, "KeyBindMatrix");
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(100));
		k += 0.1;
	}

	glfwTerminate();
}	
