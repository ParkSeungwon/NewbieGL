#include"turtle.h"
#include"glutil.h"
using namespace std;

int main()
{
	if(!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(500, 500, "turtle", NULL, NULL);
	if(!glinit(window)) return -1;

	glColor3f(1,1,0);
	glortho(1.5);

	while(!glfwWindowShouldClose(window)) {
		glBegin(GL_LINE_STRIP);
		Turtle tt{1,0,0};
		for(int i=0; i<37; i++) {
			tt.turn(170);
			tt.go(2);
			glVertex2f(tt.x(), tt.y());
		}
		glEnd();
		glfwWaitEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}



