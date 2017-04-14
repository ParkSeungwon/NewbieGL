#include<fstream>
#include<GL/glew.h>
//#include<GLFW/glfw3.h>
#include"globj.h"
using namespace std;

GLObject::GLObject(unsigned prog) 
{
	shader_program_ = prog;
}

void GLObject::vertexes(const vector<Matrix<float>>& v, const char* n, unsigned vbo) 
{
	this->vbo[0] = transfer_data(v, n, vbo);
}

void GLObject::colors(const vector<Matrix<float>>& v, const char* n, unsigned vbo) 
{
	this->vbo[1] = transfer_data(v, n, vbo);
}

void GLObject::indices(const vector<unsigned>& v, unsigned vbo)
{
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * v.size(), 
			v.data(), GL_STATIC_DRAW);
	this->vbo[2] = vbo;
}

void GLObject::read_obj_file(string file, const char* var_name)
{
	vector<Matrix<float>> ver;
	vector<unsigned> ind;
	string s;
	ifstream f(file);
	while(getline(f, s)) {
		stringstream ss;
		ss << s;
		ss >> s;
		if(s == "v") {
			float x,  y, z;
			ss >> x >> y >> z;
			ver.push_back(Matrix<float>{x,y,z});
		} else if(s == "f") {
			unsigned a, b; 
			char c;
			while(ss >> a >> c >> c >> b) ind.push_back(a-1);
		}
	}
	vertexes(ver, var_name);
	indices(ind);
}	

unsigned GLObject::transfer_data(const vector<Matrix<float>>& v, const char* var,
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
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	return vbo;
}
