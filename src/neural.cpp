#include<random>
#include"plot.h"
#include"neural.h"
#define N 1000
using namespace std;

int main()
{
	Neural<3, 3> equation;
	uniform_real_distribution<> di{1, 100};
	random_device rd;
	Matrix<float> m{1, 3};

	valarray<float> x = arange(0, 1, N);
	valarray<float> y(N), z(N);
	for(int i=0; i<N; i++) {
//		for(auto& a : equation.layers) cout << a;
//		for(auto& a : equation.hidden) cout << a;
		float b = -8, c = 6;
		while(b*b - 4*c < 0) b = di(rd), c = di(rd);
		m[1][1] = b, m[1][2] = c, m[1][3] = 1;
		equation.forward_feed(m);
		y[i] = equation.hidden[2][1][1];
		z[i] = equation.hidden[2][1][2];
		m[1][1] = (-b + sqrt(b*b - 4*c)) / 2;
		m[1][2] = (-b - sqrt(b*b - 4*c)) / 2;
		equation.back_propagation(m);
		equation.update_layer();
	}
	plot(x, y);
	plot(x, z);
}

