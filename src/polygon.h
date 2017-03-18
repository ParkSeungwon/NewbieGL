#include<vector>
#include"matrix.h"

class Polygon
{
public:
	Polygon(float r=1, int points_count=100);
	std::vector<Matrix<float>>::iterator begin();	
	std::vector<Matrix<float>>::iterator end();

protected:
	std::vector<Matrix<float>> pts;
	friend weave(const Polygon& a, const Polygon& b);
};

template<typename T> 
std::vector<T> vweave(const std::vector<T>& a, const std::vector<T>& b)
{
	if(a.size() != b.size()) throw "size not match";
	std::vector<T> v;
	for(int i=0; i<a.size(); i++) {
		v.push_back(a[i]);
		v.push_back(b[i]);
	}
	return v;
}

std::vector<Matrix<float>> weave(const Polygon& a, const Polygon& b)
{
	return vweave(a.pts, b.pts);
}

template<typename T> std::vertex<Matrix<float>> weave(T begin, T end)
{//allign vertexes in order to make it a 3d object, polygons should be the same size
	std::vector<Matrix<float>> v;
	for(T it = begin; it+1 != end; it++) {
		auto w = weave(*it, *(it+1));
		v.insert(v.end(), w.begin(), w.end());
	}
	return v;
}


