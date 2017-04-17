#include<fstream>
#include<GL/glew.h>
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

unsigned GLObject::read_obj_file(string file)
{
	string s;
	ifstream f(file);
	while(getline(f, s)) {
		stringstream ss;
		ss << s;
		ss >> s;
		if(s == "v") {
			float x,  y, z;
			ss >> x >> y >> z;
			vertexes_.push_back(Matrix<float>{x,y,z});
		} else if(s == "f") {
			unsigned a, b; 
			char c;
			while(ss >> a >> c >> c >> b) indices_.push_back(a-1);
		}
	}
	cout << file << indices_.size() << endl;
	return vertexes_.size();
}	


GLObjs::GLObjs(unsigned prog) 
{
	shader_program_ = prog;
}

GLObjs& GLObjs::operator+=(const GLObject& r)
{
	auto sz = vertexes_.size();
	vertexes_.insert(vertexes_.end(), r.vertexes_.begin(), r.vertexes_.end());
	colors_.insert(colors_.end(), r.colors_.begin(), r.colors_.end());
	//normals_.insert(normals_.end(), r.normals_.begin(), r.normals_.end());
	unsigned sum = 0;
	for(auto a : index_chunks_) sum += a;
	auto idx = r.indices_;
	for(auto& a : idx) a += sz;
	indices_.insert(indices_.end(), idx.begin(), idx.end());

	index_chunks_.push_back(r.indices_.size());
	modes_.push_back(r.mode_);
	matrixes_.push_back(r.matrix_);
}

void GLObjs::transfer_all(const char* v_var, const char* c_var)
{
	vbo[0] = transfer_data(vertexes_, v_var);
	vbo[1] = transfer_data(colors_, c_var);
//	vbo[2] = transfer_data(normals_, n_var);
	vbo[2] = indices(indices_);
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
