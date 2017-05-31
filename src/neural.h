#include"matrix.h"
#include<vector>
typedef Matrix<float> TV
class Neural
{
public:
};

class Particle
{
public:
	TV pos, vel;
};

class ParticleSystem
{
public:
	std::vector<Particle> particles;
	initParticles() {
		const int num_pts = 1000;
		particles.resize(num_pts);
		for(int p = 0; p < num_pts; p++) {
			TV& pos(particles[P].pos);
			pos.x_ = (float)rand() / (float)RAND_MAX;
			pos.y_ = (float)rand() / (float)RAND_MAX;
	}

};

glEnable(GL_VERTEX_PROGRAM_POINT_SIZE)

In the shader you just use gl_PointSize = psize;
