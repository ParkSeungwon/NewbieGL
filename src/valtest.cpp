#include"glutil.h"
using namespace std;

const void* return_data(const Matrix<float>& m) {return m.data();}
float return_data(float f){return f;}
/*template <typename T> 
unsigned int transfer_data(const T& v, GLenum mode = GL_ARRAY_BUFFER)
{
	const int dim = 3;
	int sz = v.size();
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(mode, vbo);

	if(std::is_class<typename T::value_type>::value) {
		typename T::value_type ar[sz * dim];
		for(int i=0; i<sz; i++) memcpy(ar + dim * i, return_data(v[i]), sizeof(float) * dim);
		glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	} else {
		T ar[sz];
		for(int i=0; i<sz; i++) ar[i] = return_data(v[i]);
		glBufferData(mode, sizeof(ar), ar, GL_STATIC_DRAW);
	}

	return vbo;
}

template<typename T> bool is(T a) {return is_class<typename T::value_type>::value;}
*/
int main()
{
//	vector<int> v{0, 10};
//	transfer_data(v);
//	cout << is(v) << endl;
}
