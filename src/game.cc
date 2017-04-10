#include<random>
#include"game.h"
using namespace std;

Matrix<char> shapes[] = {
	{
		{' ', 'g', ' ', ' '},
		{' ', 'g', 'g', ' '},
		{' ', ' ', 'g', ' '},
		{' ', ' ', ' ', ' '}
	},
	{
		{' ', 'b', ' ', ' '},
		{' ', 'b', ' ', ' '},
		{' ', 'b', 'b', ' '},
		{' ', ' ', ' ', ' '}
	},
	{
		{' ', 'r', ' ', ' '},
		{' ', 'r', ' ', ' '},
		{' ', 'r', ' ', ' '},
		{' ', 'r', ' ', ' '}
	},
	{
		{' ', ' ', ' ', ' '},
		{' ', 'y', 'y', ' '},
		{' ', 'y', 'y', ' '},
		{' ', ' ', ' ', ' '}
	},
	{
		{' ', ' ', 'p', ' '},
		{' ', 'p', 'p', ' '},
		{' ', 'p', ' ', ' '},
		{' ', ' ', ' ', ' '}
	},
	{
		{' ', ' ', 't', ' '},
		{' ', ' ', 't', ' '},
		{' ', 't', 't', ' '},
		{' ', ' ', ' ', ' '}
	}
};
		
Block::Block() : block{4,4}
{
	uniform_int_distribution<> di{0,5};
	random_device rd;
	block = shapes[di(rd)];
}

void Block::rotate() 
{
	auto b = block;
	for(int i=1; i<5; i++) for(int j=1; j<5; j++) block[j][5-i] = b[i][j];
}
void Block::left()
{
	bool blank = true;
	for(int i=1; i<5; i++) if(block[1][i] != ' ') blank = false;
	if(blank) {
		for(int i=1; i<4; i++) for(int j=1; j<5; j++) block[i][j] = block[i+1][j];
		for(int i=1; i<5; i++) block[4][i] = ' ';
	}
}
void Block::right()
{
	bool blank = true;
	for(int i=1; i<5; i++) if(block[4][i] != ' ') blank = false;
	if(blank) {
		for(int i=4; i>1; i--) for(int j=1; j<5; j++) block[i][j] = block[i-1][j];
		for(int i=1; i<5; i++) block[1][i] = ' ';
	}
}

void Game::left() {}

void Game::left()
{
	if(x == 1) block.left();
	else 
	for(int i=0; i<4; i++) for(int j=0; j<4; j++) 
		if(block[i+1][j+1] != ' ' && board[x+i][y+j] != ' ') return false;
	for(int i=0; i<4; i++) for(int j=0; j<4; j++) board[x+i][y+j] = block[i+1][j+1];
}

void Game::remove_block()
