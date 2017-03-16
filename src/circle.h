#include<vector>
#include"matrix.h"

class Circle
{
public:
	Circle(float* ctr, float r, int points_count = 100) 
		: center{ctr[0], ctr[1], ctr[2]}, radius{r} {
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

	auto begin() {
		return pts.begin();
	}
	auto end() {
		return pts.end();
	}
	float* ctr() {return center.data();}

protected:
	Matrix<float> center;
	std::vector<Matrix<float>> pts;
	float radius;
};

