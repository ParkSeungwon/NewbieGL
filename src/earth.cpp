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
	glColor3f(1,1,0);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		for(auto& a : v) {
			a = KeyBindMatrix * a;
			glVertex2fv(a.data());
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	glfwTerminate();
}	
