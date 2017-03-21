#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"matrix.h"
#include"glutil.h"
using namespace std;

const int wt =  640, ht = 480;
extern Matrix<float> rotate;
extern Matrix<float> translate;
int main()
{
	rotate.glrotateZ(M_PI/2);
	translate.gltranslate(0,0,sqrt(2)/2);
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(2);

	auto pl = polygon(4, 1);
	for(auto& a : pl) pl.push_back(translate * a);
	float color[] = {1,0,0,0,1,0,0,0,1};
	float ver[24];
	int idx[3] = {0,1,2};
	for(int i=0; i<pl.size(); i++) memcpy(ver + 3*i, pl[i].data(), 12);

	unsigned int vbo[3];//uint→used as a pointer
	vbo[0] = gl_transfer_data(color, 36);
	vbo[1] = gl_transfer_data(pl);
	vbo[2] = gl_transfer_data(idx, 12, GL_ELEMENT_ARRAY_BUFFER);
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
/*
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glColorPointer(3, GL_FLOAT, 0, color);
		glVertexPointer(3, GL_FLOAT, 0, ver);
		glDrawArrays(GL_TRIANGLES, 0,9);
		
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
*/
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0,0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0,0);//stride, 
		
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	//	glEnableClientState(GL_ELEMENT_ARRAY);
	//	glPolygonMode(GL_FRONT, GL_LINE);
	//	glDrawElements(GL_QUADS, 3, GL_INT, 0);
		glDrawArrays(GL_QUADS, 0,8);

	//	glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	//	glDisableClientState(GL_ELEMENT_ARRAY);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
		
	}
	glfwTerminate();

}
