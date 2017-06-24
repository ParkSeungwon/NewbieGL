#include<random>
#include"plot.h"
#include"neural.h"
#define N 10000
using namespace std;

int main()
{
	Neural<5, 3> equation;
	uniform_real_distribution<> di{1, 100};
	random_device rd;
	Matrix<float> m{1, 3}, t{1, 3};
	float b = -3, c = 2;
	while(b*b - 4*c < 0) b = di(rd), c = di(rd);
	m[1][1] = b, m[1][2] = c, m[1][3] = 1;
	t[1][1] = (-b + sqrt(b*b - 4*c)) / 2;
	t[1][2] = (-b - sqrt(b*b - 4*c)) / 2;
	t[1][3] = 1;

	valarray<float> x = arange(0, 1, N);
	valarray<float> y(N), z(N);
	for(int i=0; i<N; i++) {
		equation.forward_feed(m);
		y[i] = equation.hidden[4][1][1];
		z[i] = equation.hidden[4][1][2];
		if(equation.back_propagation(t)) equation.update_layer(0.0003);
	}
	plot(x, y);
	plot(x, z);
	for(auto& a : equation.layers) cout << a;
	for(auto& a : equation.hidden) cout << a;

	equation.forward_feed(m);
	cout << equation.hidden[4] << endl;
	MatrixStream<float> in{m};
	MatrixStream<float> ms0{equation.layers[0]};
	MatrixStream<float> ms1{equation.layers[1]};
	MatrixStream<float> ms2{equation.layers[2]};
	MatrixStream<float> ms3{equation.layers[3]};
	MatrixStream<float> ms4{equation.layers[4]};
	MatrixStream<float> m0{equation.hidden[0]};
	MatrixStream<float> m1{equation.hidden[1]};
	MatrixStream<float> m2{equation.hidden[2]};
	MatrixStream<float> m3{equation.hidden[3]};
	MatrixStream<float> m4{equation.hidden[4]};
	const char cc[] = " = ";
	for(int i=0; i<3; i++) {
		cout << ms0 << in << cc[i];
		cout << ms1 << m0 << cc[i] << endl;
	}
	for(int i=0; i<3; i++) {
		cout << ms2 << m1 << cc[i];
		cout << ms3 << m2 << cc[i] << endl;
	}
	for(int i=0; i<3; i++) {
		cout << ms4 << m3 << cc[i] << m4 << endl;
	}
}

