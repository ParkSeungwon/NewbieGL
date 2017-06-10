#include"matrix.h"
#include<vector>
#include<array>

template<int N> class Neural
{
public:
	Neural(const std::vector<Matrix<float>>& in,const std::vector<Matrix<float>>& out);
	Matrix<float> train_input, train_output;
	void forward_feed();
	void update_layer();
	void back_propagation();
	std::array<Matrix<float>, N> hidden;
	std::array<Matrix<float>, N> layers;

protected:
	std::vector<Matrix<float>> in, out;
	int height_;
	int layer_ = N;
	float alpha_ = 0.01;
};

