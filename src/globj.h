#pragma once
#include<deque>
#include<vector>
#include"matrix.h"

class GLObject
{
public:
	GLObject(unsigned shader_program);
	unsigned read_obj_file(std::string filename, const char* var_name_in_shader);
	void draw();

	//setters
	void vertexes(const std::vector<Matrix<float>>& verts, 
			const char* variable_name_in_shder, unsigned vbo=0);
	void colors(const std::vector<Matrix<float>>& cols, 
			const char* variable_name_in_shder, unsigned vbo=0);
	void normals(const std::vector<Matrix<float>>& norms, unsigned vbo=0);
	void indices(const std::vector<unsigned>& ids, unsigned vbo=0);
	void vertex_shader(std::string vs);
	void fragment_shader(std::string fs);

protected:
	unsigned shader_program_;
//	std::vector<Matrix<float>> vertexes_;
//	std::vector<Matrix<float>> colors_;
//	std::vector<Matrix<float>> normals_;
//	std::vector<unsigned> indices_;
	std::deque<Matrix<float>> matrixes_;
	std::string vertex_shader_, fragment_shader_;
	std::string ver_name, frag_name;

private:
	GLenum mode_ = GL_TRIANGLES;
	unsigned index_size_;
	unsigned vbo[3];
	unsigned transfer_data(const std::vector<Matrix<float>>& v, const char* var,
			unsigned vbo=0);
};
