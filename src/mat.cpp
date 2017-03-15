#include"rational.h"
#include"matrix.h"
using namespace std;

int main()
{
	Matrix<Ratio> m = {{7,3,2,1},
					   {3,2,1,1},
					   {4,1,3,7}, 
					   {2,6,9,8}};
	auto m2 = m.transpose();
	cout << m2;
	try {
		cout << m.inverse();
	} catch(const char* e) {
		cout << e << endl;
	}
}
