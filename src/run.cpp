#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
#include"projectile.h"
#include"globj.h"
#define STEP 0.05
using namespace std;
extern float thz, dest_x, dest_y;
float x, y;//cur xy

deque<Projectile> bullets;
bool end_game = false;

void push()
{
	while(!end_game) {
		this_thread::sleep_for(200ms);
		bullets.push_back(Projectile{{x+0.2*cos(thz), y+0.2*sin(thz), 0}, {0,0,-1}});
		bullets.push_back(Projectile{{x-0.2*cos(thz), y-0.2*sin(thz), 0}, {0,0,-1}});
		while(!bullets.empty() && bullets.front().out_of_bound()) bullets.pop_front();
	}
}

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Space War", NULL, NULL);
	if (!glinit(window)) return -1;

	GLObject spaceship;
	unsigned sz = spaceship.read_obj_file("space_frigate_6.obj");
	spaceship.texture_file("google.jpg");
	Matrix<float> m{4,4};
	spaceship.matrix(m.gltranslate(0, 0.4, 0) * m.glrotateY(-M_PI/2) * m.glscale(0.6,0.6,0.6));

	GLObject ironman;
	sz = ironman.read_obj_file("ironman.obj");
	ironman.texture_file("google.jpg");
	ironman.matrix(m.glrotateX(-M_PI/2) * m.gltranslate(0.3,-0.2,0) * m.glscale(0.8,0.8,0.8));

	GLObject background;
	Matrix<float> ve[8] = {{0,-10,0}, {1,-10,0}, {1,1,0}, {0,1,0},
		{0,-10,1}, {1,-10,1}, {1,1,1}, {0,1,1}};
	vector<Matrix<float>> v;
	for(auto a : ve) v.push_back(a);
	background.vertexes(v);
	background.texture_file("1.jpg");
	background.indices({0,1,2,3, 7,6,5,4, 4,5,1,0, 5,6,2,1, 6,7,3,2, 7,4,0,3});
	background.mode(GL_QUADS);

	GLObject projectile;
	projectile.vertexes(v);
	projectile.indices({3,2,1,0, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 3,0,4,7});
	projectile.colors(vector<Matrix<float>>{24, {1,0,1}});
	projectile.matrix(m.glscale(0.05, 0.05, 0.05));
	projectile.mode(GL_QUADS);

	GLObjs objs;
	objs += spaceship;
	objs += ironman;
	objs += background;
	objs += projectile;
	objs.transfer_all();

	Matrix<float> proj{4,4};
	proj.glprojection(-1,1,-1,1,-11,1);
	thread th{push};

	while (!glfwWindowShouldClose(window)) {
		if(abs(dest_x - x) >= STEP || abs(dest_y - y) >= STEP) {
			complex<float> to{dest_x, dest_y};
			complex<float> cur{x, y};
			float th = arg(to - cur);
			x += STEP * cos(th);
			y += STEP * sin(th);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objs.matrix(proj * m.gltranslate(x, y, 0) * m.glrotateZ(thz) * objs[0]);
		objs(0);
//		objs.matrix(KeyBindMatrix * m.gltranslate(0,0.5,0.4) * objs[1]);
//		objs(1);
		objs.matrix(proj);
		objs(2);
		
		for(auto& a : bullets) {
			objs.matrix(proj * a.time_pass() * objs[3]);
			objs(3);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(50ms);
	}
	end_game = true;
	th.join();
	glfwTerminate();
}
