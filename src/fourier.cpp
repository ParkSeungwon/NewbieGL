#include"plot.h"
using namespace std;

int main()
{
	auto x = linspace(0, 3.14, 100);
	valarray<float> y = sin(x) + sin(2.f*x) + sin(3.f*x);//auto -> bug, must define valarray

	int i=0;
	auto f = linspace(-1, 1, 1000);
	auto r = f;
	for(auto a : f) r[i++] = abs(DFT(y, a));
	plot(f, r);
}


