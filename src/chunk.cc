#include<iostream>
#include"chunk.h"
using namespace std;

Chunk::Chunk(int x, int y, int z) 
{
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	mass = new bool[x * y * z];
	for(int i=0; i<x*y*z; i++) mass[i] = true;
	vertexes_.resize(x_*y_*z_);
	for(int x=0; x<x_; x++) for(int y=0; y<y_; y++) for(int z=0; z<z_; z++) 
		vertexes_[f(x,y,z)] = Matrix<float>{x,y,z};
}

Chunk::~Chunk()
{
	delete mass;
}

bool& Chunk::at(int x, int y, int z)
{
	return mass[f(x,y,z)];
}

vector<unsigned> Chunk::indices()
{//inspect bool -> add index of the cube
	std::vector<unsigned> indices;
	const static int xx[] = {f(0,0,1), f(1,0,1), f(1,0,0), f(0,0,0),
					 		 f(0,1,1), f(1,1,1), f(1,1,0), f(0,1,0)};
	const static int idx[] = {3,2,1,0, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 3,0,4,7};
	for(int x=0; x<x_-1; x++) for(int y=0; y<y_-1; y++) for(int z=0; z<z_-1; z++)
		if(at(x,y,z)) for(int a : idx) indices.push_back(f(x,y,z) + xx[a]);
	sz_ = indices.size();
	return indices;
}

void Chunk::subtract(function<bool(int,int,int)> func, const Matrix<float>& tr)
{//get rid of a part which is inside of a tool function
	for(int x=0; x<x_; x++) for(int y=0; y<y_; y++) for(int z=0; z<z_; z++) {
		auto k = tr * Matrix<float>{x,y,z};
		if(func(k[1][1], k[1][2], k[1][3])) at(x,y,z) = false;
	}
}

void Chunk::show()
{
	for(int x=0; x<x_; x++) for(int y=0; y<y_; y++) for(int z=0; z<z_; z++)
		cout << at(x, y, z) << ' ';
}
