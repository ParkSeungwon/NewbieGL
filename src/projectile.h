#include"matrix.h"

struct Projectile
{
	Projectile(Matrix<float> pos, Matrix<float> v);
	Matrix<float> time_pass(float dt = 0.1);
	bool out_of_bound();
	Matrix<float> v_, pos_;
};
