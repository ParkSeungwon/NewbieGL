#include<iostream>
#include"spring.h"
using namespace std;

SpringModel::SpringModel(float damping, float x, float k, float m) {
	this->c = damping;
	this->k = k;
	this->m = m;
	this->x = x;
//	w = sqrt(4*m*k - c*c) / (2*m);//underdamping
}
float SpringModel::time_pass(float x0, float dt)
{
//	return x = exp(-c*t/2/m) * (c1 * cos(w*t) + c2 * sin(w*t));
	x = xp * dt + x;
	xp = (c*xp + k*(x - x0)) * dt / -m + xp;
	return x;
}

SpringModel3D::SpringModel3D() : x(*data()), y(*(data()+1)), z(*(data()+2)) { }

SpringModel3D& SpringModel3D::operator=(int n) { return *this; }

void  SpringModel3D::time_pass(float x0, float y0, float z0, float dt)
{
	x = xp * dt + x;
	y = yp * dt + y;
	z = zp * dt + z;
	xp = (c*xp + k*(x-x0))*dt/-m + xp;
	yp = (c*yp + k*(y-y0))*dt/-m + yp;
	zp = (x*zp + k*(z-z0))*dt/-m + zp;
}


SpringConnection::operator vector<Matrix<float>>()
{
	vector<Matrix<float>> v;
	for(int i=0; i<width; i++) for(int j=0; j<height; j++) 
		v.push_back((*this)[i+1][j+1]);
	return v;
}

SpringConnection::SpringConnection(int w, int h) : Matrix<SpringModel3D>{w, h}
{
	for(int i=0; i<w; i++) for(int j=0; j<h; j++) {
		(*this)[i+1][j+1].x = i * W;
		(*this)[i+1][j+1].x0 = i * W;
		(*this)[i+1][j+1].y = j * H;
		(*this)[i+1][j+1].y0 = j * H;
	}
	//indices.push_back(
}

void SpringConnection::time_pass() 
{
	for(int i=0; i<width; i++) for(int j=0; j<height; j++) {//position
		float x0 = 0, y0 = 0, z0 = 0;
		for(int m=-1; m<2; m++) for(int n=-1; n<2; n++) {//connect around
			if(i+1+m > 0 && j+1+n > 0 && i+1+m <= width && j+1+n <= height)
				if(m != 0 || n != 0) {//border check, not itself
					x0 += (*this)[i+1+m][j+1+n].x;
					y0 += (*this)[i+1+m][j+1+n].y;
					z0 += (*this)[i+1+m][j+1+n].z;
					(*this)[i+1][j+1].time_pass(x0, y0, z0);
				}
		}
	}
}



