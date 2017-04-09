#include"matrix.h"

class Tetris
{
public:
	Tetris(int w, int h);
	std::array<unsigned, 24> get_cube_element(int x, int y, int c);
	std::vector<Matrix<float>> vertexes, colors;

protected:
	Matrix<Matrix<float>> m0, m1;
	Matrix<float> color[6] = {{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1}};
	int width, height;
};


