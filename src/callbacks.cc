#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<valarray>
#include<fstream>
#include"matrix.h"
#define STEP 0.05
using namespace std;
Matrix<float> KeyBindMatrix{4,4};

static Matrix<float> m{4,4};
bool record = false;
float camera_x=1, camera_y=1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{// && action == GLFW_PRESS) 
	switch(key) {
	case GLFW_KEY_LEFT:
		KeyBindMatrix = m.gltranslate(-STEP, 0, 0) * KeyBindMatrix; break;
	case GLFW_KEY_DOWN:
		KeyBindMatrix = m.gltranslate(0, -STEP, 0) * KeyBindMatrix; break;
	case GLFW_KEY_RIGHT:
		KeyBindMatrix = m.gltranslate(STEP, 0, 0) * KeyBindMatrix; break;
	case GLFW_KEY_UP:
		KeyBindMatrix = m.gltranslate(0, STEP, 0) * KeyBindMatrix; break;
	case GLFW_KEY_Z:
		KeyBindMatrix = m.glscale(1+STEP, 1+STEP, 1+STEP) * KeyBindMatrix; break;
	case GLFW_KEY_X:
		KeyBindMatrix = m.glscale(1-STEP, 1-STEP, 1-STEP) * KeyBindMatrix; break;

	case GLFW_KEY_W: KeyBindMatrix = m.glrotateX(STEP) * KeyBindMatrix; break;
	case GLFW_KEY_A: KeyBindMatrix = m.glrotateY(-STEP) * KeyBindMatrix; break;
	case GLFW_KEY_S: KeyBindMatrix = m.glrotateX(-STEP) * KeyBindMatrix; break;
	case GLFW_KEY_D: KeyBindMatrix = m.glrotateY(STEP) * KeyBindMatrix; break;
	case GLFW_KEY_Q: KeyBindMatrix = m.glrotateZ(-STEP) * KeyBindMatrix; break;
	case GLFW_KEY_E: KeyBindMatrix = m.glrotateZ(STEP) * KeyBindMatrix; break;

	case GLFW_KEY_SPACE: KeyBindMatrix.E(); break;

	case GLFW_KEY_J: camera_x -= STEP; break;
	case GLFW_KEY_K: camera_y -= STEP; break;
	case GLFW_KEY_L: camera_x += STEP; break;
	case GLFW_KEY_I: camera_y += STEP; break;
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	KeyBindMatrix = m.glscale(1+yoffset, 1+yoffset, 1+yoffset) * KeyBindMatrix;
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

bool glinit(GLFWwindow* window) 
{
	if(!window) {
		glfwTerminate();
		return false;
	}
	// callbacks here
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(0, 0, 0, 0); // black background
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//	glCullFace(GL_FRONT_AND_BACK);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return false;
	}
	if (glewIsSupported("GL_VERSION_3_3")) printf("Ready for OpenGL 3.3\n");
    else {
        printf("OpenGL 3.3 not supported\n");
        exit(1);
    }
	KeyBindMatrix.E();
	cout << "shading language version : " << 
			glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << glGetString( GL_VENDOR ) << endl;
	cout << glGetString( GL_RENDERER ) << endl;
	cout << glGetString( GL_VERSION   ) << endl;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return true;
}

std::vector<Matrix<float>> polygon(int points_count, float r)
{
	Matrix<float> p{r, 0, 0};//when arg is 3 or 4, it makes 4x1 matrix r,0,0,1
	Matrix<float> rz{4, 4};//this makes 4x4 matrix
	vector<Matrix<float>> pts;//{Matrix<float>{0,0,0}, points_count};
	rz.glrotateZ(2 * M_PI / points_count);
	for(int i=0; i<points_count; i++) {
		pts.push_back(p);
		p = rz * p;
	}
	return pts;
}

string read_file(string file)
{
	string r;
	char c;
	ifstream f(file);
	while(f >> noskipws >> c) r += c;
	return r;
}

unsigned make_shader_program(string v_shader, string f_shader)
{
	auto* vp = v_shader.data();
	auto* fp = f_shader.data();
	const char** vertex_shader = &vp;
	const char** fragment_shader = &fp;

	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vertex_shader, NULL);
	glCompileShader(vs);
	int status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) cerr << "compile error in vertex shader" << endl;
	
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fragment_shader, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) cerr << "compile error in fragment shader" << endl;

	unsigned shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);

	//linking error message
	int linked = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);
	if(!linked)  {
		int infolen = 0;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &infolen);
		if(infolen > 1) {
			char* infoLog = (char*)malloc(sizeof(char) * infolen);
			glGetProgramInfoLog(shader_program, infolen, NULL, infoLog);
			cout << "error linking program\n" << infoLog << endl;
			free(infoLog);
		}
		glDeleteProgram(shader_program);
		return 0;
	}
	return shader_program;
}
void transfer_matrix(unsigned shader_program, const Matrix<float>& m, 
		const char* var_name)
{
	int fd = glGetUniformLocation(shader_program, var_name);
	if(fd != -1) glUniformMatrix4fv(fd, 1, GL_FALSE, m.data());
}
