#include<sstream>
#include<fstream>
#include<iostream>
#include<string>
using namespace std;

string read_file(const char* file)
{
	ifstream f(file);
	string r, s;
	while(f >> s) r += s + '\n';
	return r;
}

int main()
{
	cout << read_file("report.tex") << endl;
}



