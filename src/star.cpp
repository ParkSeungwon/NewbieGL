#include"turtle.h"
#include"glutil.h"
#include"globj.h"
using namespace std;

int main()
{
	if(!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(500, 500, "turtle", NULL, NULL);
	if(!glinit(window)) return -1;

	unsigned shader_program = 
		make_shader_program("src/vertex_shader.glsl", "src/fragment_shader.glsl");
	if(!shader_program) return 0;
	glUseProgram(shader_program);

	Matrix<float> light = {
		{0.1, 0.1, 0.1, 1}, //ambient
		{1, 1, 1, 1}, //diffuse
		{1, 1, 1, 1}, //specular
		{0, 0.3, 0, 1} //position 1 means a point 0 means a vector light
	};
	transfer_matrix(shader_program, light.transpose(), "LIGHT");

	vector<Matrix<float>> verts;
	Turtle tt{1,0,0};
	for(int i=0; i<37; i++) {
		tt.turn(170);
		tt.go(2);
		verts.push_back({tt.x(), tt.y(), 0});
	}
	vector<Matrix<float>> cols{verts.size(), {1,1,0}};
	vector<unsigned> idx;
	for(int i=0; i<verts.size(); i++) idx.push_back(i);

	GLObject star;
	star.vertexes(verts);
	star.colors(cols);
	star.indices(idx);
	star.mode(GL_LINE_STRIP);
	Matrix<float> m{4,4};
	star.matrix(m.glortho(-1.5,1.5,-1.5,1.5,-1.5,1.5));

	GLObjs stage{shader_program};
	stage += star;
	stage.transfer_all("a_pos", "a_color", "norm", "a_uv");

	extern Matrix<float> KeyBindMatrix;
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		transfer_matrix(shader_program, KeyBindMatrix * stage[0], "KeyBindMatrix");
		stage(0);

		glfwWaitEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}



