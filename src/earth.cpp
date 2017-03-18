#include<GLFW/glfw3.h>
#include"matrix.h"
#include"polygon.h"
#include"glutil.h"
using namespace std;

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(wt, ht, "Smiley Face", NULL, NULL);
	if (!glinit(window)) return -1;

	Polygon pl{1, 50};
}	
