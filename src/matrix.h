#pragma once
#include<cstring>
#include<sstream>
#include<cmath>
#include<cassert>
#include<iostream>
#include<iomanip>
#include<cxcore.hpp>
#include"combi.h"

template <typename T> class Matrix
{
public:
	Matrix(unsigned short w, unsigned short h) {
		width = w; height = h;
		arr = new T[h * w];
		for(int i=0; i<w * h; i++) arr[i] = 0;
	}
	
	Matrix<T>& operator=(T n) {
		for(int i=0; i<width*height; i++) arr[i] = n;
		return *this;
	}
	
	Matrix(T x, T y, T z, T w = 1) : Matrix{1,4} {
		arr[0] = x; arr[1] = y; arr[2] = z; arr[3] = w;
	}

	Matrix() : Matrix(0,0,0) {}

	Matrix(std::initializer_list<std::initializer_list<T>> li)
		: Matrix<T>{static_cast<unsigned short>(li.begin()->size()), 
					static_cast<unsigned short>(li.size())} {
		int x = 1, y = 1;
		for(auto& a : li) {
			for(auto& b : a) (*this)[x++][y] = b;
			y++; x = 1;
		}
	}
	
	Matrix(const Matrix<T>& r) : Matrix(r.width, r.height) {
		for(int i=0; i<width * height; i++) arr[i] = r.arr[i];
	}
	
	Matrix(Matrix<T>&& r) {
		arr = r.arr; r.arr = nullptr;
		width = r.width; height = r.height;
	}
	
	Matrix(const cv::Mat& m) : Matrix<T>{static_cast<unsigned short>(m.cols), 
		static_cast<unsigned short>(m.rows)} {
		for(int x=1; x<=width; x++) for(int y=1; y<=height; y++) 
			(*this)[x][y] = m.at<T>(y-1, x-1);
	}

	operator cv::Mat() {
		cv::Mat mat {cv::Mat_<T>{height, width}};
		for(int x=1; x<=width; x++) for(int y=1; y<=height; y++) 
			mat.at<T>(y-1, x-1) = (*this)[x][y];
		return mat;
	}

	virtual ~Matrix() {if(arr) delete [] arr;}

	///getters
	T* data() const {return arr;}
	unsigned short get_width() const{return width;}
	unsigned short get_height() const{return height;}

	///operator overloading
	T* operator[](int x) {//start from 11 21 31
		assert(x > 0);
		return arr + (x -1) * height - 1;
	}
	T* operator[](int x)const {//start from 11 21 31
		assert(x > 0);
		return arr + (x -1) * height - 1;
	}

	Matrix<T> operator+(const Matrix<T>& r) const {
		if(width != r.width || height != r.height) throw "Matrix size not match";
		Matrix<T> m(width, height);
		for(int i=0; i<width*height; i++) m.arr[i] = arr[i] + r.arr[i];
		return m;
	}

	Matrix<T> operator-(const Matrix<T>& r) const {
		if(width != r.width || height != r.height) throw "Matrix size not match";
		Matrix<T> m(width, height);
		for(int i=0; i<width*height; i++) m.arr[i] = arr[i] - r.arr[i];
		return m;
	}
	
	Matrix<T> operator*(const Matrix<T>& r) const {
		if(width != r.height) throw "Matrix size not match";
		Matrix<T> m(r.width, height);
		for(int x = 1; x <= r.width; x++) for(int y = 1; y <= height; y++)
			m[x][y] = inner_product(y, r.column(x));
		return m;
	}
	
	Matrix<T>& operator=(const Matrix<T>& r) {
		if(width != r.width || height != r.height) throw "Matrix size not match";
		for(int i=0; i<width*height; i++) arr[i] = r.arr[i];
		return *this;
	}
	
	Matrix<T>& operator*=(const Matrix<T>& r) {
		*this = *this * r;
		return *this;
	}

	Matrix<T> operator*(const T& r) const {return r * *this;}
	
	bool operator==(const Matrix<T>& r) const {
		if(width != r.width || height != r.height) return false;
		for(int i=0; i<width*height; i++) if(arr[i] != r.arr[i]) return false;
		return true;
	}

	friend Matrix<T> operator*(const T l, const Matrix<T>& r) {
		Matrix<T> m(r.width, r.height);
		for(int y=0; y<r.height; y++) for(int x=0; x<r.width; x++) 
			m.arr[y*r.width+x] = l * r.arr[y*r.width+x];
		return m;
	}

	Matrix<T> I() const {//Laplace way det
		T ad_bc = det();
		if(!ad_bc) throw "no inverse";
		Matrix<T> m{width, width};
		for(int i=1; i<=width; i++) for(int j=1; j<=width; j++) 
			m[i][j] = ((i+j) % 2 ? -1 : 1) * M(j, i).det() / ad_bc; //% > +
		return m;
	}

	Matrix<T> transpose() const{
		Matrix<T> m{height, width};
		for(int x=1; x<=width; x++) for(int y=1; y<=height; y++)
			m[y][x] = (*this)[x][y];
		return m;
	}

	Matrix<T> E() {
		if(width != height) throw "must be square matrix!";
		for(int x = 1; x <= width; x++) for(int y = 1; y <= height; y++) {
			if(x == y) (*this)[x][y] = 1;
			else (*this)[x][y] = 0;
		}
		return *this;
	}
	Matrix<T> gltranslate(T x, T y, T z) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[4][1] = x;
		(*this)[4][2] = y;
		(*this)[4][3] = z;
		return *this;
	}
	Matrix<T> glrotateZ(T th) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[1][1] = cos(th);
		(*this)[2][1] = -sin(th);
		(*this)[1][2] = sin(th);
		(*this)[2][2] = cos(th);
		return *this;
	}
	Matrix<T> glrotateX(T th) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[2][2] = cos(th);
		(*this)[3][2] = -sin(th);
		(*this)[2][3] = sin(th);
		(*this)[3][3] = cos(th);
		return *this;
	}
	Matrix<T> glrotateY(T th) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[1][1] = cos(th);
		(*this)[3][1] = -sin(th);
		(*this)[1][3] = sin(th);
		(*this)[3][3] = cos(th);
		return *this;
	}
	Matrix<T> glscale(T x, T y, T z) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[1][1] = x;
		(*this)[2][2] = y;
		(*this)[3][3] = z;
		return *this;
	}

	Matrix<T> glortho(T left, T right, T bottom, T top, T far, T near) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[1][1] = 2 / (right - left);
		(*this)[2][2] = 2 / (top - bottom);
		(*this)[3][3] = -2 / (far - near);
		(*this)[4][1] = -(left + right) / (right - left);
		(*this)[4][2] = -(top + bottom) / (top - bottom);
		(*this)[4][3] = -(far + near) / (far - near);
		return *this;
	}

	Matrix<T> glprojection(T left, T right, T bottom, T top, T far, T near) {
		if(width != 4 || height != 4) throw "should be 4x4";
		E();
		(*this)[1][1] = 2 * near / (right - left);
		(*this)[2][2] = 2 * near / (top - bottom);
		(*this)[3][3] = -(far + near) / (far - near);
		(*this)[3][1] = (left + right) / (right - left);
		(*this)[3][2] = (top + bottom) / (top - bottom);
		(*this)[4][3] = -2 * far * near / (far - near);
		(*this)[3][4] = -1;
		return *this;
	}
	Matrix<T> One() const {
		for(int i=0; i<width*height; i++)  arr[i] = 1;
	}
	Matrix<T> surround(T wall = 0) const {
		Matrix<T> m{width + 2, height + 2};
		for(int i=0; i<m.width*m.height; i++) m.arr[i] = wall;
		for(int x=1; x<=width; x++) for(int y=1; y<=height; y++) 
			m[x+1][y+1] = (*this)[x][y];
		return m;
	}
	
protected:
	T* arr;
	unsigned short width, height;

private:
	T* column(int x) const{
		return arr + (x - 1) * height;
	}

	T inner_product(int row, T* col) const{
		T sum = 0;
		for(int i=0; i<width; i++) sum += (*this)[i+1][row] * *(col+i);
		return sum;
	}

	Matrix<T> M(int x, int y) const {//x, y 열과 행을 제외한 행렬
		if(width != height) throw "should be square";
		Matrix<T> m{width-1, width-1};
		for(int i=1; i<x; i++) for(int j=1; j<y; j++) m[i][j] = (*this)[i][j];
		for(int i=x+1; i<=width; i++) for(int j=y+1; j<=width; j++) 
			m[i-1][j-1] = (*this)[i][j];
		for(int i=1; i<x; i++) for(int j=y+1; j<=width; j++) m[i][j-1] = (*this)[i][j];
		for(int i=x+1; i<=width; i++) for(int j=1; j<y; j++) m[i-1][j] = (*this)[i][j];
		return m;
	}

	T det() const {
		if(width == 1) return *data();
		T sum = 0;
		for(int i=1, j=1; i<=width; i++, j*=-1) 
			sum += j * (*this)[i][1] * M(i, 1).det();
		return sum;
	}

};

