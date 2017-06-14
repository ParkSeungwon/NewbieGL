#include<chrono>
#include<thread>
#include<iostream>
#include<random>
#include"glutil.h"
#include"projectile.h"
#include"globj.h"
#define STEP 0.05
using namespace std;
extern float thz, dest_x, dest_y, dest_z;
extern GLObjs objs;
float x, y;//cur xy

deque<Projectile> bullets;
deque<Matrix<float>> enemies;
bool end_game = false;

void push()
{
	while(!end_game) {
		bullets.push_back(Projectile{{x+0.2*cos(thz), y+0.2*sin(thz), 4}, {0,0,-2}});
		bullets.push_back(Projectile{{x-0.2*cos(thz), y-0.2*sin(thz), 4}, {0,0,-2}});
		while(!bullets.empty() && bullets.front().out_of_bound()) bullets.pop_front();
		this_thread::sleep_for(200ms);
	}
}

void enemy()
{
	uniform_real_distribution<float> di{-1, 1};
	random_device rd;
	while(!end_game) {
		enemies.push_back({di(rd), di(rd), -10});
		this_thread::sleep_for(1s);
	}
}

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Space War", NULL, NULL);
	if (!glinit(window)) return -1;
	init_globjects();

	Matrix<float> proj{4,4}, m{4,4};
	proj.glprojection(-1,1,-1,1,-5,5);
	thread th{push};

	while (!glfwWindowShouldClose(window)) {
		if(abs(dest_x - x) >= STEP || abs(dest_y - y) >= STEP) {
			complex<float> to{dest_x, dest_y};
			complex<float> cur{x, y};
			float th = arg(to - cur);
			x += STEP * cos(th) / 3;
			y += STEP * sin(th) / 3;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objs.matrix(proj * m.gltranslate(x,y,0) * m.glrotateZ(thz) * objs[0]);
		objs(0);//spaceship
//		objs.matrix(KeyBindMatrix * m.gltranslate(0,0.5,0.4) * objs[1]);
//		objs(1);
		objs.matrix(proj * objs[2]);
		objs(2);//background
		
		for(auto& a : bullets) {
			objs.matrix(proj * a.time_pass() * objs[3]);
			objs(3);//bullet
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
//		this_thread::sleep_for(50ms);
	}
	end_game = true;
	th.join();
	glfwTerminate();
}
