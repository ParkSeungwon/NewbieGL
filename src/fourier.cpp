#include"plot.h"
using namespace std;

int main()
{
	auto x = linspace(0, 2.*3.14, 1000);
	auto y = sin(3.14f*x);

	valarray<float> r(1000);
	auto f = linspace(0, 10, 1000);

	for(int i=0; i<1000; i++) r[i] = abs(DFT(y, f[i]));
	plot(f, r);
}


