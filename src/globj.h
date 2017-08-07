#pragma once
#include<deque>
#include<vector>
#include"matrix.h"
class GLObject
{
public:
	GLObject();
	unsigned read_obj_file(std::string filename, bool vt = true, bool vn = true);

	//setters
	void mode(GLenum md);
	void matrix(const Matrix<float>& m);
	void vertexes(const std::vector<Matrix<float>>& verts);
	void vertexes(std::vector<Matrix<float>>&& verts);
	void colors(const std::vector<Matrix<float>>& cols);
	void colors(std::vector<Matrix<float>>&& cols);
	void indices(const std::vector<unsigned>& ids);
	void indices(std::vector<unsigned>&& ids);
	void texture_file(std::string filename);

protected:
	void normals();
	void colors();//for texture mapping if filename is specified
	std::vector<Matrix<float>> vertexes_, colors_, normals_;
	std::vector<unsigned> indices_;
	Matrix<float> matrix_;
	GLenum mode_ = GL_TRIANGLES;
	std::string texture_file_;
	friend class GLObjs;

private:
	Matrix<float> cross(const Matrix<float>& v1, const Matrix<float>& v2);
	void normalize_vertex();
};

class GLObjs : protected GLObject
{
public:
	GLObjs();
	void operator()(int n);
	Matrix<float> operator[](int n);
	GLObjs& operator+=(GLObject& r);
	unsigned transfer_all();
	void light(const Matrix<float>& light);//after transfer 
	void matrix(const Matrix<float>& r);
	unsigned transfer_data(const std::vector<Matrix<float>>& v, const char* var,
			unsigned vbo=0);
	unsigned update(std::vector<unsigned>&& v);

protected:
	unsigned indices(const std::vector<unsigned>& v, unsigned vbo=0);
	unsigned vbo[4];
	std::vector<unsigned> index_chunks_;
	unsigned shader_program_;
	std::vector<Matrix<float>> matrixes_;
	std::vector<GLenum> modes_;
	std::vector<std::string> texture_files_;

private:
	unsigned read_texture();
};
