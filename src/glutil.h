#pragma once
void glortho(float r);
void glcolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
bool glinit(GLFWwindow* window);
std::vector<Matrix<float>> polygon(float r=1, int points_count=100);
