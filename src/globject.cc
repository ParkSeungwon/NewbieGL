#include<fstream>
#include<GL/glew.h>
#include<highgui.h>
#include"globj.h"
using namespace std;

GLObject::GLObject() : matrix_{4,4} {
	matrix_.E();
}
void GLObject::matrix(const Matrix<float>& m) { matrix_ = m; }
void GLObject::mode(GLenum md) { mode_ = md; }
void GLObject::vertexes(const vector<Matrix<float>>& v) { vertexes_ = v; }
void GLObject::vertexes(vector<Matrix<float>>&& v) { vertexes_ = move(v); }
void GLObject::colors(const vector<Matrix<float>>& v) { colors_ = v; }
void GLObject::colors(vector<Matrix<float>>&& v) { colors_ = move(v); }
void GLObject::indices(const vector<unsigned>& v) { indices_ = v; }
void GLObject::indices(vector<unsigned>&& v) { indices_ = move(v); }
void GLObject::texture_file(string f) { texture_file_ = f; }
void GLObject::subdiv_triangle()
{
	vector<Matrix<float>> v;
	vector<unsigned> ix;
	try {
		for(int i=0; i<indices_.size(); i+=3) {
			auto a = vertexes_[indices_[i]];
			auto b = vertexes_[indices_[i+1]];
			auto c = vertexes_[indices_[i+2]];
			v.push_back(a);
			v.push_back(b);
			v.push_back(c);
			v.push_back((a + b) * 0.5f);
			v.push_back((b + c) * 0.5f);
			v.push_back((c + a) * 0.5f);
			unsigned rel_pos[] = {0, 3, 5, 3, 1, 4, 3, 4, 5, 5, 4, 2};
			for(unsigned j : rel_pos) ix.push_back(i*2 + j);
		}
	} catch(const char* e) { cerr << e << endl; }
	vertexes_ = v;
	indices_ = ix;
	normals_.clear();
}

void GLObject::butterfly()
{
	vector<Matrix<float>> v;
	vector<unsigned> ix;
	try {
		for(int i=0; i<indices_.size(); i+=3) {
			auto a = vertexes_[indices_[i]];
			auto b = vertexes_[indices_[i+1]];
			auto c = vertexes_[indices_[i+2]];
			v.push_back(a);
			v.push_back(b);
			v.push_back(c);
			auto d1 = a - b;
			auto d2 = c - b;
			auto d3 = a - c;
			auto c1 = cross(a, b);
			auto c2 = cross(b, c);
			auto c3 = cross(c, a);
			v.push_back((a + b) * 0.5f + c1 * 0.1 * d1.distance());
			v.push_back((b + c) * 0.5f + c2 * 0.1 * d2.distance());
			v.push_back((c + a) * 0.5f + c3 * 0.1 * d3.distance());
			unsigned rel_pos[] = {0, 3, 5, 3, 1, 4, 3, 4, 5, 5, 4, 2};
			for(unsigned j : rel_pos) ix.push_back(i * 2 + j);
		}
	} catch(const char* e) { cerr << e << endl; }
	vertexes_ = v;
	indices_ = ix;
	normals_.clear();
}

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
		a = a * (1.0f / sqrt(a[1][1]*a[1][1] + a[1][2]*a[1][2] + a[1][3]*a[1][3]));
		a[1][4] = 1;
	}
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
	cout << file << "\'s indices size : " << indices_.size() << endl;
	return vertexes_.size();
}	


void GLObject::colors()
{
	if(texture_file_ == "") return;
	colors_.clear();
	normalize_vertex();
	for(int i=0; i<normals_.size(); i++) {
		float x = normals_[i][1][1];
		float y = normals_[i][1][2];
		float z = normals_[i][1][3];//find biggest abs->vertex coord
		float vx = vertexes_[i][1][1];
		float vy = vertexes_[i][1][2];
		float vz = vertexes_[i][1][3];

//		if(abs(x) > abs(y) && abs(x) > abs(z)) colors_.push_back({x>0?1:-1, vy, vz});
//		else if(abs(y)>abs(z) && abs(y)>abs(x)) colors_.push_back({vx, y>0?1:-1, vz});
//		else colors_.push_back({vx, vy, z>0?1:-1});

		if(abs(x) > abs(y) && abs(x) > abs(z)) //map to 육면체 전개도 
			colors_.push_back({x > 0 ? 0.5 - (vz + 1) / 8 : (vz + 1) / 8, 
					1.0f / 3 + (vy + 1) / 6, 0});
		else if(abs(y)>abs(z) && abs(y)>abs(x)) 
			colors_.push_back({0.25 + (vx + 1) / 8, 
					y > 0 ? (vz + 1) / 6 : 2.0f / 3 - (vz + 1) / 6, 0});
		else colors_.push_back({z > 0 ? 0.25 + (vx + 1) / 8 : 0.75 - (vx + 1) / 8,
				1.0f / 3 + (vy + 1) / 6, 0});
	}

	cout << "colors_ size : " << colors_.size() << endl;
	for(auto& a : colors_) {
		assert(a[1][1] >= -1 && a[1][1] <= 1);
		assert(a[1][2] >= -1 && a[1][2] <= 1);
		assert(a[1][3] >= -1 && a[1][3] <= 1);
	}
}

void GLObject::normalize_vertex()
{
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = xmax = vertexes_[0][1][1];
	ymin = ymax = vertexes_[0][1][2];
	zmin = zmax = vertexes_[0][1][3];
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
		for(int i=1; i<4; i++) {
			a[1][i] /= rate;
			a[1][i] -= 0.5;
			a[1][i] *= 2;
		}
	}
	for(auto& a : vertexes_) {
		assert(a[1][1] >= -1 && a[1][1] <= 1);
		assert(a[1][2] >= -1 && a[1][2] <= 1);
		assert(a[1][3] >= -1 && a[1][3] <= 1);
	}
}

