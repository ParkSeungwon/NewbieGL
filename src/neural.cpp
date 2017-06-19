#include<random>
#include"neural.h"
using namespace std;

int main()
{
	Neural<3, 3> equation;
	uniform_real_distribution<> di{1, 100};
	random_device rd;
	Matrix<float> m{1, 3};

	for(int i=0; i<5; i++) {
		for(auto& a : equation.layers) cout << a;
		for(auto& a : equation.hidden) cout << a;
		float b = 0, c = 1;
		while(b*b - 4*c < 0) b = di(rd), c = di(rd);
		m[1][1] = b, m[1][2] = c, m[1][3] = 1;
		equation.forward_feed(m);
		m[1][1] = (-b + sqrt(b*b - 4*c)) / 2;
		m[1][2] = (-b - sqrt(b*b - 4*c)) / 2;
		equation.back_propagation(m);
		equation.update_layer();
	}
	m[1][1] = 2, m[1][2] = 1;
	equation.forward_feed(m);
	cout << equation.hidden[2];
}

