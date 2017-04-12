#include<vector>
#include"matrix.h"

class ObjReader
{
public:
	ObjReader(std::string filename);
	std::vector<Matrix<float>> vertexes;
	std::vector<unsigned> indices;
	float range[6];//minx, miny, minz, maxx, maxy, maxz
};
