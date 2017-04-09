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
//	glortho(10);	

	vector<Matrix<float>> colors, vertexes, vtx;
	Matrix<float> mm{4,4};

	auto pl = polygon(4);
	vtx.insert(vtx.end(), begin(pl), end(pl));
	Matrix<float> m{4,4};
	pl = m.gltranslate(0,0,sqrt(2)) * pl;//z+1
	vtx.insert(vtx.end(), begin(pl), end(pl));//append elevated 4 vertex
	int idx[24] = {0,1,2,3, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 0,3,7,4};
	for(int a : idx) vertexes.push_back(vtx[a]);
	for(auto& a : vertexes) a = m.glrotateZ(M_PI/4) * a;

	Matrix<float> clr[] = {{1,0,0}, {0,1,0}, {0,0,1}, {1,1,0}, {1,0,1}, {0,1,1}};
	for(int i=0; i<6; i++) for(int j=0; j<4; j++) colors.push_back(clr[i]);

	unsigned element[24];
	for(int i=0; i<24; i++) element[i] = i;

	unsigned fc = gl_transfer_data(colors);
	unsigned fv = gl_transfer_data(vertexes);
	unsigned fe = gl_transfer_data(element, element + 24, GL_ELEMENT_ARRAY_BUFFER);
	
	const valarray<Matrix<float>> cube{vertexes.data(), vertexes.size()};
	valarray<Matrix<float>> v;

	///compile shaders
	unsigned shader_program = 
		make_shader_program("src/vertex_shader.glsl", "src/fragment_shader.glsl",
				"a_pos", "a_color");
	if(!shader_program) return 0;
	float k = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);
		auto tm = m.glscale(0.2,0.2,0.2) * KeyBindMatrix * m.glrotateX(k);
		transfer_matrix(shader_program, tm, "KeyBindMatrix");
		k += 0.1;

		gl_bind_data(fv, fc, fe);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
