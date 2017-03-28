#include<chrono>
#include<thread>
#include"glutil.h"
using namespace std;
extern Matrix<float> KeyBindMatrix;

int main()
{
	Matrix<float> translate{4,4};
	translate.gltranslate(0,0,sqrt(2));
	if (!glfwInit()) return -1;
	GLFWwindow* window = glfwCreateWindow(640, 480, "Color Cube", NULL, NULL);
	if (!glinit(window)) return -1;
	glortho(3);

	auto pl = polygon(4);
	vector<Matrix<float>> pl2;
	pl2.insert(pl2.end(), begin(pl), end(pl));
	pl = translate * pl;//z+1
	pl2.insert(pl2.end(), begin(pl), end(pl));//append elevated 4 vertex
	//for(auto& a : pl2) a = grotate * a;//rotate a little to have a good view
	valarray<float> color(72);//{}does not make 72 size - initialize_list construct
	color[slice(0,12,3)] = 1;
	color[slice(26,12,3)] = 1;
	color[slice(13,4,3)] = 1;
	color[slice(49,8,3)] = 1;

	int idx[24] = {0,1,2,3, 4,5,6,7, 0,1,5,4, 1,2,6,5, 2,3,7,6, 0,3,7,4};
	vector<Matrix<float>> v;
	for(auto& a : idx) v.push_back(pl2[a]);

	auto fc = gl_transfer_data(color);
	auto fv = gl_transfer_data(v);
	Matrix<float> con{4,4};
	const float r = 0.7;
	con = con.gltranslate(0,0,r) * con.glrotateY(0.1) * con.gltranslate(0,0,-r);
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for(auto& a : v) a = KeyBindMatrix * a;
		gl_transfer_data(v, fv);
		glBindBuffer(GL_ARRAY_BUFFER, fc);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, fv);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);//3 float is 1 vertex stride 0, 
		
		glDrawArrays(GL_QUADS, 0, 24);//mode, first, count

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	glfwTerminate();
}
