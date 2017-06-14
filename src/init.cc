#include"glutil.h"
#include"globj.h"
using namespace std;

GLObjs objs;
void init_globjects()
{
	GLObject spaceship, background, projectile, ironman, buddha, monkey;

	spaceship.read_obj_file("space_frigate_6.obj");
	spaceship.texture_file("google.jpg");
	Matrix<float> m{4,4};
	spaceship.matrix(m.gltranslate(0,0,-6) * m.glrotateY(M_PI/2) * m.glscale(0.6,0.6,0.6) * m.gltranslate(0, 0.8, 0));
	
	ironman.read_obj_file("ironman.obj");
	ironman.texture_file("google.jpg");
	ironman.matrix(m.glrotateX(1.5 * M_PI) * m.glscale(0.8,0.8,0.8));

	Matrix<float> ve[8] = {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
		{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}};
	vector<Matrix<float>> v{ve, ve+8};
	background.vertexes(v);
	background.texture_file("1.jpg");
	background.indices({0,1,2,3, 7,6,5,4, 4,5,1,0, 5,6,2,1, 6,7,3,2, 7,4,0,3});
	background.matrix(m.gltranslate(0,0,-9.9) * m.glscale(1,1,5));
	background.mode(GL_QUADS);

	monkey.read_obj_file("monkey.obj");
	monkey.texture_file("brick.png");
	monkey.matrix(m.glscale(.5,.5,.5));

	projectile.vertexes(v);
	projectile.indices({3,2,1,0, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 3,0,4,7});
	projectile.colors(vector<Matrix<float>>{24, {1,0,1}});
	projectile.matrix(m.glscale(0.05, 0.05, 0.05));
	projectile.mode(GL_QUADS);

	buddha.read_obj_file("BuddhaSculpture.obj");
	buddha.texture_file("marble.jpg");
	buddha.matrix(m.glrotateY(0.5*M_PI)*m.glrotateX(-0.5*M_PI)*m.glscale(.7,.7,.7));

	objs += spaceship;
	objs += ironman;
	objs += buddha;
	objs += monkey;
	objs += background;
	objs += projectile;
	objs.transfer_all();
}
