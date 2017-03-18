#include"turtle.h"
using namespace std;

Turtle::Turtle(float x, float y, float degree) : complex<float>{x, y}
{
	radian_ = degree * M_PI * 2 / 360.0;
}

void Turtle::turn(float degree)
{
	radian_ += degree * M_PI * 2 / 360.0;
}

void Turtle::go(float distance)
{
	*this += polar(distance, radian_);
}

float Turtle::x() { return real(); }
float Turtle::y() { return imag(); }
float Turtle::radian() { return radian_; }

