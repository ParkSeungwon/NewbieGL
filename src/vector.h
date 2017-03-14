#pragma once

template<typename T> class Vec2D
{
public:
	Vec2D(T x, T y) : x_(x), y_(y) {}
	T x() const {return x_;}
	T y() const {return y_;}
	T operator*(const Vec2D<T>& r) {
		return x_*r.x_ + y_*r.y_;
	}

protected:
	T x_, y_;
};

template<typename T> class Vec3D : public Vec2D<T>
{
public:
	Vec3D(T x, T y, T z) : Vec2D<T>(x, y) { z_ = z;}
	T z() const {return z_;}
	T operator*(const Vec3D<T>& r) {//inner product of 2 vector
		return Vec2D<T>::operator*(r) + z_*r.z_;
	}
	operator Matrix<T>() {
		return Matrix<T>{{this->x()},{this->y()},{z()}};
	}
	Vec3D<T> operator^(const Vec3D<T>& r) {//counter product X
		return Vec3D<T> {
			this->y() * r.z() - z() * r.y(),
			this->x() * r.z() - z() * r.x(), 
			this->x() * r.y() - this->y() * r.x()
		};
	}

protected:
	T z_;
};

template<typename T> struct Vec4D
{
	union {
		struct {
			T x, y, z, w;
		};
		T v[4];
	};
 	Vec4D(T x, T y, T z, T w) {
 		this->x = x; this->y = y; this->z = z; this->w = w;
 	}
 	Vec4D(const Matrix<T>& r) {
 		memcpy(v, r.data(), 4 * sizeof(T));
 	}
		
	T operator*(const Vec4D<T>& r) {//dot product
		return x * r.x + y * r.y + z * r.z + w * r.w;
	}
	operator Matrix<T>() {
		return Matrix<T>{{x},{y},{z},{w}};
	}
};

template<typename T> std::ostream& operator<<(std::ostream& o, const Vec3D<T>& r) {
	o << '(' << r.x() << ',' << r.y() << ',' << r.z() << ')';
	return o;
}
template<typename T> std::ostream& operator<<(std::ostream& o, const Vec4D<T>& r) {
	o << '(' << r.x << ',' << r.y << ',' << r.z << ',' << r.w << ')';
	return o;
}
