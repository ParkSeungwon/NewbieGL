#include"game.h"
using namespace std;

int main()
{
	Block b;
	cout << b.block;
	b.right();
	cout << b.block;
	b.right();
	cout << b.block;
	b.left();
	cout << b.block;
	b.left();
	cout << b.block;
	b.left();
	cout << b.block;

}

