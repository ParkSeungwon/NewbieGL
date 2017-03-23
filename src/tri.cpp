#include"matrix.h"
#include"glutil.h"
using namespace std;

const int wt =  640, ht = 480;
extern Matrix<float> grotate;
extern Matrix<float> translate;
extern float camera_x, camera_y;
int main()
{
	grotate.glrotateY(M_PI/4);
	translate.gltranslate(0,0,sqrt(2));
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(3);

	auto pl = polygon(4, 1);
	vector<Matrix<float>> pl2{ begin(pl), end(pl)};
	pl = translate * pl;
	pl2.insert(pl2.end(), begin(pl), end(pl));
	for(auto& a : pl2) a = grotate * a;
	float color[72] = {1,0,0, 1,0,0, 1,0,0,1,0,0,
		0,1,0, 0,1,0,0,1,0,0,1,0,
		0,0,1, 0,0,1,0,0,1,0,0,1,
		1,1,0, 1,1,0,1,1,0,1,1,0,
		0,1,1, 0,1,1,0,1,1,0,1,1,
		1,0,1, 1,0,1,1,0,1,1,0,1};
	GLubyte idx[24] = {0,1,2,3, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 0,3,7,4};
	vector<Matrix<float>> v;
	for(auto& a : idx) v.push_back(pl2[a]);

	auto fc = gl_transfer_data(color, color + 72);
	auto fv = gl_transfer_data(v);
	auto fi = gl_transfer_data(idx, idx + sizeof(int)*24, GL_ELEMENT_ARRAY_BUFFER);
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

//		glLoadIdentity();
//		gluLookAt(camera_x, camera_y, 1, 0,0,0,0,1,0);

		glBindBuffer(GL_ARRAY_BUFFER, fc);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, fv);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);//3 float is 1 vertex stride 0, 
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fi);
	//	glEnableClientState(GL_INDEX_ARRAY);
	//	glIndexPointer(GL_QUADS, 4, 0);
	//	glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, idx);//
		glDrawArrays(GL_QUADS, 0, 24);//mode, first, count

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	//	glDisableClientState(GL_INDEX_ARRAY);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
		
	}
	glfwTerminate();

}
