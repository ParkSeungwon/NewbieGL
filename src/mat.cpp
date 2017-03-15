#include"rational.h"
#include"irrational.h"
#include"matrix.h"
using namespace std;

int main()
{
	Matrix<Ratio> m = {{7,3},
					 {2,6}};
	try {
		cout << m.inverse();
	} catch(const char* e) {
		cout << e << endl;
	}
}
