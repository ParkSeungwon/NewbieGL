#include"matrix.h"
using namespace std;

int main()
{
	Matrix<float> m3 = {
		{1, 2, 3},
		{1, 3, 2},
		{0, 1, 1}
	};
	cout << m3.I();
	cout << m3.M(2,3);
	cout << m3.M(3,3);
}



