#include<iostream>
#include"plot.h"
using namespace std;

int main()
{
	auto x = linspace(0, 2 * 3.14, 100);
	valarray<float> y = sin(x) + sin(2.f*x) + sin(3.f*x);//auto -> bug, must define valarray

	auto w = linspace(-3, 3, 500);
	plot(w, abs(DFT(y, w)));
}


