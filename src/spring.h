#include<valarray>
#include<complex>
#include"matrix.h"

struct SpringModel
{//F = ma = m d2x/dt2 = -k(x-x0) - c dx/dt, x = position
// mx'' + cx' + k(x-x0) = 0
// x = e^at -> ma^2t^2 + cat + k = 0
	SpringModel(float damping = 0.5, float x = 0, float k = 1, float m = 1);
	float time_pass(float x0 = 0, float dt = 0.1);
	float m = 1;//mass
	float x0, x = 0;//position
	float c1 = 1, c2 = 1;//c1, c2 is determined by initial state :x(0), x'(0)
	float k;//spring constant
	float c;//damping constant
	float w;//T
	float xp = 0;//x'
};

struct SpringModel3D : public SpringModel, public Matrix<float>
{
	SpringModel3D();
	float y0=0, z0=0, yp=0, zp=0;
	void time_pass(float x0 = 0, float y0 = 0, float z0 = 0, float dt = 0.1);
	SpringModel3D& operator=(int n);
	float &x,&y,&z;
};

struct SpringConnection : public Matrix<SpringModel3D>
{
	SpringConnection(int w, int h);
	operator std::vector<Matrix<float>>();
	void time_pass(float dt);
	std::vector<unsigned> indices;
	
	const float W = 0.02;
	const float H = 0.04;
};
	
