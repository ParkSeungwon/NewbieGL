#include<valarray>
#include"matrix.h"

class Polygon : public std::valarray<Matrix<float>>
{
public:
	Polygon(unsigned int points=3, float radius=1);
};
