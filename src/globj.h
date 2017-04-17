#pragma once
#include<deque>
#include<vector>
#include"matrix.h"
class GLObject
{
public:
	GLObject();
	unsigned read_obj_file(std::string filename);

	//setters
	void mode(GLenum md);
	void matrix(const Matrix<float>& m);
	void vertexes(const std::vector<Matrix<float>>& verts);
	void vertexes(std::vector<Matrix<float>>&& verts);
	void colors(const std::vector<Matrix<float>>& cols);
	void colors(std::vector<Matrix<float>>&& cols);
	void normals(const std::vector<Matrix<float>>& norms);
	void normals(std::vector<Matrix<float>>&& norms);
	void indices(const std::vector<unsigned>& ids);
	void indices(std::vector<unsigned>&& ids);

protected:
	std::vector<Matrix<float>> vertexes_, colors_, normals_;
	std::vector<unsigned> indices_;
	Matrix<float> matrix_;
	GLenum mode_ = GL_TRIANGLES;
	friend class GLObjs;
};

class GLObjs : public GLObject
{
public:
	GLObjs(unsigned shader_program);
	unsigned indices(const std::vector<unsigned>& v, unsigned vbo=0);
	void operator()(int n);
	Matrix<float> operator[](int n);
	GLObjs& operator+=(const GLObject& r);
	void transfer_all(const char* v_var, const char* c_var);

protected:
	unsigned shader_program_;
	std::vector<unsigned> index_chunks_;
	std::vector<Matrix<float>> matrixes_;
	std::vector<GLenum> modes_;
	unsigned vbo[3];

private:
	unsigned transfer_data(const std::vector<Matrix<float>>& v, const char* var,
			unsigned vbo=0);
};
