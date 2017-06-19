#include<random>
#include"neural.h"
using namespace std;

template<int L, int H> Neural<L, H>::Neural() 
	: layers(L, Matrix<float>{H,H}), hidden(L, Matrix<float>{1,H}) 
{
	uniform_real_distribution<float> di{0,1};
	random_device rd;
	for(int k=0; k<L; k++) for(int i=0; i<H; i++) for(int j=0; j<H; j++) 
		layers[k][i+1][j+1] = di(rd);
}

template<int L, int H> void Neural<L, H>::forward_feed(const Matrix<float>& in)
{
	hidden[0] = layers[0] * in;
	for(int i=0; i<L-1; i++) hidden[i+1] = layers[i+1] * hidden[i];
}

template<int L, int H> void Neural<L, H>::back_propagation(const Matrix<float>& out)
{
	hidden[L-1] = hidden[L-1] - out;
	for(int i = L-1; i>0; i--) hidden[i-1] = layers[i].transpose() * hidden[i];
}

template<int L, int H> void Neural<L, H>::update_layer()
{
	for(int k=L - 2; k >=0; k--)
		for(int i=0; i<H ; i++) for(int j=0; j<H; j++) 
			layers[k][i+1][j+1] -= alpha_ * hidden[k][1][i+1];
}

static void init()
{
	Neural<3,3> n;
	Matrix<float> m{2,2};
	n.forward_feed(m);
	n.back_propagation(m);
	n.update_layer();
}
