#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<valarray>
#include"matrix.h"
using namespace std;
Matrix<float> KeyBindMatrix{4,4};
static Matrix<float> m{4,4};
bool record = false;
float camera_x=1, camera_y=1;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{// && action == GLFW_PRESS) 
	switch(key) {
	case GLFW_KEY_LEFT:
		KeyBindMatrix = m.gltranslate(-0.01, 0, 0) * KeyBindMatrix; break;
	case GLFW_KEY_DOWN:
		KeyBindMatrix = m.gltranslate(0, -0.01, 0) * KeyBindMatrix; break;
	case GLFW_KEY_RIGHT:
		KeyBindMatrix = m.gltranslate(0.01, 0, 0) * KeyBindMatrix; break;
	case GLFW_KEY_UP:
		KeyBindMatrix = m.gltranslate(0, 0.01, 0) * KeyBindMatrix; break;

	case GLFW_KEY_W: KeyBindMatrix = m.glrotateX(0.01) * KeyBindMatrix; break;
	case GLFW_KEY_A: KeyBindMatrix = m.glrotateY(-0.01) * KeyBindMatrix; break;
	case GLFW_KEY_S: KeyBindMatrix = m.glrotateX(-0.01) * KeyBindMatrix; break;
	case GLFW_KEY_D: KeyBindMatrix = m.glrotateY(0.01) * KeyBindMatrix; break;

	case GLFW_KEY_SPACE: KeyBindMatrix.E(); break;
	case GLFW_KEY_J: camera_x -= 0.1; break;
	case GLFW_KEY_K: camera_y -= 0.1; break;
	case GLFW_KEY_L: camera_x += 0.1; break;
	case GLFW_KEY_I: camera_y += 0.1; break;
	}
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return false;
	}
	KeyBindMatrix.E();
	return true;
}

std::valarray<Matrix<float>> polygon(int points_count, float r)
{
	Matrix<float> p{r, 0, 0};//when arg is 3 or 4, it makes 4x1 matrix r,0,0,1
	Matrix<float> rz{4, 4};//this makes 4x4 matrix
	std::valarray<Matrix<float>> pts{Matrix<float>{0,0,0}, points_count};
	rz.glrotateZ(2 * M_PI / points_count);
	for(int i=0; i<points_count; i++) {
		pts[i] = p;
		p = rz * p;
	}
	return pts;
}

