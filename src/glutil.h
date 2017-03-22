#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<valarray>
#include<type_traits>
#include<typeinfo>
#include"matrix.h"

void glortho(float r);
void glcolor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
bool glinit(GLFWwindow* window);
std::valarray<Matrix<float>> polygon(int points_count=100, float r=1);

template <typename T>
unsigned int gl_transfer_data(T* begin, T* end, GLenum mode = GL_ARRAY_BUFFER)
{
	int sz = end - begin;
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);
	
	T ar[sizeof(T) * sz];
	for(int i=0; i<sz; i++) ar[i] = begin[i];
	glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	
	return vbo;
}
void mcopy(float* p, const Matrix<float>& m) {
	memcpy(p, m.data(), 3 * sizeof(float));
}
void mcopy(float* p, float m) {
	*p = m;
}

template <typename T> 
unsigned int gl_transfer_data(const T& v, GLenum mode = GL_ARRAY_BUFFER)
{
	int dim = 1;
	int sz = v.size();
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);

	if(std::is_class<typename T::value_type>::value) dim = 3;
	float ar[sz * dim];
	for(int i=0; i<sz; i++) mcopy(ar+i*dim, v[i]);//memcpy(ar + dim * i, v[i].data(), sizeof(float) * dim);
	glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
/*	} else {
		float ar[sz];
		for(int i=0; i<sz; i++) ar[i] = v[i];
		glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	}
*/
	return vbo;
}
