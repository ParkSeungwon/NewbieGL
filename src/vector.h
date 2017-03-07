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
		Matrix<T> m{1,3};
		m[1][1] = this->x();
		m[1][2] = this->y();
		m[1][3] = z();
		return m;
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
