#pragma once
#include<deque>
#include<vector>
#include"matrix.h"

class GLObject
{
public:
	void read_obj_file(std::string filename);

	//setters
	void vertexes(const std::vector<Matrix<float>>& v, unsigned vbo=0);
	void colors(const std::vector<Matrix<float>>& v, unsigned vbo=0);
	void normals(const std::vector<Matrix<float>>& v, unsigned vbo=0);
	void indices(const std::vector<unsigned>& v, unsigned vbo=0);
	void vertex_shader(std::string vs);
	void fragment_shader(std::string fs);

protected:
//	std::vector<Matrix<float>> vertexes_;
//	std::vector<Matrix<float>> colors_;
//	std::vector<Matrix<float>> normals_;
//	std::vector<unsigned> indices_;
	std::deque<Matrix<float>> matrixes_;
	std::string vertex_shader_, fragment_shader_;

private:
	unsigned vbo[3];
	unsigned transfer_data(const std::vector<Matrix<float>>& v, unsigned vbo=0);
};
