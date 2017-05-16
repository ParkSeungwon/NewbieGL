#pragma once
#include<deque>
#include<vector>
#include"matrix.h"
class GLObject
{
public:
	GLObject();
	unsigned read_obj_file(std::string filename);
	unsigned read_texture(std::string filename);

	//setters
	void mode(GLenum md);
	void matrix(const Matrix<float>& m);
	void vertexes(const std::vector<Matrix<float>>& verts);
	void vertexes(std::vector<Matrix<float>>&& verts);
	void colors(const std::vector<Matrix<float>>& cols);
	void colors(std::vector<Matrix<float>>&& cols);
	void indices(const std::vector<unsigned>& ids);
	void indices(std::vector<unsigned>&& ids);
	void normals();

protected:
	std::vector<Matrix<float>> vertexes_, colors_, normals_, tex_uv_;
	std::vector<unsigned> indices_;
	Matrix<float> matrix_;
	GLenum mode_ = GL_TRIANGLES;
	friend class GLObjs;

private:
	Matrix<float> cross(const Matrix<float>& v1, const Matrix<float>& v2);
};

class GLObjs : protected GLObject
{
public:
	GLObjs(unsigned shader_program);
	unsigned indices(const std::vector<unsigned>& v, unsigned vbo=0);
	void operator()(int n);
	Matrix<float> operator[](int n);
	GLObjs& operator+=(GLObject& r);
	void transfer_all(const char* v_var, const char* c_var, const char* n_var, const char* t_var);

protected:
	unsigned shader_program_;
	std::vector<unsigned> index_chunks_;
	std::vector<Matrix<float>> matrixes_;
	std::vector<GLenum> modes_;
	unsigned vbo[5];

private:
	unsigned transfer_data(const std::vector<Matrix<float>>& v, const char* var,
			unsigned vbo=0);
};
