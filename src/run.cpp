#include<chrono>
#include<thread>
#include<iostream>
#include<mutex>
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
mutex mtx1, mtx2;
bool end_game = false;

void generate_bullet()
{
	unique_lock<mutex> lock{mtx1, defer_lock};
	while(!end_game) {
		bullets.push_back(Projectile{{x+0.2*cos(thz), y+0.2*sin(thz), 4}, {0,0,-2}});
		bullets.push_back(Projectile{{x-0.2*cos(thz), y-0.2*sin(thz), 4}, {0,0,-2}});
		lock.lock();
		while(!bullets.empty() && bullets.front().out_of_bound()) bullets.pop_front();
		lock.unlock();
		this_thread::sleep_for(200ms);
	}
}

void generate_enemy()
{
	uniform_real_distribution<float> di{-1, 1};
	uniform_int_distribution<> di2{2, 6};
	random_device rd;
	Matrix<float> pos;
	unique_lock<mutex> lock{mtx2, defer_lock};
	while(!end_game) {//initial x,y,z position
		pos[1][1] = di(rd); 
		pos[1][2] = di(rd); 
		pos[1][3] = -14; 
		pos[1][4] = di2(rd);//buddha or ironman
		enemies.push_back(pos);
		lock.lock();
		while(!enemies.empty() && enemies.front()[1][3] > -5) enemies.pop_front();
		lock.unlock();
		this_thread::sleep_for(2s);
	}
}

void detect_crash()
{
	while(!end_game) {
		lock(mtx1, mtx2);
		for(auto& a : bullets) for(auto& b : enemies) {
			auto m = a.pos_ - b;
			if(m[1][1] * m[1][1] + m[1][2] * m[1][2] + m[1][3] * m[1][3] < 0.1) {
				a.pos_[1][3] = -100;
				b[1][3] = 100;
			}
		}		
		mtx1.unlock();
		mtx2.unlock();
		this_thread::sleep_for(50ms);
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
	unique_lock<mutex> lock1{mtx1, defer_lock}, lock2{mtx2, defer_lock};
	thread th1{generate_bullet}, th2{generate_enemy}, th3{detect_crash};

	while (!glfwWindowShouldClose(window)) {
		if(abs(dest_x - x) >= STEP || abs(dest_y - y) >= STEP) {//move ship x,y
			complex<float> to{dest_x, dest_y};
			complex<float> cur{x, y};
			float th = arg(to - cur);
			x += STEP * cos(th) / 3;
			y += STEP * sin(th) / 3;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objs.matrix(proj * m.gltranslate(x,y,0) * m.glrotateZ(thz) * objs[0]);
		objs(0);//spaceship
		lock2.lock();
		for(auto& a : enemies) {
			objs.matrix(proj * m.gltranslate(a[1][1],a[1][2],a[1][3]) * objs[a[1][4]]);
			objs(a[1][4]);//enemy
			a[1][3] += 0.05;//advance forward
		}
		lock2.unlock();
		objs.matrix(proj * objs[1]);
		objs(1);//background
		
		lock1.lock();
		for(auto& a : bullets) {
			objs.matrix(proj * a.time_pass() * objs[7]);
			objs(7);//bullet
		}
		lock1.unlock();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	end_game = true;
	th1.join();
	th2.join();
	th3.join();
	glfwTerminate();
}
