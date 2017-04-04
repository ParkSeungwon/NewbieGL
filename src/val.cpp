#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;


static char vertex_shader[1000] = R"(
#version 130
uniform float Scale;
uniform mat4 glModelViewMarix;
mat4 m1 = mat4(anchor1);
mat4 m2 = mat4(1.0);

in vec3 a_pos;
in vec3 a_color;

out vec4 v_color;

void main() {
	gl_Position = m1 * m2 * vec4(a_pos * Scale, 1.0);
	v_color = vec4(a_pos, 1);
}
)";
static char fragment_shader[1000] = R"(
#version 130
uniform float Scale;
out vec4 f_color;
in vec4 v_color;
void main() {
//	f_color = vec4(1.0, 0.0, 0.0, 1.0);
	f_color = v_color * Scale;
}
)";

int main()
{
	if (!glfwInit()) return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	
	GLFWwindow* window = glfwCreateWindow(640, 480, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(10);	

	vector<Matrix<float>> colors, vertexes, vtx;
	Matrix<float> mm{4,4};
	replace(vertex_shader, "anchor1", mm.glrotate(0,0,10)* mm.glscale(0.1,0.1,0.1));

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
	float k = 0;

	gluLookAt(1.2, 0.8, 1.2, 0.5, 0.5, 0.5, 0,1,0);

	///compile shaders
	cout << vertex_shader << endl;
	unsigned shader_program = 
		make_shader_program(vertex_shader, fragment_shader, "a_pos");
	if(!shader_program) return 0;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);

		int loc = glGetUniformLocation(shader_program, "Scale");
		if(loc != -1) {
			static float scale = 0.0f;
			scale += 0.1f;
			glUniform1f(loc, sin(scale));
		}

		glBindBuffer(GL_ARRAY_BUFFER, fv);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//attribute 0, xyx3, float, normalized?, stride, offset

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fe);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
		/*
		//공전하는 긴 막대
		v = KeyBindMatrix * m.glrotateX(k) * m.gltranslate(0,3,0) * 
			m.glscale(1,3,1) * cube;
		gl_transfer_data(v, fv);
		bindNdraw(fc, fv, GL_QUADS, 0, 24, fe);
		//스스로 자전하는 큐브
		v = KeyBindMatrix * m.glrotateX(k) * m.gltranslate(0,8,0) *
			m.glrotateZ(k) * cube;
		gl_transfer_data(v, fv);
		bindNdraw(fc, fv, GL_QUADS, 0, 24, fe);
		k += 0.1;//회전각도를 증가
		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
