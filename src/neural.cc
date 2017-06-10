#include"neural.h"
using namespace std;

template<int N>
Neural<N>::Neural(const vector<Matrix<float>>& in, const vector<Matrix<float>>& out)
{
	height_ = in[0].get_height();
	for(int i=0; i<layer_; i++) layers[i] = Matrix<float>{height_, height_};
	this->in = in; this->out = out;
}

template<int N> void Neural<N>::forward_feed()
{
	hidden[0] = layers[0] * in;
	for(int i=0; i<layer_-1; i++) hidden[i+1] = layers[i+1] * hidden[i];
}

template<int N> void Neural<N>::back_propagation()
{
	hidden[layer_-2] = hidden[layer_ - 2] - out;
	for(int i=layer_ - 2; i>0; i--) hidden[i-1] = layers[i].I() * hidden[i];
}

template<int N> void Neural<N>::update_layer()
{
	for(int k=layer_ - 2; k >=0; k--)
		for(int i=0; i<height_ ; i++) for(int j=0; j<height_; j++) 
			layers[k][i][j] -= alpha_ * hidden[1][i];
}
