#include<GLFW/glfw3.h>
#include<iostream>
using namespace std;

bool record = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_U && action == GLFW_PRESS) cout << 'D';
	if(key == GLFW_KEY_D && action == GLFW_PRESS) cout << 'U';
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
	glClearColor(1, 1, 1, 1); // white background
	return true;
}
