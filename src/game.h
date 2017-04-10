#include"matrix.h"

class Block
{
public:
	Block();
	void rotate(), left(), right();
	Matrix<char> block;

protected:
};

class Game
{
public:
	Game(int w, int h);
	Matrix<char> board;
	Block block;
	void left(), right(), down();
	int x, y;
};

