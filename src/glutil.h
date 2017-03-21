#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<valarray>
#include"matrix.h"
#include"polygon.h"
template<typename T> class Matrix;

void glortho(float r);
void glcolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
bool glinit(GLFWwindow* window);
std::vector<Matrix<float>> polygon(int points_count=100, float r=1);
unsigned int gl_transfer_data(void* p, size_t sz, GLenum mode = GL_ARRAY_BUFFER);
unsigned int gl_transfer_data(const std::vector<Matrix<float>>& ver, GLenum mode = GL_ARRAY_BUFFER);
unsigned int gl_transfer_data(const std::valarray<Matrix<float>>& ver, GLenum mode = GL_ARRAY_BUFFER);
