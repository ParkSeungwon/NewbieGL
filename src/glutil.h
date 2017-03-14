#include<GLFW/glfw3.h>
using namespace std;

void glortho(float r) {
	glOrtho(-r,r,-r,r,-r,r);
}

void glcolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	glColor4f(float(r)/256, float(g)/256, float(b)/256, float(a)/256);
}
