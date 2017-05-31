#include<iostream>
#include"glutil.h"
#include"globj.h"
#include"spring.h"
#define W 20
#define H 40
using namespace std;

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;

	SpringConnection cloak{20, 20};
	GLObject ob;
	ob.vertexes(cloak);

}
