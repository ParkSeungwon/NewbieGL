#include"polygon.h"
using namespace std;

Polygon::Polygon(unsigned int pts, float r)
	: valarray<Matrix<float>>{Matrix<float>{0,0,0}, pts}
{
	Matrix<float> p{r, 0, 0};
	Matrix<float> rz{4, 4};
	rz.glrotateZ(M_PI * 2 / pts);
	for(int i=0; i<pts; i++) {
		(*this)[i] = p;
		p = rz * p;
	}
}



