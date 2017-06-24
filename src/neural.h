#include<vector>
#include"matrix.h"

template<int L, int H> class Neural
{
public:
	Neural();
	void forward_feed(const Matrix<float>& input);
	bool back_propagation(const Matrix<float>& target, float error = 0.1);
	void update_layer(float alpha = 0.01);
	std::vector<Matrix<float>> hidden;//hidden layer
	std::vector<Matrix<float>> layers;//weight & bias
};

