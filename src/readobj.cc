#include<fstream>
#include<limits>
#include"readobj.h"
using namespace std;

ObjReader::ObjReader(string file)
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
			vertexes.push_back(Matrix<float>{x,y,z});
		} else if(s == "f") {
			unsigned a, b; 
			char c;
			while(ss >> a >> c >> c >> b) indices.push_back(a-1);
		}
	}
	range[0] = range[1] = range[2] = numeric_limits<float>::max();
	range[3] = range[4] = range[5] = numeric_limits<float>::min();
	for(auto& a : vertexes) {
		for(int i=0; i<3; i++) if(a[1][i+1] < range[i]) range[i] = a[1][i+1];
		for(int i=0; i<3; i++) if(a[1][i+1] > range[i+3]) range[i+3] = a[1][i+1];
	}
	for(auto a : range) cout << a << ' ';
	cout << endl << vertexes.size() << ' ' << indices.size() << endl;
}
