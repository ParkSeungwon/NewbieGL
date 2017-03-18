#pragma once
#include<complex>
#include"matrix.h"

class Turtle : public std::complex<float>
{
public:
	Turtle(float x, float y, float degree);
	void turn(float degree);
	void go(float distance);

	float x(); float y(); float radian();

protected:
	float radian_;
};

