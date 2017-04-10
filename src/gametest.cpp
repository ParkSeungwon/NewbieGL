#include"game.h"
using namespace std;

int main()
{
	Game g(6,6);
	cout << g.board;
	g.left();
	cout << g.board;
	g.left();
	cout << g.board;
	g.left();
	cout << g.board;
	g.left();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
	g.down();
	cout << g.board;
}

