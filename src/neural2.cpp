#include<opencv2/ml/ml.hpp>
#include<random>
#include"matrix.h"
#define CASE 10000
using namespace std;
using namespace cv;

uniform_int_distribution<> di{-9, 9};
random_device rd;

Matrix<float> generate_input(int n)
{
	Matrix<float> v{3, n};
	for(int i=0; i<n; i++) {
		int a=1, b=0, c=1;
		while(b*b - 4*a*c < 0 || !a) a = di(rd), b = di(rd), c = di(rd);
		v[1][i+1] = a, v[2][i+1] = b, v[3][i+1] = c;
	}
	return v;
}

Matrix<float> generate_output(const Matrix<float>& v)
{
	int h = v.get_height();
	Matrix<float> o{2, h};
	for(int i=1; i<h+1; i++) {
		float a = v[1][i], b = v[2][i], c = v[3][i];
		o[1][i] = (-b + sqrt(b*b - 4*a*c)) / 2 / a;
		o[2][i] = (-b - sqrt(b*b - 4*a*c)) / 2 / a;
	}
	return o;
}

int main()
{
	Matrix<int> layerSize = {{3, 5, 7, 7, 7, 7, 7, 5, 5, 2}}; 
	Matrix<float> input = generate_input(CASE);//{{1, 2, 1}, {1, -3, 2}, {2, -1, -1}};
	cout << input;
	Matrix<float> output = generate_output(input);//{{-1, -1}, {1, 2}, {1, -0.5}};
	cout << output;
	Matrix<float> sample{CASE, 1};// = {{1, 1, 1}};
	for(int i=0; i<CASE; i++) sample[i+1][1] = .001;
	Matrix<float> test = {{1, 2, 1}};
	Mat result;
	CvANN_MLP nn{layerSize};
	nn.train(input, output, sample);
	nn.predict(test, result);
	cout << result;
}

