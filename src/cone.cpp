#include"glutil.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

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
	Matrix<float> grotate{4,4};
	grotate.glrotateX(5*M_PI/4 + 0.1);//set rotate matrix
	for(auto& a : pl2) a = grotate * a;//rotate a little to have a good view
	grotate.E();
	auto fv = gl_transfer_data(pl2);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindBuffer(GL_ARRAY_BUFFER, fv);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, fv);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);//3 float is 1 vertex stride 0, 
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 102);//mode, first, count

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

