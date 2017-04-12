#include"matrix.h"
using namespace std;

int main()
{
	Matrix<float> m3 = {
		{1, 2, 3},
		{1, 3, 2},
		{0, 1, 1}
	};
	cout << m3 << m3.det() << endl;
	cout << m3.I();
	cout << 1 / 0.3 << endl;
	for(int i=0; i<3; i++) for(int j=0; j<3; j++) cout << m3.M(i+1, j+1).det() << endl;
}



