#include<chrono>
#include<thread>
#include<iostream>
#include"glutil.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main()
{
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(640, 480, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(10);

	vector<Matrix<float>> colors, vertexes, vtx;

	auto pl = polygon(4);
	vtx.insert(vtx.end(), begin(pl), end(pl));
	Matrix<float> m{4,4};
	pl = m.gltranslate(0,0,sqrt(2)) * pl;//z+1
	vtx.insert(vtx.end(), begin(pl), end(pl));//append elevated 4 vertex
	int idx[24] = {0,1,2,3, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 0,3,7,4};
	for(int a : idx) vertexes.push_back(vtx[a]);
	for(auto& a : vertexes) a = m.glrotateZ(M_PI/4) * a;

	Matrix<float> clr[] = {{1,0,0}, {0,1,0}, {0,0,1}, {1,1,0}, {1,0,1}, {0,1,1}};
	for(int i=0; i<6; i++) for(int j=0; j<4; j++) colors.push_back(clr[i]);

	unsigned fc = gl_transfer_data(colors);
	unsigned fv = gl_transfer_data(vertexes);
	
	const valarray<Matrix<float>> cube{vertexes.data(), vertexes.size()};
	valarray<Matrix<float>> v;
	float k = 0;

	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//공전하는 긴 막대
		v = KeyBindMatrix * m.glrotateX(k) * m.gltranslate(0,3,0) * 
			m.glscale(1,3,1) * cube;
		gl_transfer_data(v, fv);
		bindNdraw(fc, fv, GL_QUADS, 0, 24);
		//스스로 자전하는 큐브
		v = KeyBindMatrix * m.glrotateX(k) * m.gltranslate(0,8,0) *
			m.glrotateZ(k) * cube;
		gl_transfer_data(v, fv);
		bindNdraw(fc, fv, GL_QUADS, 0, 24);
		k += 0.1;//회전각도를 증가
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
