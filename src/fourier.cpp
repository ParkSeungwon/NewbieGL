#include<iostream>
#include"plot.h"
using namespace std;

int main()
{
	auto x = linspace(0, 2 * M_PI, 1000);
	valarray<float> y = sin(x) + sin(2.f * x) + sin(3.f * x);//auto -> bug, must define valarray
	plot(x, y);

	auto w = linspace(0, 2 * M_PI, 10000);
	auto Xw = DFT(y, x);
	plot(x, abs(Xw));

	for(int i=0; i<1000; i++) Xw[i] = IDFT(Xw, i);
	plot(x, Xw);

	valarray<complex<float>> yy(y.size());
	for(int i=0; i<y.size(); i++) yy[i] = {y[i],0};
	fft(yy);
	plot(x, yy);

	ifft(yy);
	plot(x, yy);
}


