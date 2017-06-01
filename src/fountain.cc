#include<random>
#include"fountain.h"
using namespace std;

Particle::Particle(float x, float y, float z) : vel{x, y, z}, accel{0, -9.8, 0}
{}

void Particle::time_pass(float dt)
{
	vel[1][2] -= -9.8 * dt;//y velocity
	pos = pos + vel * dt;//position
}

Fountain::Fountain()
{
	for(int i=0; i<1000; i++) 
		v.push_back(Particle{ini_velx(rd), ini_vely(rd), ini_velx(rd)});
}

void Fountain::time_pass(float dt)
{
	pos.clear();
	pos.resize(1000);
	for(auto& a : v) {
		a.time_pass(dt);
		pos.push_back(a.pos);
	}
}
