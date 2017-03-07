#include"matrix.h"
#include"vector.h"
using namespace std;

int main()
{
	Matrix<int> m = {{1,2,3},
					 {4,5,6},
					 {7,8,329}};
	Matrix<int> n{3,3};
	n[1][2] = 2;
	Vec3D<int> v{2,3,4};

	cout << m;
	cout << m * v;


	cout << (Matrix<int>)v;
}
