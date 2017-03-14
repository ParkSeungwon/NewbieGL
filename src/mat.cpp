#include<sstream>
#include"matrix.h"
#include"vector.h"
using namespace std;

int main()
{
	Matrix<int> m = {{1,2,3},
					 {4,5,6},
					 {7,8,9}};
	m[3][2] = 11;
	Vec3D<int> v{2,3,4};
	auto m2 = m * v;
	MatrixStream<int> ms1{m}, ms2{v}, ms3{m2};//cannot use on the fly
	const char *c = " = ";
	for(int i=0; i<ms1.get_height(); i++) 
		cout << ms1 << ms2 << c[i] << ms3 << endl;

	Vec3D<int> v2{1,3,4};
	cout << v * v2 << endl;
	cout << (v ^ v2) << endl;
	
	Matrix<float> m6{{2.1},{1},{2}};
	Matrix<float> m5{{0.3,0.4, 1.123}};
	auto m9 = m5 * m6;
	MatrixStream<float> ms5{m5};
	MatrixStream<float> ms4{m9};
	MatrixStream<float> ms7{m6};

	cout << ms5.space() << ms7 << ' ' << endl;
	cout << ms5 << ms7 << '=' << ms4 << endl;
	cout << ms5.space() << ms7 << ' ' << endl;

	Matrix<double> mr = {{3.2,0,-4},
					 {4,6,6.1},
					 {3,8.9,0.9}};
	cout << mr.inverse();
}
