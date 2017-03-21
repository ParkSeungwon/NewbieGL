#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<valarray>
#include"matrix.h"
using namespace std;
Matrix<float> translate{4,4};
Matrix<float> rotate{4,4};
static Matrix<float> m{4,4};
bool record = false;
float camera_x=1, camera_y=1;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) translate[4][1]-=0.01;
	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) translate[4][2]-=0.01;
	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) translate[4][1]+=0.01;
	if(key == GLFW_KEY_UP && action == GLFW_PRESS) translate[4][2]+=0.01;
	if(key == GLFW_KEY_W && action == GLFW_PRESS) rotate *= m.glrotateX(0.01);
	if(key == GLFW_KEY_A && action == GLFW_PRESS) rotate *= m.glrotateY(-0.01);
	if(key == GLFW_KEY_S && action == GLFW_PRESS) rotate *= m.glrotateX(-0.01);
	if(key == GLFW_KEY_D && action == GLFW_PRESS) rotate *= m.glrotateY(0.01);
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) rotate.E();
	if(key == GLFW_KEY_J && action == GLFW_PRESS) camera_x-=0.1;
	if(key == GLFW_KEY_K && action == GLFW_PRESS) camera_y-=0.1;
	if(key == GLFW_KEY_L && action == GLFW_PRESS) camera_x+=0.1;
	if(key == GLFW_KEY_I && action == GLFW_PRESS) camera_y+=0.1;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {//GLFW_RELEASE
        glfwGetCursorPos(window, &x, &y);
		cout << '(' << x / 4 << ',' << y / 4 << ')' << flush;
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	if(record) cout << xpos << ' ' << ypos << ' ' << flush;
}

void glortho(float r) {
	glOrtho(-r,r,-r,r,-r,r);
}
void glcolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	glColor4f(float(r)/256, float(g)/256, float(b)/256, float(a)/256);
}
bool glinit(GLFWwindow* window) {
	if(!window) {
		glfwTerminate();
		return false;
	}
	// callbacks here
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(0, 0, 0, 0); // white background

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return false;
	}
	return true;
}

std::vector<Matrix<float>> polygon(int points_count, float r)
{
	Matrix<float> p{r, 0, 0};//when arg is 3 or 4, it makes 4x1 matrix r,0,0,1
	Matrix<float> rz{4, 4};//this makes 4x4 matrix
	std::vector<Matrix<float>> pts;
	rz.glrotateZ(2 * M_PI / points_count);
	for(int i=0; i<points_count; i++) {
		pts.push_back(p);
		p = rz * p;
	}
	return pts;
}

unsigned int gl_transfer_data(void* p, size_t sz, GLenum mode)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);
	glBufferData(mode, sz, p, GL_STATIC_DRAW);
	return vbo;
}

unsigned int gl_transfer_data(const vector<Matrix<float>>& ver, GLenum mode)
{
	const int dim = 3;
	int sz = ver.size();
	float ar[sz * dim];
	for(int i=0; i<sz; i++) memcpy(ar + dim * i, ver[i].data(), sizeof(float) * dim);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);
	glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	return vbo;
}

unsigned int gl_transfer_data(const valarray<Matrix<float>>& ver, GLenum mode)
{
	const int dim = 3;
	int sz = ver.size();
	float ar[sz * dim];
	for(int i=0; i<sz; i++) memcpy(ar + dim * i, ver[i].data(), sizeof(float) * dim);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);
	glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	return vbo;
}
