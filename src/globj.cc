#include<fstream>
#include<GL/glew.h>
#include<highgui.h>
#include"globj.h"
using namespace std;

GLObjs::GLObjs(unsigned prog) 
{
	shader_program_ = prog;
	glPolygonMode(GL_FRONT, GL_FILL);
}

GLObjs& GLObjs::operator+=(GLObject& r)
{
	r.normals();
	r.colors();//if texture
	if(r.indices_.empty()) for(int i=0; i<r.vertexes_.size(); i++) //if no indices
		r.indices_.push_back(i);
//	for(int i=0; i<10; i++) cout << r.vertexes_[i] << r.colors_[i] << r.normals_[i];
	auto sz = vertexes_.size();
	vertexes_.insert(vertexes_.end(), r.vertexes_.begin(), r.vertexes_.end());
	colors_.insert(colors_.end(), r.colors_.begin(), r.colors_.end());
	normals_.insert(normals_.end(), r.normals_.begin(), r.normals_.end());
	auto idx = r.indices_;
	for(auto& a : idx) a += sz;
	indices_.insert(indices_.end(), idx.begin(), idx.end());

	index_chunks_.push_back(r.indices_.size());
	modes_.push_back(r.mode_);
	matrixes_.push_back(r.matrix_);
	texture_files_.push_back(r.texture_file_);
}

void GLObjs::transfer_all()
{
	read_texture();
	vbo[0] = transfer_data(vertexes_, "vertexes_");
	vbo[1] = transfer_data(colors_, "colors_");
	vbo[2] = transfer_data(normals_, "normals_");
	vbo[3] = indices(indices_);
	cout << indices_.size() << endl;
}

unsigned GLObjs::read_texture()
{///cube map texture
	using namespace cv;
	int cubewall[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	unsigned vbo;
	glGenTextures(1, &vbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, vbo);
	for(int i=0; i<6; i++) {
		Mat image = imread("b.jpg");
//	int sq = min(image.cols/4, image.rows/3);
//	Rect 					r1(sq, 0, sq, sq), 
//		 r2(0, sq, sq, sq), r3(sq, sq, sq, sq), r4(2*sq,sq,sq,sq), r5(3*sq,sq,sq,sq), 
//		 					r6(sq, 2*sq, sq, sq);
	//glActiveTexture(GL_TEXTURE0);
		glTexImage2D(cubewall[i], 0, GL_RGB, image.cols, image.rows, 0, 
				GL_BGR, GL_UNSIGNED_BYTE, image.data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(shader_program_, "TEXTURE"), 0);
	return vbo;
}

unsigned GLObjs::indices(const vector<unsigned>& v, unsigned vbo)
{
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * v.size(), 
			v.data(), GL_STATIC_DRAW);
	return vbo;
}

Matrix<float> GLObjs::operator[](int n)
{
	return matrixes_[n];
}

void GLObjs::operator()(int n)
{
	unsigned offset = 0;
	for(int i=0; i<n; i++) offset += index_chunks_[i];
	glDrawElements(modes_[n], index_chunks_[n], GL_UNSIGNED_INT, 
			(void*)(offset * sizeof(unsigned)));
}

unsigned GLObjs::transfer_data(const vector<Matrix<float>>& v, const char* var,
		unsigned vbo) 
{
	int sz = v.size();
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float ar[sz * 3];
	for(int i=0; i<sz; i++) memcpy(ar + 3*i, v[i].data(), 3 * sizeof(float));
	glBufferData(GL_ARRAY_BUFFER, sizeof(ar), ar, GL_STATIC_DRAW);

	unsigned loc = glGetAttribLocation(shader_program_, var);
	glEnableVertexAttribArray(loc);
	cout << var << " : " << loc << ", " << v.size() <<endl;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return vbo;
}
