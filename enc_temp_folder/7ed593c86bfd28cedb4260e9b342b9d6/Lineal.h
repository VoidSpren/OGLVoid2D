#pragma once

#define F_PI 3.1415927f
#define D_PI 3.141592653589793

#include <cmath>

namespace voi {

	void swap(int& n1, int& n2) { int t = n1; n1 = n2; n2 = t; };

	template<typename T>
	struct Vec2 {
		T x;
		T y;

		Vec2() : x(0), y(0) {}
		Vec2(T x, T y) : x(x), y(y) {}
		Vec2(const Vec2& other) : x(other.x), y(other.y) {}

		static T dotProd(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
		static Vec2 unit(const Vec2& in) {
			float l = sqrtf(in.x * in.x + in.y * in.y);
			return { in.x / l, in.y / l };
		}

		void toUnit() {
			float l = sqrtf(x * x + y * y);
			x / l; y / l;
		}

		Vec2 operator + (const Vec2& o) { return { x + o.x, y + o.y }; }
		Vec2 operator - (const Vec2& o) { return { x - o.x, y - o.y }; }
		Vec2 operator * (const Vec2& o) { return { x * o.x, y * o.y }; }
		Vec2 operator * (const T s) { return { x * s, y * s }; }
		Vec2 operator += (const Vec2& o) { x += o.x; y += o.y; return *this; }
		Vec2 operator -= (const Vec2& o) { x -= o.x; y -= o.y; return *this; }
		Vec2 operator *= (const Vec2& o) { x *= o.x; y *= o.y; return *this; }
		Vec2 operator *= (const T& s) { x *= s; y *= s; return *this; }

	};
	typedef Vec2<int> Vec2i;
	typedef Vec2<float> Vec2f;
	typedef Vec2<double> Vec2d;

	/* simple vector3 struct */
	template<typename T>
	struct Vec3 {
		T x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {}

		T vecMin() {
			if (x <= y && x <= z) return x;
			if (y <= x && y <= z) return y;
			if (z <= x && z <= y) return z;
		}
		T vecMax() {
			if (x >= y && x >= z) return x;
			if (y >= x && y >= z) return y;
			if (z >= x && z >= y) return z;
		}

		static T dotProd(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
		static Vec3 cross(const Vec3& a, const Vec3& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
		static Vec3 unit(const Vec3& in) {
			float l = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
			return { in.x / l, in.y / l,in.z / l };
		}

		void toUnit() {
			float l = sqrtf(x * x + y * y + z * z);
			x / l; y / l; z / l;
		}

		Vec3 operator + (const Vec3& o) { return { x + o.x, y + o.y, z + o.z }; }
		Vec3 operator - (const Vec3& o) { return { x - o.x, y - o.y, z - o.z }; }
		Vec3 operator * (const Vec3& o) { return { x * o.x, y * o.y, z * o.z }; }
		Vec3 operator * (const T s) { return { x * s, y * s, z * s }; }
		Vec3 operator += (const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
		Vec3 operator -= (const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
		Vec3 operator *= (const Vec3& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
		Vec3 operator *= (const T& s) { x *= s; y *= s; z *= s; return *this; }

	};
	typedef Vec3<int> Vec3i;
	typedef Vec3<float> Vec3f;
	typedef Vec3<double> Vec3d;

	/*simple vector4 struct*/
	template<typename T>
	struct Vec4 {
		union { T x, r, s; };
		union { T y, g, t; };
		union { T z, b, p; };
		union { T w, a, q; };

		Vec4() : x(0), y(0), z(0), w((T)1) {}
		Vec4(T w) : x(0), y(0), z(0), w(w) {}
		Vec4(T x, T y, T z) : x(x), y(y), z(z), w((T)1) {}
		Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vec4(const Vec4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

		/*the w component will be ignored in all vec to vec operations, it will only be used in vec to mat operations*/

		static T dotProd(const Vec4& a, const Vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
		static Vec4 cross(const Vec4& a, const Vec4& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
		static Vec4 unit(const Vec4& in) {
			float l = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
			if (l == 0) return { 0,0,0 };
			return { in.x / l, in.y / l,in.z / l };
		}

		void toUnit() {
			float l = sqrtf(x * x + y * y + z * z);
			if (l != 0) {
				x /= l; y /= l; z /= l;
			}
			else {
				x = 0; y = 0; z = 0;
			}
		}

		Vec4 operator + (const Vec4& o) { return { x + o.x, y + o.y, z + o.z }; }
		Vec4 operator - (const Vec4& o) { return { x - o.x, y - o.y, z - o.z }; }
		Vec4 operator * (const Vec4& o) { return { x * o.x, y * o.y, z * o.z }; }
		Vec4 operator * (const T s) { return { x * s, y * s, z * s }; }
		Vec4 operator / (const T s) { return { x / s, y / s, z / s }; }
		Vec4 operator += (const Vec4& o) { x += o.x; y += o.y; z += o.z; return *this; }
		Vec4 operator -= (const Vec4& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
		Vec4 operator *= (const Vec4& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
		Vec4 operator *= (const T s) { x *= s; y *= s; z *= s; return *this; }
		//TODO fix
		T& operator [] (const size_t i) {
			switch (i%4) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			}
		}
	};
	typedef Vec4<int> Vec4i;
	typedef Vec4<float> Vec4f;
	typedef Vec4<double> Vec4d;

	/*square mat4 struct*/
	template<typename T>
	struct Mat4 {
		Vec4<T> m[4] = { Vec4<T>(0) };

		void identity() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = 0;
				}
			}
			m[0][0] = (T)1;
			m[1][1] = (T)1;
			m[2][2] = (T)1;
			m[3][3] = (T)1;
		}

		Mat4 operator * (const Mat4& a) {
			Mat4 c;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					c.m[i][j] = m[i][0] * a.m[0][j] + m[i][1] * a.m[1][j] + m[i][2] * a.m[2][j] + m[i][3] * a.m[3][j];
				}
			}
			return c;
		}
		//T* operator [] (const size_t pos) { return m[pos]; }
		Vec4<T>& operator [] (const size_t pos) { return m[pos]; }
		Vec4<T> operator * (const Vec4<T>& vec) {
			return{
				vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0] + vec.w * m[3][0],
				vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1] + vec.w * m[3][1],
				vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2] + vec.w * m[3][2],
				vec.x * m[0][3] + vec.y * m[1][3] + vec.z * m[2][3] + vec.w * m[3][3]
			};
		}
	};
	typedef Mat4<int> Mat4i;
	typedef Mat4<float> Mat4f;
	typedef Mat4<double> Mat4d;

	template<typename T>
	Vec4<T> operator * (const Vec4<T>& vec, const Mat4<T>& mat) {
		return{
			vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0],
			vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1],
			vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2],
			vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3]
		};
	}

	/*makes the given matrix into a rotation matrix for the X axis*/
	void create_RotXMat(float theta, Mat4f& mat) {
		mat.identity();
		mat[0][0] = 1.0f;
		mat[1][1] = cosf(theta);
		mat[1][2] = sinf(theta);
		mat[2][1] = -sinf(theta);
		mat[2][2] = cosf(theta);
		mat[3][3] = 1.0f;
	}
	/*makes the given matrix into a rotation matrix for the Y axis*/
	void create_RotYMat(float theta, Mat4f& mat) {
		mat.identity();
		mat[0][0] = cosf(theta);
		mat[0][2] = -sinf(theta);
		mat[1][1] = 1.0f;
		mat[2][0] = sinf(theta);
		mat[2][2] = cosf(theta);
		mat[3][3] = 1.0f;
	}
	/*makes the given matrix into a rotation matrix for the Z axis*/
	void create_RotZMat(float theta, Mat4f& mat) {
		mat.identity();
		mat[0][0] = cosf(theta);
		mat[0][1] = sinf(theta);
		mat[1][0] = -sinf(theta);
		mat[1][1] = cosf(theta);
		mat[2][2] = 1.0f;
		mat[3][3] = 1.0f;
	}
}