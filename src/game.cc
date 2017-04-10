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

bool Block::rotate() 
{
	auto b = block;
	for(int i=1; i<5; i++) for(int j=1; j<5; j++) block[j][5-i] = b[i][j];
	return true;
}

bool Block::left()
{
	bool blank = true;
	for(int i=1; i<5; i++) if(block[1][i] != ' ') blank = false;
	if(blank) {
		for(int i=1; i<4; i++) for(int j=1; j<5; j++) block[i][j] = block[i+1][j];
		for(int i=1; i<5; i++) block[4][i] = ' ';
	}
	return blank;
}

bool Block::right()
{
	bool blank = true;
	for(int i=1; i<5; i++) if(block[4][i] != ' ') blank = false;
	if(blank) {
		for(int i=4; i>1; i--) for(int j=1; j<5; j++) block[i][j] = block[i-1][j];
		for(int i=1; i<5; i++) block[1][i] = ' ';
	}
	return blank;
}

bool Block::down()
{
	bool blank = true;
	for(int i=1; i<5; i++) if(block[i][4] != ' ') blank = false;
	if(blank) {
		for(int i=1; i<5; i++) for(int j=4; j>1; j--) block[i][j] = block[i][j-1];
		for(int i=1; i<5; i++) block[i][1] = ' ';
	}
	return blank;
}

bool Block::up()
{
	bool blank = true;
	for(int i=1; i<5; i++) if(block[i][1] != ' ') blank = false;
	if(blank) {
		for(int i=1; i<5; i++) for(int j=1; j<4; j++) block[i][j] = block[i][j+1];
		for(int i=1; i<5; i++) block[i][4] = ' ';
	}
	return blank;
}	

Game::Game(int w, int h) : board{w, h}
{
	width = w;
	height = h;
	x = w / 2 - 2; 
	y = h + 1;
	board = ' ';
	put_block();
}

void Game::remove_block() 
{
	for(int i=0; i<4; i++) for(int j=0; j<4; j++) 
		if(block.block[i+1][j+1] != ' ') board[x+i][y+j] = ' ';
}

void Game::put_block()
{
	for(int i=0; i<4; i++) for(int j=0; j<4; j++) 
		if(block.block[i+1][j+1] != ' ') board[x+i][y+j] = block.block[i+1][j+1];
}

void Game::left()
{
	remove_block();
	if(x > 1) {
		bool ok = overlap(x-1, y, block);
		if(!overlap(x-1, y, block)) x--;
	} else {
		if(block.left()) if(overlap(x, y, block)) block.right();
	}
	cout << x << ' ' << y << block.block;
	put_block();
}

void Game::right()
{
	remove_block();
	if(x < width - 3) {
		if(!overlap(x+1, y, block)) x++;
	} else {
		if(block.right()) if(overlap(x, y, block)) block.left();
	}
	put_block();
}

void Game::down()
{
	remove_block();
	if(y > 4) {
		if(!overlap(x, y-1, block)) y--;
	} else {
		if(!block.down() || overlap(x, y+1, block)) {//concrete old, gen new block
			put_block();
			x = width / 2 - 2; y = height-1;
			Block temp; block = temp;
		} 
	} 
	put_block();
}

void Game::rotate()
{
	remove_block();
	auto b = block;
	b.rotate();
	if(!overlap(x, y, b)) block.rotate();
	put_block();
}

bool Game::overlap(int x, int y, const Block& block)
{
	for(int i=0; i<4; i++) for(int j=0; j<4; j++) 
		if(board[x+i][y+j] != ' ' && block.block[i+1][j+1] != ' ') return true;
	return false;
}


