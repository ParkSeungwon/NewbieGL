#include<fstream>
#include<GL/glew.h>
#include<highgui.h>
#include"globj.h"
using namespace std;

GLObject::GLObject() : matrix_{4,4} {}
void GLObject::matrix(const Matrix<float>& m) { matrix_ = m; }
void GLObject::mode(GLenum md) { mode_ = md; }
void GLObject::vertexes(const vector<Matrix<float>>& v) { vertexes_ = v; }
void GLObject::vertexes(vector<Matrix<float>>&& v) { vertexes_ = move(v); }
void GLObject::colors(const vector<Matrix<float>>& v) { colors_ = v; }
void GLObject::colors(vector<Matrix<float>>&& v) { colors_ = move(v); }
void GLObject::indices(const vector<unsigned>& v) { indices_ = v; }
void GLObject::indices(vector<unsigned>&& v) { indices_ = move(v); }
void GLObject::normals()
{///should come after setting mode
	if(normals_.size() == vertexes_.size()) return;
	normals_.resize(vertexes_.size());
	int face;
	switch(mode_) {
		case GL_TRIANGLES: face = 3; break;
		case GL_QUADS: face = 4; break;
		default: face = 3;
	}
	try{
		for(int i=0; i<vertexes_.size(); i+=face) {
			auto v1 = vertexes_[i+1] - vertexes_[i];
			auto v2 = vertexes_[i+2] - vertexes_[i];
			auto n = cross(v1, v2);
			for(int j=0; j<face; j++) normals_[i+j] = normals_[i+j] + n;
		}
	} catch(const char* e) { cerr << e << endl; }
	//for(auto& a : normals_) a /= a[1][4];
}

Matrix<float> GLObject::cross(const Matrix<float>& v1, const Matrix<float>& v2)
{
	Matrix<float> m{v1[1][2] * v2[1][3] - v1[1][3] * v2[1][2],
					v1[1][3] * v2[1][1] - v1[1][1] * v2[1][3],
					v1[1][1] * v2[1][2] - v1[1][2] * v2[1][1]};
	float r = sqrt(m[1][1] * m[1][1] + m[1][2] * m[1][2] + m[1][3] * m[1][3]);
	m = m * (1.0f/r);
	m[1][4] = 1;
	return m;
}
							 
unsigned GLObject::read_obj_file(string file)
{
	int face = 0;
	string s;
	ifstream f(file);
	while(getline(f, s)) {
		stringstream ss{s};
		ss >> s;
		if(s == "v") {
			float x,  y, z;
			ss >> x >> y >> z;
			vertexes_.push_back(Matrix<float>{x,y,z});
		} else if(s == "f") {
			while(getline(ss, s, '/')) {
				indices_.push_back(stoi(s)-1);
				getline(ss, s, ' ');
				face++;
			}
			if(face == 3) mode(GL_TRIANGLES);
			else if(face == 4) mode(GL_QUADS);
		} else if(s == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			normals_.push_back(Matrix<float>{x, y, z});
		}
	}
	cout << file << indices_.size() << endl;
	return vertexes_.size();
}	

unsigned GLObject::read_texture(string file)
{///cube map texture
	using namespace cv;
	Mat image = imread(file);
	int sq = min(image.cols/4, image.rows/3);
	Rect 					r1(sq, 0, sq, sq), 
		 r2(0, sq, sq, sq), r3(sq, sq, sq, sq), r4(2*sq,sq,sq,sq), r5(3*sq,sq,sq,sq), 
		 					r6(sq, 2*sq, sq, sq);
	unsigned vbo;
	glGenTextures(1, &vbo);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, vbo);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, sq, sq, 0, GL_BGR, GL_UNSIGNED_BYTE, image(r1).data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, sq, sq, 0, GL_BGR, GL_UNSIGNED_BYTE, image(r2).data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, sq, sq, 0, GL_BGR, GL_UNSIGNED_BYTE, image(r3).data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, sq, sq, 0, GL_BGR, GL_UNSIGNED_BYTE, image(r4).data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, sq, sq, 0, GL_BGR, GL_UNSIGNED_BYTE, image(r5).data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, sq, sq, 0, GL_BGR, GL_UNSIGNED_BYTE, image(r6).data);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return vbo;
}

GLObjs::GLObjs(unsigned prog) 
{
	shader_program_ = prog;
	glPolygonMode(GL_FRONT, GL_FILL);
}

GLObjs& GLObjs::operator+=(GLObject& r)
{
	r.normals();
	auto sz = vertexes_.size();
	vertexes_.insert(vertexes_.end(), r.vertexes_.begin(), r.vertexes_.end());
	colors_.insert(colors_.end(), r.colors_.begin(), r.colors_.end());
	normals_.insert(normals_.end(), r.normals_.begin(), r.normals_.end());
	tex_uv_.insert(tex_uv_.end(), r.tex_uv_.begin(), r.tex_uv_.end());
	auto idx = r.indices_;
	for(auto& a : idx) a += sz;
	indices_.insert(indices_.end(), idx.begin(), idx.end());

	index_chunks_.push_back(r.indices_.size());
	modes_.push_back(r.mode_);
	matrixes_.push_back(r.matrix_);
}

void GLObjs::transfer_all(const char* v_var, const char* c_var, const char* n_var, const char* t_var)
{
	vbo[0] = transfer_data(vertexes_, v_var);
	vbo[1] = transfer_data(colors_, c_var);
	vbo[2] = transfer_data(normals_, n_var);
	vbo[3] = transfer_data(tex_uv_, t_var);
	vbo[4] = indices(indices_);
	cout << indices_.size() << endl;
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
	cout << var << " : " << loc << endl;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return vbo;
}
