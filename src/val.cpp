#include"glutil.h"
using namespace std;
extern Matrix<float> grotate;
extern Matrix<float> translate;

int main()
{
	grotate.glrotateY(M_PI/4);
	translate.gltranslate(0,0,sqrt(2));
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(640, 480, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(3);

	auto pl = polygon(4);
	vector<Matrix<float>> pl2;
	pl2.insert(pl2.end(), begin(pl), end(pl));
	pl = translate * pl;
	pl2.insert(pl2.end(), begin(pl), end(pl));
	for(auto& a : pl2) a = grotate * a;
	valarray<float> color{0.0, 72};
	color[slice(0,4,3)] = 1;
	color[slice(13,4,3)] = 1;
	color[slice(26,4,3)] = 1;
	color[gslice(36,{4,2},{3,1})] = 1;
	color[gslice(49,{4,2},{3,1})] = 1;
	color[gslice(60,{4,2},{3,2})] = 1;

/*	float color[72] = {1,0,0, 1,0,0, 1,0,0,1,0,0,
		0,1,0, 0,1,0,0,1,0,0,1,0,
		0,0,1, 0,0,1,0,0,1,0,0,1,
		1,1,0, 1,1,0,1,1,0,1,1,0,
		0,1,1, 0,1,1,0,1,1,0,1,1,
		1,0,1, 1,0,1,1,0,1,1,0,1};
*/	int idx[24] = {0,1,2,3, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 0,3,7,4};
	vector<Matrix<float>> v;
	for(auto& a : idx) v.push_back(pl2[a]);

	float fd[72];
	for(int i=0; i<72; i++) fd[i] = color[i];
	auto fc = gl_transfer_data(fd, 72*4);
	auto fv = gl_transfer_data(v);
	auto fi = gl_transfer_data(idx, sizeof(int)*24, GL_ELEMENT_ARRAY_BUFFER);
	
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
		
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fi);
	//	glEnableClientState(GL_INDEX_ARRAY);
	//	glIndexPointer(GL_QUADS, 4, 0);
	//	glPolygonMode(GL_FRONT, GL_LINE);
	//	glDrawElements(GL_QUADS, 24, GL_INT, nullptr);//
		glDrawArrays(GL_QUADS, 0, 24);//mode, first, count

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	//	glDisableClientState(GL_INDEX_ARRAY);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
		
	}
	glfwTerminate();

}
