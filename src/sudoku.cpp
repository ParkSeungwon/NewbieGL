#include<iostream>
#include"matrix.h"
using namespace std;

Matrix<unsigned> m = {
	{0,0,4,8,6,0,0,3,0},
	{0,0,1,0,0,0,0,9,0},
	{8,0,0,0,0,9,0,6,0},
	{5,0,0,2,0,6,0,0,1},
	{0,2,7,0,0,1,0,0,0},
	{0,0,0,0,4,3,0,0,6},
	{0,5,0,0,0,0,0,0,0},
	{0,0,9,0,0,0,4,0,0},
	{0,0,0,4,0,0,0,1,5}
 };

void sudoku(int x, int y)
{
	if(x == 10 && y == 9) cout << m;
	else if(x == 10) sudoku(1, y+1);
	else if(m[x][y]) sudoku(x+1, y);
	else {
		bool used[10] = {false,};
		for(int i=1; i<10; i++) used[m[i][y]] = used[m[x][i]] = true;
		int bx = ((x-1)/3 + 1) * 3 - 2;
		int by = ((y-1)/3 + 1) * 3 - 2;
		for(int i=bx; i<bx+3; i++) for(int j=by; j<by+3; j++) used[m[i][j]] = true;

		for(int i=1; i<10; i++) if(!used[i]) {
			m[x][y] = i;
			sudoku(x+1, y);
			m[x][y] = 0;
		}
	}
}

int main()
{
	sudoku(1, 1);
}
