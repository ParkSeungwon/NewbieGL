#include"polygon.h"

Polygon::Polygon(float r, int points_count)
{
	Matrix<float> p{r, 0, 0};//when arg is 3 or 4, it makes 4x1 matrix r,0,0,1
	Matrix<float> rz{4, 4};//this makes 4x4 matrix
	rz.glrotateZ(2 * M_PI / points_count);
	for(int i=0; i<points_count; i++) {
		pts.push_back(p);
		p = rz * p;
	}
}

//std::vector<Matrix<float>>::iterator Polygon::begin() { return pts.begin(); }
//std::vector<Matrix<float>>::iterator Polygon::end() { return pts.end(); }
