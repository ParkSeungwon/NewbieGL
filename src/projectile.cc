#include"projectile.h"
using namespace std;

Projectile::Projectile(Matrix<float> pos, Matrix<float> v)
{
	pos_ = pos;
	v_ = v;
}

Matrix<float> Projectile::time_pass(float dt)
{
	Matrix<float> m{4,4};
	pos_ = pos_ + dt * v_;
	return m.gltranslate(pos_[1][1], pos_[1][2], pos_[1][3]);
}

bool Projectile::out_of_bound()
{
	if(abs(pos_[1][3]) > 15) return true;
	return false;
}

