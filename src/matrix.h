#pragma once
#include<sstream>
#include<cmath>
#include<cassert>
#include<iostream>
#include<iomanip>

template <typename T> class Matrix
{
public:
	Matrix(size_t w, size_t h) {
		width = w;
		height = h;
		arr = new T[h * w];
		for(int i=0; i<w * h; i++) arr[i] = 0;
	}
	Matrix(std::initializer_list<std::initializer_list<T>> li)
		: Matrix<T>{li.begin()->size(), li.size()} {
		int x = 1, y = 1;
		for(auto& a : li) {
			for(auto& b : a) (*this)[x++][y] = b;
			y++; x = 1;
		}
	}

	T* operator[](int x) {//start from 11 21 31
		assert(x > 0);
		return arr + (x -1) * height - 1;
	}
	T* operator[](int x)const {//start from 11 21 31
		assert(x > 0);
		return arr + (x -1) * height - 1;
	}
	T* data() {return arr;}
	Matrix(const Matrix<T>& r) : Matrix(r.width, r.height) {
		for(int i=0; i<width * height; i++) arr[i] = r.arr[i];
	}
	Matrix(Matrix<T>&& r) {
		arr = r.arr;
		width = r.width;
		height = r.height;
		r.arr = nullptr;
	}
	virtual ~Matrix() {if(arr) delete [] arr;}
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
	bool operator==(const Matrix<T>& r) const;
	friend Matrix<T> operator*(const T l, const Matrix<T>& r) {
		Matrix<T> m(r.width, r.height);
		for(int y=0; y<r.height; y++) {
			for(int x=0; x<r.width; x++) {
				m.arr[y*r.width+x] = l * r.arr[y*r.width+x];
			}
		}
		return m;
	}
	int get_width() const{return width;}
	int get_height() const{return height;}
	
protected:
	T* arr;
	size_t width, height;

private:
	T* column(int x) const{
		return arr + (x - 1) * height;
	}
	T inner_product(int row, T* col) const{
		T sum = 0;
		for(int i=0; i<width; i++) sum += (*this)[i+1][row] * *(col+i);
		return sum;
	}
};

template <typename T> std::ostream& operator<<(std::ostream& o, const Matrix<T>& r){
	int w = r.get_width(), h = r.get_height();
	int gap[w+1] {0,};
	for(int y=1; y<=h; y++) for(int x=1; x<=w; x++) {
		std::stringstream ss;
		ss << r[x][y];
		int sz = ss.str().length();
		if(gap[x] < sz) gap[x] = sz;
	}
	o << "\u23a1" << ' ';
	for(int x=1; x<=w; x++) o << std::setw(gap[x]) << r[x][1] << ' ';
	o << "\u23a4" << std::endl;
	for(int y=2; y<h; y++) {
	o << "\u23a2" << ' ';
		for(int x=1; x<=w; x++) o << std::setw(gap[x]) << r[x][y] << ' ';
		o << "\u23a5" << std::endl;
	}
	o << "\u23a3" << ' ';
	for(int x=1; x<=w; x++) o << std::setw(gap[x]) << r[x][h] << ' ';
	o << "\u23a6" << std::endl;
	return o;
}
