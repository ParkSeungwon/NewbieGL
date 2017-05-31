#include<iostream>
#include"spring.h"
using namespace std;

int main()
{
	SpringModel sp{0.1};
	for(float t = 0; t < 30; t += 0.1) cout << sp.time_pass() << ' ';
}
