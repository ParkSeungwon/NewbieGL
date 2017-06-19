#include<vector>
#include"matrix.h"

template<int L, int H> class Neural
{
public:
	Neural();
	void forward_feed(const Matrix<float>& input);
	void back_propagation(const Matrix<float>& target);
	void update_layer();
	std::vector<Matrix<float>> hidden;//hidden layer
	std::vector<Matrix<float>> layers;//weight & bias

protected:
	float alpha_ = 0.001;
};

