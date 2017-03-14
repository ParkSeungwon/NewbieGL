#include"matrix.h"

Matrix<T> LxB(Matrix<T> L, Matrix<T> B) {
	int h = L.height;
	if(L.width != h || B.width != 1 || B.height != h) throw "type mismatch";
	T x[h+1];
	for(int i=1; i<=h; i++) {
		T sum = 0;
		for(int j=1; j<i; j++) sum += L[j][i] * x[1][j];
		x[i] = B[1][i] - sum;
	}

	Matrix<T> m{1, h};
	for(int i=1; i<=h; i++) m[1][i] = x[i];
	return m;
}

Matrix<T> UxB(Matrix<T> U, Matrix<T> B) {
	int h = U.height;
	if(U.width != h || B.width != 1 || B.height != h) throw "type mismatch";
	T x[h+1];
	for(int i=h; i>0; i--) {
		T sum = 0;
		for(int j=h; j>i; j--) sum += U[j][i] * x[1][j];
		x[i] = (B[1][i] - sum) / U[i][i]
	}

	Matrix<T> m{1, h};
	for(int i=1; i<=h; i++) m[1][i] = x[i];
	return m;
}
