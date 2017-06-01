#include<functional>
#include"matrix.h"
class Chunk
{
public:
	Chunk(int x, int y, int z);
	~Chunk();
	bool& at(int x, int y, int z);
	void subtract(std::function<bool(int,int,int)> f, 
			const Matrix<float>& m = Matrix<float>{4,4}.E());
	void show();
	std::vector<unsigned> indices();
	std::vector<Matrix<float>> vertexes_;
protected:
	int x_, y_, z_;
	
private:
	bool* mass;
	int f(int x, int y, int z) { return x + y * x_ + z * x_ * y_; }
};
