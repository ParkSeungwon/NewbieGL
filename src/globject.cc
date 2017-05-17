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
void GLObject::texture_file(string f) { file_name_ = f; }
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
	for(auto& a : normals_) {
		a /= sqrt(a[1][1]*a[1][1] + a[1][2]*a[1][2] + a[1][3]*a[1][3]);
		a[1][4] = 1;
	}
	normalize_vertex();
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


void GLObject::colors()
{
	if(file_name_ == "") return;
	colors_.clear();
	for(auto& a : normals_) {//-1 ~ 1 cube로 계산 뒤 0~1로 변형
		float x = a[1][1], y = a[1][2], z = a[1][3];//find biggest abs->vertex coord
		if(abs(x) > abs(y) && abs(x) > abs(z)) colors_.push_back({x>0?1:-1, y, z});
		else if(abs(y)>abs(z) && abs(y)>abs(x)) colors_.push_back({x, y>0?1:-1, z});
		else colors_.push_back({x, y, z>0?1:-1});
	}
}

void GLObject::normalize_vertex()
{
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = xmax = ymin = ymax = zmin = zmax = vertexes_[0][1][1];
	for(auto& a : vertexes_) {
		if(xmin > a[1][1]) xmin = a[1][1];
		if(xmax < a[1][1]) xmax = a[1][1];
		if(ymin > a[1][2]) ymin = a[1][2];
		if(ymax < a[1][2]) ymax = a[1][2];
		if(zmin > a[1][3]) zmin = a[1][3];
		if(zmax < a[1][3]) zmax = a[1][3];
	}
	float x = xmax - xmin;
	float y = ymax - ymin;
	float z = zmax - zmin;
	float rate = max(x, max(y,z));
	for(auto& a : vertexes_) {
		a[1][1] -= xmin;
		a[1][2] -= ymin;
		a[1][3] -= zmin;
	}
	for(auto& a : vertexes_) {
		a[1][1] /= rate;
		a[1][2] /= rate;
		a[1][3] /= rate;
	}
}

