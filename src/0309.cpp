#include"matrix.h"
#include"vector.h"
#include<cmath>
using namespace std;

int main()
{
	Vec4D<int> v1={1,2,3,4}, v2={2,3,4,5};//1.define vec4d
	cout << "dot product test : " << v1 * v2 << endl;//2.dot product

	Matrix<double> m = {//3.
		{0, 0, 0 ,0},
		{0, 0, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	Vec4D<double> v{1,0,0,0};
	cout << "3번 문제" << endl;
	int i = 0;
	for(double theta = M_PI / 6; theta < M_PI / 2 + 0.001; theta += M_PI / 12) {
		m[1][1] = cos(theta);
		m[1][2] = -sin(theta);
		m[2][1] = sin(theta);
		m[2][2] = cos(theta);
		cout << "thata is " << 30 + 15 * i++ << endl;
		cout << m * v << endl;
	}
	
	cout << "4번 문제" << endl;
	Matrix<double> m1{4,4}, m2{4,4};
	double theta = M_PI / 6;
	m1[1][1] = cos(theta);
	m1[1][2] = -sin(theta);
	m1[2][1] = sin(theta);
	m1[2][2] = cos(theta);
	m1[3][3] = 1;
	m1[4][4] = 1;
	theta = M_PI / 3;
	m2[1][1] = cos(theta);
	m2[1][2] = -sin(theta);
	m2[2][1] = sin(theta);
	m2[2][2] = cos(theta);
	m2[3][3] = 1;
	m2[4][4] = 1;
	cout << m1 * m2 * v << endl;//4.
}
