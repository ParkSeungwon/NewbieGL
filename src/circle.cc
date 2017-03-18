#include"circle.h"

Circle::Circle(float* ctr, float r, int points_count) 
	: center{ctr[0], ctr[1], ctr[2]}, radius{r} 
{
	Matrix<float> p{r, 0, 0};
	Matrix<float> rz{4, 4}, tr{4, 4};
	rz.glrotateZ(2 * M_PI / points_count);
	tr.gltranslate(ctr[0], ctr[1], ctr[2]);
	for(int i=0; i<points_count; i++) {
		pts.push_back(p);
		p = rz * p;
	}
	for(auto& a : pts) a = tr * a;
}

std::vector<Matrix<float>>::iterator Circle::begin() { return pts.begin(); }
std::vector<Matrix<float>>::iterator Circle::end() { return pts.end(); }
float* Circle::ctr() {return center.data();}
