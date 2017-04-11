#include<fstream>
#include<iostream>
#include"readobj.h"
using namespace std;

int main()
{
	ifstream f("a.txt");
	unsigned a, b;
	char c; 
	f >> a >> c >> c >> b;
	cout << a << ' ' << b;
}
