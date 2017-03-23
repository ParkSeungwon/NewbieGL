#include"glutil.h"
using namespace std;
extern Matrix<float> grotate;
extern Matrix<float> translate;

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(640, 480, "Cone", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(3);

	auto pl = polygon(100);//return 100 circle vertexes
	vector<Matrix<float>> pl2;
	pl2.push_back({0,0,3});//top point of cone
	pl2.insert(pl2.end(), begin(pl), end(pl));
	pl2.push_back(pl[0]);
	grotate.glrotateX(5*M_PI/4 + 0.1);
	for(auto& a : pl2) a = grotate * a;//rotate a little to have a good view
	grotate.E();
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBegin(GL_TRIANGLE_FAN);
		for(auto& a : pl2) {
			glColor3fv(a.data());
			a = grotate * a;
			glVertex3fv(a.data());
		}
		glEnd();
		glfwSwapBuffers(window);

		glfwPollEvents();
		
	}
	glfwTerminate();

}

