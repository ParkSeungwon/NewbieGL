#include"matrix.h"
#include<stdarg.h>
#include<algorithm>
using namespace std;
typedef float type;

void printm(const char* expr, ...) 
{
	va_list arp;
	va_start(arp, expr);//arp -> first
	vector<MatrixStream<type>> v;
	int i=0;
	for(int j=0; expr[j]; j++) {
		if(expr[j] == '%') {
			auto m = va_arg(arp, Matrix<type>);//arp = expr + sizeof(Matrix<T>)
			MatrixStream<type> ms{m};
			v.push_back(ms);
		} else i++;
	}
	int cc = i;//character count
	int mc = v.size();//matrix count
	unsigned short h[mc];//matrix height
	i = 0;
	for(auto& a : v) h[i++] = a.get_height();
	auto max = *max_element(h, h+mc);
	vector<string> vc{cc, string{max, ' '}};
	for(int i=0, j=0; expr[i]; i++) 
		if(expr[i] != '%') vc[j++][max/2+1] = expr[i];

	int m=0, n=0;
	for(int j=0; j<max; j++) {
		for(int k=0; expr[k]; k++) {
			if(expr[k] == '%') {
				if(j>=(max-v[m].get_height())/2 && j<(max+v[m].get_height())/2)
					cout << v[m];
				else cout << v[m].space();
				m++;
			} else cout << vc[n++][j];
		}
	}
	va_end(arp);
}

