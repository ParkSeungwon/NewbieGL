#include<fstream>
#include<GL/glew.h>
#include<highgui.h>
#include"globj.h"
#include"glutil.h"
using namespace std;

static string vshader = R"(
#version 130
uniform mat4 KeyBindMatrix;
in vec3 vertexes_;
in vec3 colors_;
in vec3 normals_;

out vec3 color;
out vec4 normal;
out vec3 vertex;

void main() {
//	gl_PointSize = 10;
	gl_Position = KeyBindMatrix * vec4(vertexes_, 1.0);
	normal = KeyBindMatrix * vec4(normals_, 0.0f);
	color = colors_;
	vertex = vertexes_;
}
)";

static string fshader1 = R"(
#version 130
uniform mat4 LIGHT;
uniform mat4 KeyBindMatrix;
uniform mat4 INFO;
)";
static string fshader2 = R"(
in vec3 color;
in vec4 normal;
in vec3 vertex;
out vec4 f_color;

vec3 ambient = LIGHT[0].xyz;
vec3 diffuse = LIGHT[1].xyz;
vec3 specular = LIGHT[2].xyz;
vec3 light_pos = LIGHT[3].xyz;
vec3 view = vec3(0,0,3);

void main() {
	vec4 lp = vec4(light_pos, 1);
//	lp = KeyBindMatrix * lp;
	vec3 N = normalize(normal.xyz);
	vec3 L = normalize(lp.xyz - vertex);
	vec3 V = normalize(view);
	vec3 R = -L + 2 * dot(N, L) * N;

	vec3 shade = ambient + diffuse*max(dot(L, N),0) + specular*pow(dot(V, R), 24);
	
	vec4 col;
	if(INFO[0].x < 0) col = vec4(color, 1.0);
)";
static string fshader3 = R"(
	f_color = vec4(col.rgb * shade, col.a);
}
)";

GLObjs::GLObjs() 
{
	glPolygonMode(GL_FRONT, GL_FILL);
}

GLObjs& GLObjs::operator+=(GLObject& r)
{
	if(r.indices_.empty()) for(int i=0; i<r.vertexes_.size(); i++) //if no indices
		r.indices_.push_back(i);
	r.normals();
	r.colors();//if texture, normalize vertex
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

void GLObjs::light(const Matrix<float>& light) 
{
	transfer_matrix(shader_program_, light.transpose(), "LIGHT");
}

void GLObjs::matrix(const Matrix<float>& m) 
{
	transfer_matrix(shader_program_, m, "KeyBindMatrix");
}

unsigned GLObjs::transfer_all()
{
	unsigned r = read_texture();
	vbo[0] = transfer_data(vertexes_, "vertexes_");
	vbo[1] = transfer_data(colors_, "colors_");
	vbo[2] = transfer_data(normals_, "normals_");
	vbo[3] = indices(indices_);
	cout << "indices size : " << indices_.size() << endl;
	light({//default light
		{0.1, 0.1, 0.1, 1}, //ambient
		{0.5, 0.5, 0.5, 0.5}, //diffuse
		{1, 1, 1, 1}, //specular
		{3, 3, -3, 1} //position 1 means a point 0 means a vector light
	});
	return r;
}

unsigned GLObjs::read_texture()
{///cube map texture
	using namespace cv;
	int n = texture_files_.size();
	unsigned vbo[n];
	glGenTextures(n, vbo);
	string s1, s2;
	for(int i=0; i<n; i++) if(texture_files_[i] != "") {//insert to shader program
		cout << texture_files_[i] << endl;
		s1 += "uniform sampler2D TEXTURE";
		s1 += to_string(i) + ";\n";
		s2 += "\telse if(INFO[0].x ==";
		s2 += to_string(i) + ") col = texture(TEXTURE" + to_string(i) + ", color.xy);\n";
	}
	
	string fshader = fshader1 + s1 + fshader2 + s2 + fshader3;
	cout << vshader << fshader;
	shader_program_ = make_shader_program(vshader, fshader);
	if(!shader_program_) return 0;
	glUseProgram(shader_program_);

	for(int i=0; i<texture_files_.size(); i++) { 
		if(texture_files_[i] == "") continue;
		Mat im = imread(texture_files_[i]);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, vbo[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.cols, im.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, im.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		char tex[] = "TEXTURE0"; tex[7] += i;
		glUniform1i(glGetUniformLocation(shader_program_, tex), i);
	}
	return shader_program_;
}

unsigned GLObjs::indices(const vector<unsigned>& v, unsigned vbo)
{
	if(!vbo) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * v.size(), 
			v.data(), GL_STATIC_DRAW);
	return vbo;
}

unsigned GLObjs::update(vector<unsigned>&& v)
{//only for michelangelo
	indices(v, vbo[3]);
	indices_ = move(v);
	index_chunks_[0] = indices_.size();
	for(auto& a : normals_) a = 0;
	int face;
	switch(modes_[0]) {
		case GL_TRIANGLES: face = 3; break;
		case GL_QUADS: face = 4; break;
		default: face = 3;
	}
	try{
		for(int i=0; i<indices_.size(); i+=face) {
			auto v1 = vertexes_[indices_[i+1]] - vertexes_[indices_[i]];
			auto v2 = vertexes_[indices_[i+2]] - vertexes_[indices_[i]];
			auto n = cross(v1, v2);
			for(int j=0; j<face; j++)
				normals_[indices_[i+j]] = normals_[indices_[i+j]] + n;
		}
	} catch(const char* e) { cerr << e << endl; }
	for(auto& a : normals_) {
		a = a * (1.0f / sqrt(a[1][1]*a[1][1] + a[1][2]*a[1][2] + a[1][3]*a[1][3]));
		a[1][4] = 1;
	}
	transfer_data(normals_, "normals_", vbo[2]);
}	

Matrix<float> GLObjs::operator[](int n)
{
	return matrixes_[n];
}

void GLObjs::operator()(int n)
{//draw nth object
	unsigned offset = 0;
	for(int i=0; i<n; i++) offset += index_chunks_[i];
//	glActiveTexture(GL_TEXTURE0 + n);//???
	Matrix<float> m{4,4};
	if(texture_files_[n] == "") m[1][1] = -1;
	else m[1][1] = n;
	transfer_matrix(shader_program_, m, "INFO");
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
