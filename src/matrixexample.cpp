#include"matrix.h"
using namespace std;

int main()
{
	Matrix<float> m1{4,4};
	Matrix<float> m2 = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	};
	m2[2][2] = 3;
	cout << m2;
	m1.glrotateX(M_PI/3);
	cout << m1;
	auto m3 = m1 * m2;
	cout << m3;
}



