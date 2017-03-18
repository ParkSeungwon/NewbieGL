#include<vector>
#include"matrix.h"

class Circle
{
public:
	Circle(float* ctr, float r, int points_count = 100);
	std::vector<Matrix<float>>::iterator begin();	
	std::vector<Matrix<float>>::iterator end();
	float* ctr();

protected:
	Matrix<float> center;
	std::vector<Matrix<float>> pts;
	float radius;
};

