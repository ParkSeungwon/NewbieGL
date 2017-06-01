#include"matrix.h"
#include<random>

struct Particle 
{
	Particle(float x, float y, float z);
	void time_pass(float dt = 0.01);
	Matrix<float> pos, vel, accel;
};

struct Fountain 
{
	Fountain();
	std::normal_distribution<float> ini_velx{0, .05}, ini_vely{10, 1};
	std::random_device rd;
	std::vector<Particle> v;
	std::vector<Matrix<float>> pos;
	void time_pass(float dt = 0.01);
};

