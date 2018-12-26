#ifndef YASP_SWIZZLE_MATH_H
#define YASP_SWIZZLE_MATH_H

#include <ostream>
#include <cmath>
#include <limits>

namespace yasp
{

	template<class T>
	constexpr T PI = T(3.1415926535897932385L);

	template <typename T>
	class vec2
	{
	public:
		template<unsigned a, unsigned b>
		class swizzle
		{
		public:
			T v[2];
		public:
			operator vec2() { return { v[a], v[b] }; }
			operator vec2() const { return { v[a], v[b] }; }

			constexpr vec2 operator+(const vec2& rhs) const { return { v[a] + rhs.x, v[b] + rhs.y }; }
			constexpr vec2 operator-(const vec2& rhs) const { return { v[a] - rhs.x, v[b] - rhs.y }; }
			constexpr vec2 operator*(const vec2& rhs) const { return { v[a] * rhs.x, v[b] * rhs.y }; }
			constexpr vec2 operator/(const vec2& rhs) const { return { v[a] / rhs.x, v[b] / rhs.y }; }

			constexpr vec2 operator+(const float rhs) const { return { v[a] + rhs, v[b] + rhs }; }
			constexpr vec2 operator-(const float rhs) const { return { v[a] - rhs, v[b] - rhs }; }
			constexpr vec2 operator*(const float rhs) const { return { v[a] * rhs, v[b] * rhs }; }
			constexpr vec2 operator/(const float rhs) const { return { v[a] / rhs, v[b] / rhs }; }

			constexpr vec2 operator-() const { return { -v[a],-v[b] }; }

			//If a compile error points you here it is because you have tried to do something like f2.xx = ff2.xy; i.e. tried to assign when two of the swizzles on the left are the same.
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator=(const vec2& rhs) { v[a] = rhs.x; v[b] = rhs.y; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator+=(const vec2& rhs) { v[a] += rhs.x; v[b] += rhs.y; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator-=(const vec2& rhs) { v[a] -= rhs.x; v[b] -= rhs.y; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator*=(const vec2& rhs) { v[a] *= rhs.x; v[b] *= rhs.y; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator/=(const vec2& rhs) { v[a] /= rhs.x; v[b] /= rhs.y; return *this; }

			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator=(const float rhs) { v[a] = rhs; v[b] = rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator+=(const float rhs) { v[a] += rhs; v[b] += rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator-=(const float rhs) { v[a] -= rhs; v[b] -= rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator*=(const float rhs) { v[a] *= rhs; v[b] *= rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b, vec2>::type>
			Dummy operator/=(const float rhs) { v[a] /= rhs; v[b] /= rhs; return *this; }
		};

	public:
		union
		{
			struct
			{
				T x, y;
			};
			swizzle<0, 0> xx;
			swizzle<0, 1> xy;
			swizzle<1, 0> yx;
			swizzle<1, 1> yy;
			T v[2];
		};

		constexpr vec2() {}
		constexpr vec2(T x) : x(x), y(x) {}
		constexpr vec2(T x, T y) : x(x), y(y) {};
		constexpr vec2 operator+(const vec2& rhs) const { return { x + rhs.x, y + rhs.y }; }
		constexpr vec2 operator-(const vec2& rhs) const { return { x - rhs.x, y - rhs.y }; }
		constexpr vec2 operator*(const vec2& rhs) const { return { x * rhs.x, y * rhs.y }; }
		constexpr vec2 operator/(const vec2& rhs) const { return { x / rhs.x, y / rhs.y }; }
		constexpr vec2 operator+(const float rhs) const { return { x + rhs, y + rhs }; }
		constexpr vec2 operator-(const float rhs) const { return { x - rhs, y - rhs }; }
		constexpr vec2 operator*(const float rhs) const { return { x * rhs, y * rhs }; }
		constexpr vec2 operator/(const float rhs) const { return { x / rhs, y / rhs }; }

		constexpr vec2 operator-() const { return { -x, -y }; }

		vec2& operator+=(const vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
		vec2& operator-=(const vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		vec2& operator*=(const vec2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
		vec2& operator/=(const vec2& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
		vec2& operator+(const float rhs) { x += rhs; y += rhs; return *this; }
		vec2& operator-(const float rhs) { x -= rhs; y -= rhs; return *this; }
		vec2& operator*(const float rhs) { x *= rhs; y *= rhs; return *this; }
		vec2& operator/(const float rhs) { x /= rhs; y /= rhs; return *this; }
		friend std::ostream& operator<<(std::ostream& stream, const vec2& f2) { stream << f2.x << ", " << f2.y; return stream; }
	};

	template<typename T>
	constexpr vec2<T> operator*(float l, const vec2<T>& right) { return right * l; }


	template <typename T>
	class vec3
	{
	public:
		template<unsigned a, unsigned b, unsigned c>
		class swizzle
		{
		private:
			T v[3];
		public:
			operator vec3() { return { v[a], v[b], v[c] }; }
			operator vec3() const { return { v[a], v[b], v[c] }; }

			constexpr vec3 operator+(const vec3& rhs) const { return { v[a] + rhs.x, v[b] + rhs.y, v[c] + rhs.z }; }
			constexpr vec3 operator-(const vec3& rhs) const { return { v[a] - rhs.x, v[b] - rhs.y, v[c] - rhs.z }; }
			constexpr vec3 operator*(const vec3& rhs) const { return { v[a] * rhs.x, v[b] * rhs.y, v[c] * rhs.z }; }
			constexpr vec3 operator/(const vec3& rhs) const { return { v[a] / rhs.x, v[b] / rhs.y, v[c] / rhs.z }; }

			constexpr vec3 operator+(const float rhs) const { return { v[a] + rhs, v[b] + rhs, v[c] + rhs }; }
			constexpr vec3 operator-(const float rhs) const { return { v[a] - rhs, v[b] - rhs, v[c] - rhs }; }
			constexpr vec3 operator*(const float rhs) const { return { v[a] * rhs, v[b] * rhs, v[c] * rhs }; }
			constexpr vec3 operator/(const float rhs) const { return { v[a] / rhs, v[b] / rhs, v[c] / rhs }; }

			vec3 operator-() const { return { -v[a],-v[b],-v[c] }; }

			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator=(const float rhs) { v[a] = rhs; v[b] = rhs; v[c] = rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator+=(const float rhs) { v[a] += rhs; v[b] += rhs; v[c] += rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator-=(const float rhs) { v[a] -= rhs; v[b] -= rhs; v[c] -= rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator*=(const float rhs) { v[a] *= rhs; v[b] *= rhs; v[c] *= rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator/=(const float rhs) { v[a] /= rhs; v[b] /= rhs; v[c] /= rhs; return *this; }

			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator=(const vec3& rhs) { v[a] = rhs.x; v[b] = rhs.y; v[c] = rhs.z; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator+=(const vec3& rhs) { v[a] += rhs.x; v[b] += rhs.y; v[c] += rhs.z; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator-=(const vec3& rhs) { v[a] -= rhs.x; v[b] -= rhs.y; v[c] -= rhs.z; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator*=(const vec3& rhs) { v[a] *= rhs.x; v[b] *= rhs.y; v[c] *= rhs.z; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && b != c, vec3>::type>
			Dummy operator/=(const vec3& rhs) { v[a] /= rhs.x; v[b] /= rhs.y; v[c] /= rhs.z; return *this; }
		};

	public:
		union
		{
			struct
			{
				T x, y, z;
			};

			vec2<T>::swizzle<0, 0> xx;
			vec2<T>::swizzle<0, 1> xy;
			vec2<T>::swizzle<0, 2> xz;
			vec2<T>::swizzle<1, 0> yx;
			vec2<T>::swizzle<1, 1> yy;
			vec2<T>::swizzle<1, 2> yz;
			vec2<T>::swizzle<2, 0> zx;
			vec2<T>::swizzle<2, 1> zy;
			vec2<T>::swizzle<2, 2> zz;
			swizzle<0, 0, 0> xxx;
			swizzle<0, 0, 1> xxy;
			swizzle<0, 0, 2> xxz;
			swizzle<0, 1, 0> xyx;
			swizzle<0, 1, 1> xyy;
			swizzle<0, 1, 2> xyz;
			swizzle<0, 2, 0> xzx;
			swizzle<0, 2, 1> xzy;
			swizzle<0, 2, 2> xzz;
			swizzle<1, 0, 0> yxx;
			swizzle<1, 0, 1> yxy;
			swizzle<1, 0, 2> yxz;
			swizzle<1, 1, 0> yyx;
			swizzle<1, 1, 1> yyy;
			swizzle<1, 1, 2> yyz;
			swizzle<1, 2, 0> yzx;
			swizzle<1, 2, 1> yzy;
			swizzle<1, 2, 2> yzz;
			swizzle<2, 0, 0> zxx;
			swizzle<2, 0, 1> zxy;
			swizzle<2, 0, 2> zxz;
			swizzle<2, 1, 0> zyx;
			swizzle<2, 1, 1> zyy;
			swizzle<2, 1, 2> zyz;
			swizzle<2, 2, 0> zzx;
			swizzle<2, 2, 1> zzy;
			swizzle<2, 2, 2> zzz;
			T v[3];
		};
		constexpr vec3() {}
		constexpr vec3(T x) : x(x), y(x), z(x) {}
		constexpr vec3(T x, T y, T z) : x(x), y(y), z(z) {}

		constexpr vec3 operator+(const vec3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
		constexpr vec3 operator-(const vec3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		constexpr vec3 operator*(const vec3& rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		constexpr vec3 operator/(const vec3& rhs) const { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
		constexpr vec3 operator+(const T rhs) const { return { x + rhs, y + rhs, z + rhs }; }
		constexpr vec3 operator-(const T rhs) const { return { x - rhs, y - rhs, z - rhs }; }
		constexpr vec3 operator*(const T rhs) const { return { x * rhs, y * rhs, z * rhs }; }
		constexpr vec3 operator/(const T rhs) const { return { x / rhs, y / rhs, z / rhs }; }

		vec3& operator+=(const vec3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z;  return *this; }
		vec3& operator-=(const vec3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z;  return *this; }
		vec3& operator*=(const vec3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z;  return *this; }
		vec3& operator/=(const vec3& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z;  return *this; }
		vec3& operator+=(const T rhs) { x += rhs; y += rhs; z += rhs; return *this; }
		vec3& operator-=(const T rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
		vec3& operator*=(const T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
		vec3& operator/=(const T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

		vec3 operator-() const { return { -x,-y,-z }; }

		friend std::ostream& operator<<(std::ostream& stream, const vec3& f3) { stream << f3.x << ", " << f3.y << ", " << f3.z; return stream; }
	};

	template<typename T>
	constexpr vec3<T> operator*(float l, const vec3<T>& right) { return right * l; }

	template <typename T>
	class vec4
	{
	private:
		template<unsigned a, unsigned b, unsigned c, unsigned d>
		class swizzle
		{
		private:
			T v[4];
		public:
			operator vec4() { return { v[a], v[b], v[c], v[d] }; }
			operator vec4() const { return { v[a], v[b], v[c], v[d] }; }

			/*constexpr vec4 operator+(const vec4& rhs) const { return { v[a] + rhs.x, v[b] + rhs.y, v[c] + rhs.z, v[d] + rhs.w; }; }
			constexpr vec4 operator-(const vec4& rhs) const { return { v[a] - rhs.x, v[b] - rhs.y, v[c] - rhs.z, v[d] - rhs.w; }; }
			constexpr vec4 operator*(const vec4& rhs) const { return { v[a] * rhs.x, v[b] * rhs.y, v[c] * rhs.z, v[d] * rhs.w; }; }
			constexpr vec4 operator/(const vec4& rhs) const { return { v[a] / rhs.x, v[b] / rhs.y, v[c] / rhs.z, v[d] / rhs.w; }; }

			constexpr vec4 operator+(const T rhs) const { return { v[a] + rhs, v[b] + rhs, v[c] + rhs, v[d] + rhs; }; }
			constexpr vec4 operator-(const T rhs) const { return { v[a] - rhs, v[b] - rhs, v[c] - rhs, v[d] - rhs; }; }
			constexpr vec4 operator*(const T rhs) const { return { v[a] * rhs, v[b] * rhs, v[c] * rhs, v[d] * rhs; }; }
			constexpr vec4 operator/(const T rhs) const { return { v[a] / rhs, v[b] / rhs, v[c] / rhs, v[d] / rhs; }; }*/

			constexpr vec4 operator-() const { return { -v[a],-v[b],-v[c],-v[d] }; }

			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator+=(const T rhs) { v[a] += rhs; v[b] += rhs; v[c] += rhs; v[d] += rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator-=(const T rhs) { v[a] -= rhs; v[b] -= rhs; v[c] -= rhs; v[d] -= rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator*=(const T rhs) { v[a] *= rhs; v[b] *= rhs; v[c] *= rhs; v[d] *= rhs; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator/=(const T rhs) { v[a] /= rhs; v[b] /= rhs; v[c] /= rhs; v[d] /= rhs; return *this; }

			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator=(const vec4& rhs) { v[a] = rhs.x; v[b] = rhs.y; v[c] = rhs.z; v[d] = rhs.w; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator+=(const vec4& rhs) { v[a] += rhs.x; v[b] += rhs.y; v[c] += rhs.z; v[d] += rhs.w; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator-=(const vec4& rhs) { v[a] -= rhs.x; v[b] -= rhs.y; v[c] -= rhs.z; v[d] -= rhs.w; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator*=(const vec4& rhs) { v[a] *= rhs.x; v[b] *= rhs.y; v[c] *= rhs.z; v[d] *= rhs.w; return *this; }
			template<typename Dummy = std::enable_if<a != b && a != c && a != d && b != c && b != d && c != d, vec4>::type>
			Dummy operator/=(const vec4& rhs) { v[a] /= rhs.x; v[b] /= rhs.y; v[c] /= rhs.z; v[d] /= rhs.w; return *this; }
		};

	public:
		union
		{
			struct
			{
				T x, y, z, w;
			};
			vec2<T>::swizzle<0, 0> xx;
			vec2<T>::swizzle<0, 1> xy;
			vec2<T>::swizzle<0, 2> xz;
			vec2<T>::swizzle<0, 3> xw;
			vec2<T>::swizzle<1, 0> yx;
			vec2<T>::swizzle<1, 1> yy;
			vec2<T>::swizzle<1, 2> yz;
			vec2<T>::swizzle<1, 3> yw;
			vec2<T>::swizzle<2, 0> zx;
			vec2<T>::swizzle<2, 1> zy;
			vec2<T>::swizzle<2, 2> zz;
			vec2<T>::swizzle<2, 3> zw;
			vec2<T>::swizzle<3, 0> wx;
			vec2<T>::swizzle<3, 1> wy;
			vec2<T>::swizzle<3, 2> wz;
			vec2<T>::swizzle<3, 3> ww;

			vec3<T>::swizzle<0, 0, 0> xxx;
			vec3<T>::swizzle<0, 0, 1> xxy;
			vec3<T>::swizzle<0, 0, 2> xxz;
			vec3<T>::swizzle<0, 0, 3> xxw;
			vec3<T>::swizzle<0, 1, 0> xyx;
			vec3<T>::swizzle<0, 1, 1> xyy;
			vec3<T>::swizzle<0, 1, 2> xyz;
			vec3<T>::swizzle<0, 1, 3> xyw;
			vec3<T>::swizzle<0, 2, 0> xzx;
			vec3<T>::swizzle<0, 2, 1> xzy;
			vec3<T>::swizzle<0, 2, 2> xzz;
			vec3<T>::swizzle<0, 2, 3> xzw;
			vec3<T>::swizzle<0, 3, 0> xwx;
			vec3<T>::swizzle<0, 3, 1> xwy;
			vec3<T>::swizzle<0, 3, 2> xwz;
			vec3<T>::swizzle<0, 3, 3> xww;
			vec3<T>::swizzle<1, 0, 0> yxx;
			vec3<T>::swizzle<1, 0, 1> yxy;
			vec3<T>::swizzle<1, 0, 2> yxz;
			vec3<T>::swizzle<1, 0, 3> yxw;
			vec3<T>::swizzle<1, 1, 0> yyx;
			vec3<T>::swizzle<1, 1, 1> yyy;
			vec3<T>::swizzle<1, 1, 2> yyz;
			vec3<T>::swizzle<1, 1, 3> yyw;
			vec3<T>::swizzle<1, 2, 0> yzx;
			vec3<T>::swizzle<1, 2, 1> yzy;
			vec3<T>::swizzle<1, 2, 2> yzz;
			vec3<T>::swizzle<1, 2, 3> yzw;
			vec3<T>::swizzle<1, 3, 0> ywx;
			vec3<T>::swizzle<1, 3, 1> ywy;
			vec3<T>::swizzle<1, 3, 2> ywz;
			vec3<T>::swizzle<1, 3, 3> yww;
			vec3<T>::swizzle<2, 0, 0> zxx;
			vec3<T>::swizzle<2, 0, 1> zxy;
			vec3<T>::swizzle<2, 0, 2> zxz;
			vec3<T>::swizzle<2, 0, 3> zxw;
			vec3<T>::swizzle<2, 1, 0> zyx;
			vec3<T>::swizzle<2, 1, 1> zyy;
			vec3<T>::swizzle<2, 1, 2> zyz;
			vec3<T>::swizzle<2, 1, 3> zyw;
			vec3<T>::swizzle<2, 2, 0> zzx;
			vec3<T>::swizzle<2, 2, 1> zzy;
			vec3<T>::swizzle<2, 2, 2> zzz;
			vec3<T>::swizzle<2, 2, 3> zzw;
			vec3<T>::swizzle<2, 3, 0> zwx;
			vec3<T>::swizzle<2, 3, 1> zwy;
			vec3<T>::swizzle<2, 3, 2> zwz;
			vec3<T>::swizzle<2, 3, 3> zww;
			vec3<T>::swizzle<3, 0, 0> wxx;
			vec3<T>::swizzle<3, 0, 1> wxy;
			vec3<T>::swizzle<3, 0, 2> wxz;
			vec3<T>::swizzle<3, 0, 3> wxw;
			vec3<T>::swizzle<3, 1, 0> wyx;
			vec3<T>::swizzle<3, 1, 1> wyy;
			vec3<T>::swizzle<3, 1, 2> wyz;
			vec3<T>::swizzle<3, 1, 3> wyw;
			vec3<T>::swizzle<3, 2, 0> wzx;
			vec3<T>::swizzle<3, 2, 1> wzy;
			vec3<T>::swizzle<3, 2, 2> wzz;
			vec3<T>::swizzle<3, 2, 3> wzw;
			vec3<T>::swizzle<3, 3, 0> wwx;
			vec3<T>::swizzle<3, 3, 1> wwy;
			vec3<T>::swizzle<3, 3, 2> wwz;
			vec3<T>::swizzle<3, 3, 3> www;

			swizzle<0, 0, 0, 0> xxxx;
			swizzle<0, 0, 0, 1> xxxy;
			swizzle<0, 0, 0, 2> xxxz;
			swizzle<0, 0, 0, 3> xxxw;
			swizzle<0, 0, 1, 0> xxyx;
			swizzle<0, 0, 1, 1> xxyy;
			swizzle<0, 0, 1, 2> xxyz;
			swizzle<0, 0, 1, 3> xxyw;
			swizzle<0, 0, 2, 0> xxzx;
			swizzle<0, 0, 2, 1> xxzy;
			swizzle<0, 0, 2, 2> xxzz;
			swizzle<0, 0, 2, 3> xxzw;
			swizzle<0, 0, 3, 0> xxwx;
			swizzle<0, 0, 3, 1> xxwy;
			swizzle<0, 0, 3, 2> xxwz;
			swizzle<0, 0, 3, 3> xxww;
			swizzle<0, 1, 0, 0> xyxx;
			swizzle<0, 1, 0, 1> xyxy;
			swizzle<0, 1, 0, 2> xyxz;
			swizzle<0, 1, 0, 3> xyxw;
			swizzle<0, 1, 1, 0> xyyx;
			swizzle<0, 1, 1, 1> xyyy;
			swizzle<0, 1, 1, 2> xyyz;
			swizzle<0, 1, 1, 3> xyyw;
			swizzle<0, 1, 2, 0> xyzx;
			swizzle<0, 1, 2, 1> xyzy;
			swizzle<0, 1, 2, 2> xyzz;
			swizzle<0, 1, 2, 3> xyzw;
			swizzle<0, 1, 3, 0> xywx;
			swizzle<0, 1, 3, 1> xywy;
			swizzle<0, 1, 3, 2> xywz;
			swizzle<0, 1, 3, 3> xyww;
			swizzle<0, 2, 0, 0> xzxx;
			swizzle<0, 2, 0, 1> xzxy;
			swizzle<0, 2, 0, 2> xzxz;
			swizzle<0, 2, 0, 3> xzxw;
			swizzle<0, 2, 1, 0> xzyx;
			swizzle<0, 2, 1, 1> xzyy;
			swizzle<0, 2, 1, 2> xzyz;
			swizzle<0, 2, 1, 3> xzyw;
			swizzle<0, 2, 2, 0> xzzx;
			swizzle<0, 2, 2, 1> xzzy;
			swizzle<0, 2, 2, 2> xzzz;
			swizzle<0, 2, 2, 3> xzzw;
			swizzle<0, 2, 3, 0> xzwx;
			swizzle<0, 2, 3, 1> xzwy;
			swizzle<0, 2, 3, 2> xzwz;
			swizzle<0, 2, 3, 3> xzww;
			swizzle<0, 3, 0, 0> xwxx;
			swizzle<0, 3, 0, 1> xwxy;
			swizzle<0, 3, 0, 2> xwxz;
			swizzle<0, 3, 0, 3> xwxw;
			swizzle<0, 3, 1, 0> xwyx;
			swizzle<0, 3, 1, 1> xwyy;
			swizzle<0, 3, 1, 2> xwyz;
			swizzle<0, 3, 1, 3> xwyw;
			swizzle<0, 3, 2, 0> xwzx;
			swizzle<0, 3, 2, 1> xwzy;
			swizzle<0, 3, 2, 2> xwzz;
			swizzle<0, 3, 2, 3> xwzw;
			swizzle<0, 3, 3, 0> xwwx;
			swizzle<0, 3, 3, 1> xwwy;
			swizzle<0, 3, 3, 2> xwwz;
			swizzle<0, 3, 3, 3> xwww;
			swizzle<1, 0, 0, 0> yxxx;
			swizzle<1, 0, 0, 1> yxxy;
			swizzle<1, 0, 0, 2> yxxz;
			swizzle<1, 0, 0, 3> yxxw;
			swizzle<1, 0, 1, 0> yxyx;
			swizzle<1, 0, 1, 1> yxyy;
			swizzle<1, 0, 1, 2> yxyz;
			swizzle<1, 0, 1, 3> yxyw;
			swizzle<1, 0, 2, 0> yxzx;
			swizzle<1, 0, 2, 1> yxzy;
			swizzle<1, 0, 2, 2> yxzz;
			swizzle<1, 0, 2, 3> yxzw;
			swizzle<1, 0, 3, 0> yxwx;
			swizzle<1, 0, 3, 1> yxwy;
			swizzle<1, 0, 3, 2> yxwz;
			swizzle<1, 0, 3, 3> yxww;
			swizzle<1, 1, 0, 0> yyxx;
			swizzle<1, 1, 0, 1> yyxy;
			swizzle<1, 1, 0, 2> yyxz;
			swizzle<1, 1, 0, 3> yyxw;
			swizzle<1, 1, 1, 0> yyyx;
			swizzle<1, 1, 1, 1> yyyy;
			swizzle<1, 1, 1, 2> yyyz;
			swizzle<1, 1, 1, 3> yyyw;
			swizzle<1, 1, 2, 0> yyzx;
			swizzle<1, 1, 2, 1> yyzy;
			swizzle<1, 1, 2, 2> yyzz;
			swizzle<1, 1, 2, 3> yyzw;
			swizzle<1, 1, 3, 0> yywx;
			swizzle<1, 1, 3, 1> yywy;
			swizzle<1, 1, 3, 2> yywz;
			swizzle<1, 1, 3, 3> yyww;
			swizzle<1, 2, 0, 0> yzxx;
			swizzle<1, 2, 0, 1> yzxy;
			swizzle<1, 2, 0, 2> yzxz;
			swizzle<1, 2, 0, 3> yzxw;
			swizzle<1, 2, 1, 0> yzyx;
			swizzle<1, 2, 1, 1> yzyy;
			swizzle<1, 2, 1, 2> yzyz;
			swizzle<1, 2, 1, 3> yzyw;
			swizzle<1, 2, 2, 0> yzzx;
			swizzle<1, 2, 2, 1> yzzy;
			swizzle<1, 2, 2, 2> yzzz;
			swizzle<1, 2, 2, 3> yzzw;
			swizzle<1, 2, 3, 0> yzwx;
			swizzle<1, 2, 3, 1> yzwy;
			swizzle<1, 2, 3, 2> yzwz;
			swizzle<1, 2, 3, 3> yzww;
			swizzle<1, 3, 0, 0> ywxx;
			swizzle<1, 3, 0, 1> ywxy;
			swizzle<1, 3, 0, 2> ywxz;
			swizzle<1, 3, 0, 3> ywxw;
			swizzle<1, 3, 1, 0> ywyx;
			swizzle<1, 3, 1, 1> ywyy;
			swizzle<1, 3, 1, 2> ywyz;
			swizzle<1, 3, 1, 3> ywyw;
			swizzle<1, 3, 2, 0> ywzx;
			swizzle<1, 3, 2, 1> ywzy;
			swizzle<1, 3, 2, 2> ywzz;
			swizzle<1, 3, 2, 3> ywzw;
			swizzle<1, 3, 3, 0> ywwx;
			swizzle<1, 3, 3, 1> ywwy;
			swizzle<1, 3, 3, 2> ywwz;
			swizzle<1, 3, 3, 3> ywww;
			swizzle<2, 0, 0, 0> zxxx;
			swizzle<2, 0, 0, 1> zxxy;
			swizzle<2, 0, 0, 2> zxxz;
			swizzle<2, 0, 0, 3> zxxw;
			swizzle<2, 0, 1, 0> zxyx;
			swizzle<2, 0, 1, 1> zxyy;
			swizzle<2, 0, 1, 2> zxyz;
			swizzle<2, 0, 1, 3> zxyw;
			swizzle<2, 0, 2, 0> zxzx;
			swizzle<2, 0, 2, 1> zxzy;
			swizzle<2, 0, 2, 2> zxzz;
			swizzle<2, 0, 2, 3> zxzw;
			swizzle<2, 0, 3, 0> zxwx;
			swizzle<2, 0, 3, 1> zxwy;
			swizzle<2, 0, 3, 2> zxwz;
			swizzle<2, 0, 3, 3> zxww;
			swizzle<2, 1, 0, 0> zyxx;
			swizzle<2, 1, 0, 1> zyxy;
			swizzle<2, 1, 0, 2> zyxz;
			swizzle<2, 1, 0, 3> zyxw;
			swizzle<2, 1, 1, 0> zyyx;
			swizzle<2, 1, 1, 1> zyyy;
			swizzle<2, 1, 1, 2> zyyz;
			swizzle<2, 1, 1, 3> zyyw;
			swizzle<2, 1, 2, 0> zyzx;
			swizzle<2, 1, 2, 1> zyzy;
			swizzle<2, 1, 2, 2> zyzz;
			swizzle<2, 1, 2, 3> zyzw;
			swizzle<2, 1, 3, 0> zywx;
			swizzle<2, 1, 3, 1> zywy;
			swizzle<2, 1, 3, 2> zywz;
			swizzle<2, 1, 3, 3> zyww;
			swizzle<2, 2, 0, 0> zzxx;
			swizzle<2, 2, 0, 1> zzxy;
			swizzle<2, 2, 0, 2> zzxz;
			swizzle<2, 2, 0, 3> zzxw;
			swizzle<2, 2, 1, 0> zzyx;
			swizzle<2, 2, 1, 1> zzyy;
			swizzle<2, 2, 1, 2> zzyz;
			swizzle<2, 2, 1, 3> zzyw;
			swizzle<2, 2, 2, 0> zzzx;
			swizzle<2, 2, 2, 1> zzzy;
			swizzle<2, 2, 2, 2> zzzz;
			swizzle<2, 2, 2, 3> zzzw;
			swizzle<2, 2, 3, 0> zzwx;
			swizzle<2, 2, 3, 1> zzwy;
			swizzle<2, 2, 3, 2> zzwz;
			swizzle<2, 2, 3, 3> zzww;
			swizzle<2, 3, 0, 0> zwxx;
			swizzle<2, 3, 0, 1> zwxy;
			swizzle<2, 3, 0, 2> zwxz;
			swizzle<2, 3, 0, 3> zwxw;
			swizzle<2, 3, 1, 0> zwyx;
			swizzle<2, 3, 1, 1> zwyy;
			swizzle<2, 3, 1, 2> zwyz;
			swizzle<2, 3, 1, 3> zwyw;
			swizzle<2, 3, 2, 0> zwzx;
			swizzle<2, 3, 2, 1> zwzy;
			swizzle<2, 3, 2, 2> zwzz;
			swizzle<2, 3, 2, 3> zwzw;
			swizzle<2, 3, 3, 0> zwwx;
			swizzle<2, 3, 3, 1> zwwy;
			swizzle<2, 3, 3, 2> zwwz;
			swizzle<2, 3, 3, 3> zwww;
			swizzle<3, 0, 0, 0> wxxx;
			swizzle<3, 0, 0, 1> wxxy;
			swizzle<3, 0, 0, 2> wxxz;
			swizzle<3, 0, 0, 3> wxxw;
			swizzle<3, 0, 1, 0> wxyx;
			swizzle<3, 0, 1, 1> wxyy;
			swizzle<3, 0, 1, 2> wxyz;
			swizzle<3, 0, 1, 3> wxyw;
			swizzle<3, 0, 2, 0> wxzx;
			swizzle<3, 0, 2, 1> wxzy;
			swizzle<3, 0, 2, 2> wxzz;
			swizzle<3, 0, 2, 3> wxzw;
			swizzle<3, 0, 3, 0> wxwx;
			swizzle<3, 0, 3, 1> wxwy;
			swizzle<3, 0, 3, 2> wxwz;
			swizzle<3, 0, 3, 3> wxww;
			swizzle<3, 1, 0, 0> wyxx;
			swizzle<3, 1, 0, 1> wyxy;
			swizzle<3, 1, 0, 2> wyxz;
			swizzle<3, 1, 0, 3> wyxw;
			swizzle<3, 1, 1, 0> wyyx;
			swizzle<3, 1, 1, 1> wyyy;
			swizzle<3, 1, 1, 2> wyyz;
			swizzle<3, 1, 1, 3> wyyw;
			swizzle<3, 1, 2, 0> wyzx;
			swizzle<3, 1, 2, 1> wyzy;
			swizzle<3, 1, 2, 2> wyzz;
			swizzle<3, 1, 2, 3> wyzw;
			swizzle<3, 1, 3, 0> wywx;
			swizzle<3, 1, 3, 1> wywy;
			swizzle<3, 1, 3, 2> wywz;
			swizzle<3, 1, 3, 3> wyww;
			swizzle<3, 2, 0, 0> wzxx;
			swizzle<3, 2, 0, 1> wzxy;
			swizzle<3, 2, 0, 2> wzxz;
			swizzle<3, 2, 0, 3> wzxw;
			swizzle<3, 2, 1, 0> wzyx;
			swizzle<3, 2, 1, 1> wzyy;
			swizzle<3, 2, 1, 2> wzyz;
			swizzle<3, 2, 1, 3> wzyw;
			swizzle<3, 2, 2, 0> wzzx;
			swizzle<3, 2, 2, 1> wzzy;
			swizzle<3, 2, 2, 2> wzzz;
			swizzle<3, 2, 2, 3> wzzw;
			swizzle<3, 2, 3, 0> wzwx;
			swizzle<3, 2, 3, 1> wzwy;
			swizzle<3, 2, 3, 2> wzwz;
			swizzle<3, 2, 3, 3> wzww;
			swizzle<3, 3, 0, 0> wwxx;
			swizzle<3, 3, 0, 1> wwxy;
			swizzle<3, 3, 0, 2> wwxz;
			swizzle<3, 3, 0, 3> wwxw;
			swizzle<3, 3, 1, 0> wwyx;
			swizzle<3, 3, 1, 1> wwyy;
			swizzle<3, 3, 1, 2> wwyz;
			swizzle<3, 3, 1, 3> wwyw;
			swizzle<3, 3, 2, 0> wwzx;
			swizzle<3, 3, 2, 1> wwzy;
			swizzle<3, 3, 2, 2> wwzz;
			swizzle<3, 3, 2, 3> wwzw;
			swizzle<3, 3, 3, 0> wwwx;
			swizzle<3, 3, 3, 1> wwwy;
			swizzle<3, 3, 3, 2> wwwz;
			swizzle<3, 3, 3, 3> wwww;
			T v[4];
		};
		constexpr vec4() {}
		explicit constexpr vec4(T x) : x(x), y(x), z(x), w(x) {}
		constexpr vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		constexpr vec4(vec3<T> xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
		constexpr vec4(T x, vec3<T> yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
		constexpr vec4(vec2<T> v, T z, T w) : x(v.x), y(v.y), z(z), w(w) {}
		constexpr vec4(T x, vec2<T> v, T w) : x(x), y(v.x), z(v.y), w(w) {}
		constexpr vec4(T x, T y, vec2<T> v) : x(x), y(y), z(v.x), w(w.y) {}
		constexpr vec4(vec2<T> u, vec2<T> v) : x(u.x), y(u.y), z(v.x), w(v.y) {}

		constexpr vec4 operator+(const vec4& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
		constexpr vec4 operator-(const vec4& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
		constexpr vec4 operator-() const { return { -x,-y,-z,-w }; }
		constexpr vec4 operator*(const vec4& rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
		constexpr vec4 operator/(const vec4& rhs) const { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }
		constexpr vec4 operator+(const T rhs) const { return { x + rhs, y + rhs, z + rhs, w + rhs }; }
		constexpr vec4 operator-(const T rhs) const { return { x - rhs, y - rhs, z - rhs, w - rhs }; }
		constexpr vec4 operator*(const T rhs) const { return { x * rhs, y * rhs, z * rhs, w * rhs }; }
		constexpr vec4 operator/(const T rhs) const { return { x / rhs, y / rhs, z / rhs, w / rhs }; }
		vec4& operator+=(const vec4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
		vec4& operator-=(const vec4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
		vec4& operator*=(const vec4& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
		vec4& operator/=(const vec4& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }
		vec4& operator+=(const T rhs) { x += rhs; y += rhs; z += rhs; w += rhs; return *this; }
		vec4& operator-=(const T rhs) { x -= rhs; y -= rhs; z -= rhs; w -= rhs; return *this; }
		vec4& operator*=(const T rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
		vec4& operator/=(const T rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

		friend std::ostream& operator<<(std::ostream& stream, const vec4& f4) { stream << f4.x << ", " << f4.y << ", " << f4.z << ", " << f4.w; return stream; }
	};

	template<typename T>
	constexpr vec4<T> operator*(float l, const vec4<T>& right) { return right * l; }

	using float4 = vec4<float>;
	using int4 = vec4<float>;
	using float3 = vec3<float>;
	using int3 = vec3<int>;
	using float2 = vec2<float>;
	using int2 = vec2<float>;

	constexpr float sqrtNR(float x, float cur, float prev)
	{
		return cur == prev ? cur : sqrtNR(x, 0.5f*(cur + x / cur), cur);
	}

	constexpr float ce_sqrt(float x)
	{
		return x >= 0 && x < std::numeric_limits<float>::infinity() ? sqrtNR(x, x, 0) : std::numeric_limits<float>::quiet_NaN();
	}
	constexpr float ce_sqrt_rcp(float x)
	{
		return x >= 0 && x < std::numeric_limits<float>::infinity() ? 1.0f / sqrtNR(x, x, 0) : std::numeric_limits<float>::quiet_NaN();
	}

	/* Template arguement deduction happens before implicit conversion which is why vec2<T> isn't used here. Would be nice if it worked but no.
	* Clarification: If these function were template function akin to Foo(vec3<T>) then a call such ass Foo(v.xyz) would not compile since the type of v.xyz is not deduced.
	* Further assuming dot, length, normalize etc is only interesting for float vectors and not int. */

	constexpr float dot(const vec2<float>& a, const  vec2<float>& b) { return (a.x * b.x) + (a.y * b.y); }
	constexpr float dot(const vec3<float>& a, const  vec3<float>& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
	constexpr float dot(const vec4<float>& a, const  vec4<float>& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); }

	constexpr float length(const vec2<float>& a) { return ce_sqrt(dot(a, a)); }
	constexpr float length(const vec3<float>& a) { return ce_sqrt(dot(a, a)); }
	constexpr float length(const vec4<float>& a) { return ce_sqrt(dot(a, a)); }

	//Assuming normalization and reflection is only interesting for float vectors.
	constexpr vec2<float> normalize(const vec2<float>& a) { return a * ce_sqrt_rcp(dot(a, a)); }
	constexpr vec3<float> normalize(const vec3<float>& a) { return a * ce_sqrt_rcp(dot(a, a)); }
	constexpr vec4<float> normalize(const vec4<float>& a) { return a * ce_sqrt_rcp(dot(a, a)); }
	constexpr vec2<float> reflect(const vec2<float>& u, const vec2<float>& normal) { return (normal * 2.0f * dot(u, normal)) - u; }
	constexpr vec3<float> reflect(const vec3<float>& u, const vec3<float>& normal) { return (normal * 2.0f * dot(u, normal)) - u; }
	constexpr vec4<float> reflect(const vec4<float>& u, const vec4<float>& normal) { return (normal * 2.0f * dot(u, normal)) - u; }
	constexpr vec2<float> project(const vec2<float>& vector, const vec2<float>& onto) { return (dot(vector, onto) / length(onto)) * onto; }
	constexpr vec3<float> project(const vec3<float>& vector, const vec3<float>& onto) { return (dot(vector, onto) / length(onto)) * onto; }
	constexpr vec4<float> project(const vec4<float>& vector, const vec4<float>& onto) { return (dot(vector, onto) / length(onto)) * onto; }
	constexpr vec2<float> projectOnNormal(const vec2<float>& vector, const vec2<float>& onto) { return (dot(vector, onto)) * onto; }
	constexpr vec3<float> projectOnNormal(const vec3<float>& vector, const vec3<float>& onto) { return (dot(vector, onto)) * onto; }
	constexpr vec4<float> projectOnNormal(const vec4<float>& vector, const vec4<float>& onto) { return (dot(vector, onto)) * onto; }
	constexpr vec3<float> projectOnPlane(const vec3<float>& u, const vec3<float>& planeNormal) { return u - (projectOnNormal(u, planeNormal) * planeNormal); }
	constexpr vec3<float> cross(const vec3<float>& a, const vec3<float>& b) { return { a.y * b.z - b.y * a.z, -(a.x * b.z - a.z * b.x), a.x * b.y - a.y * b.x }; }
	//Disregarding the w-component as cross product isn't properly defined in 4 dimensions. You can also write cross(u.xyz, v.xyz);
	constexpr vec4<float> cross(const vec4<float>& a, const vec4<float>& b) { return { a.y * b.z - b.y * a.z, -(a.x * b.z - a.z * b.x), a.x * b.y - a.y * b.x, 0.0f }; }



	class mat4
	{
	public:
		template <unsigned c>
		class Columnizer
		{
		private:
			float v[16];
		public:
			operator vec4<float>() const { return { v[c], v[c + 4], v[c + 8], v[c + 12] }; }
		};
	public:
		union
		{
			struct
			{
				vec4<float> r1, r2, r3, r4;
			};
			Columnizer<0> c1;
			Columnizer<1> c2;
			Columnizer<2> c3;
			Columnizer<3> c4;
			struct
			{
				float m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
			};
			float v[16];
		};

		explicit constexpr mat4(float fill) : r1(fill, fill, fill, fill), r2(fill, fill, fill, fill), r3(fill, fill, fill, fill), r4(fill, fill, fill, fill) {}
		constexpr mat4() : r1(1, 0, 0, 0), r2(0, 1, 0, 0), r3(0, 0, 1, 0), r4(0, 0, 0, 1) {}
		constexpr mat4(vec4<float> r1, vec4<float> r2, vec4<float> r3, vec4<float> r4) : r1(r1), r2(r2), r3(r3), r4(r4) {}

		constexpr static mat4 Identity() { return { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 } }; }
		constexpr static mat4 Translation(float x, float y, float z) { return { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ x,y,z,1 } }; }
		constexpr static mat4 Translation(const vec3<float>& xyz) { return { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ xyz,1 } }; }
		static mat4 RotationRollPitchYaw(float radRoll, float radPitch, float radYaw) {
			const float cosyaw = std::cos(radYaw);
			const float sinyaw = std::sin(radYaw);
			const float cosroll = std::cos(radRoll);
			const float sinroll = std::sin(radRoll);
			const float cospitch = std::cos(radPitch);
			const float sinpitch = std::sin(radPitch);
			return {
				{ cosyaw * cosroll + sinyaw * sinpitch *sinroll,    sinroll * cospitch,     -sinyaw * cosroll + cosyaw * sinpitch*sinroll,      0 },
				{ -cosyaw * sinroll + sinyaw * sinpitch * cosroll,  cosroll * cospitch,     sinroll * sinyaw + cosyaw * sinpitch * cosroll,   0 },
				{ sinyaw * cospitch,                                -sinpitch,              cosyaw * cospitch,                                0 },
				{ 0,                                                0,                      0,                                                1 } };
		}
		static mat4 Rotation(const float3& rollPitchYawXYZ) { return mat4::RotationRollPitchYaw(rollPitchYawXYZ.x, rollPitchYawXYZ.y, rollPitchYawXYZ.z); }
		static mat4 RotationAxis(const float3& v, float angle) {
			const float c = std::cos(angle);
			const float s = std::sin(angle);
			const float cn = 1.0f - c;
			return {
				{ c + v.x*v.x*cn,   v.x*v.y*cn - v.z*s,   v.x*v.z*cn + v.y*s,   0 },
				{ v.y*v.x*cn + v.z*s,  c + v.y*v.y*cn,         v.y*v.z*cn - v.x*s,     0 },
				{ v.z*v.x*cn - v.y*s,  v.z*v.y*cn + v.x*s,     c + v.z*v.z*cn,         0 },
				{ 0,                0,                    0,                    1 }
			};
		}

		static mat4 RotationQuaternion(const float4& q)
		{
			return {
				{ 1 - 2 * (q.z*q.z + q.w*q.w), 2 * (q.y*q.z - q.w*q.x),   2 * (q.y*q.w + q.z*q.x), 0.0f },
				{ 2 * (q.y*q.z + q.w*q.x),     1 - 2 * (q.y*q.y + q.w*q.w), 2 * (q.z*q.w - q.y*q.x),   0.0f },
				{ 2 * (q.y*q.w - q.z*q.x),       2 * (q.z*q.w + q.y*q.x),   1 - 2 * (q.y*q.y + q.z*q.z), 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			};

		}

		static mat4 RotationX(float r) {
			const float c = std::cos(r);
			const float s = std::sin(r);
			return {
				{ 1, 0, 0, 0 },
				{ 0, c, s,0 },
				{ 0, -s, c, 0 },
				{ 0, 0, 0, 1 }
			};
		}

		static mat4 RotationY(float r) {
			const float c = std::cos(r);
			const float s = std::sin(r);
			return {
				{ c, 0, -s, 0 },
				{ 0, 1, 0, 0 },
				{ s, 0, c, 0 },
				{ 0, 0, 0, 1 }
			};
		}

		static mat4 RotationZ(float r) {
			const float c = std::cos(r);
			const float s = std::sin(r);
			return {
				{ c, s, 0, 0 },
				{ -s, c, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			};
		}

		constexpr static mat4 Scale(float x, float y, float z) { return { { x,0,0,0 },{ 0,y,0,0 },{ 0,0,z,0 },{ 0,0,0,1 } }; }
		constexpr static mat4 Scale(const float3& xyz) { return { { xyz.x,0,0,0 },{ 0,xyz.y,0,0 },{ 0,0,xyz.z,0 },{ 0,0,0,1 } }; }

		//Modified version of gluInvertMatrix from the MESA implementation of the GLU library.
		//https://stackoverflow.com/a/1148405/9576577
		static mat4 Inverse(const mat4& m) noexcept(false)
		{
			mat4 inv;
			float det;

			inv.v[0] = m.v[5] * m.v[10] * m.v[15] -
				m.v[5] * m.v[11] * m.v[14] -
				m.v[9] * m.v[6] * m.v[15] +
				m.v[9] * m.v[7] * m.v[14] +
				m.v[13] * m.v[6] * m.v[11] -
				m.v[13] * m.v[7] * m.v[10];

			inv.v[4] = -m.v[4] * m.v[10] * m.v[15] +
				m.v[4] * m.v[11] * m.v[14] +
				m.v[8] * m.v[6] * m.v[15] -
				m.v[8] * m.v[7] * m.v[14] -
				m.v[12] * m.v[6] * m.v[11] +
				m.v[12] * m.v[7] * m.v[10];

			inv.v[8] = m.v[4] * m.v[9] * m.v[15] -
				m.v[4] * m.v[11] * m.v[13] -
				m.v[8] * m.v[5] * m.v[15] +
				m.v[8] * m.v[7] * m.v[13] +
				m.v[12] * m.v[5] * m.v[11] -
				m.v[12] * m.v[7] * m.v[9];

			inv.v[12] = -m.v[4] * m.v[9] * m.v[14] +
				m.v[4] * m.v[10] * m.v[13] +
				m.v[8] * m.v[5] * m.v[14] -
				m.v[8] * m.v[6] * m.v[13] -
				m.v[12] * m.v[5] * m.v[10] +
				m.v[12] * m.v[6] * m.v[9];

			inv.v[1] = -m.v[1] * m.v[10] * m.v[15] +
				m.v[1] * m.v[11] * m.v[14] +
				m.v[9] * m.v[2] * m.v[15] -
				m.v[9] * m.v[3] * m.v[14] -
				m.v[13] * m.v[2] * m.v[11] +
				m.v[13] * m.v[3] * m.v[10];

			inv.v[5] = m.v[0] * m.v[10] * m.v[15] -
				m.v[0] * m.v[11] * m.v[14] -
				m.v[8] * m.v[2] * m.v[15] +
				m.v[8] * m.v[3] * m.v[14] +
				m.v[12] * m.v[2] * m.v[11] -
				m.v[12] * m.v[3] * m.v[10];

			inv.v[9] = -m.v[0] * m.v[9] * m.v[15] +
				m.v[0] * m.v[11] * m.v[13] +
				m.v[8] * m.v[1] * m.v[15] -
				m.v[8] * m.v[3] * m.v[13] -
				m.v[12] * m.v[1] * m.v[11] +
				m.v[12] * m.v[3] * m.v[9];

			inv.v[13] = m.v[0] * m.v[9] * m.v[14] -
				m.v[0] * m.v[10] * m.v[13] -
				m.v[8] * m.v[1] * m.v[14] +
				m.v[8] * m.v[2] * m.v[13] +
				m.v[12] * m.v[1] * m.v[10] -
				m.v[12] * m.v[2] * m.v[9];

			inv.v[2] = m.v[1] * m.v[6] * m.v[15] -
				m.v[1] * m.v[7] * m.v[14] -
				m.v[5] * m.v[2] * m.v[15] +
				m.v[5] * m.v[3] * m.v[14] +
				m.v[13] * m.v[2] * m.v[7] -
				m.v[13] * m.v[3] * m.v[6];

			inv.v[6] = -m.v[0] * m.v[6] * m.v[15] +
				m.v[0] * m.v[7] * m.v[14] +
				m.v[4] * m.v[2] * m.v[15] -
				m.v[4] * m.v[3] * m.v[14] -
				m.v[12] * m.v[2] * m.v[7] +
				m.v[12] * m.v[3] * m.v[6];

			inv.v[10] = m.v[0] * m.v[5] * m.v[15] -
				m.v[0] * m.v[7] * m.v[13] -
				m.v[4] * m.v[1] * m.v[15] +
				m.v[4] * m.v[3] * m.v[13] +
				m.v[12] * m.v[1] * m.v[7] -
				m.v[12] * m.v[3] * m.v[5];

			inv.v[14] = -m.v[0] * m.v[5] * m.v[14] +
				m.v[0] * m.v[6] * m.v[13] +
				m.v[4] * m.v[1] * m.v[14] -
				m.v[4] * m.v[2] * m.v[13] -
				m.v[12] * m.v[1] * m.v[6] +
				m.v[12] * m.v[2] * m.v[5];

			inv.v[3] = -m.v[1] * m.v[6] * m.v[11] +
				m.v[1] * m.v[7] * m.v[10] +
				m.v[5] * m.v[2] * m.v[11] -
				m.v[5] * m.v[3] * m.v[10] -
				m.v[9] * m.v[2] * m.v[7] +
				m.v[9] * m.v[3] * m.v[6];

			inv.v[7] = m.v[0] * m.v[6] * m.v[11] -
				m.v[0] * m.v[7] * m.v[10] -
				m.v[4] * m.v[2] * m.v[11] +
				m.v[4] * m.v[3] * m.v[10] +
				m.v[8] * m.v[2] * m.v[7] -
				m.v[8] * m.v[3] * m.v[6];

			inv.v[11] = -m.v[0] * m.v[5] * m.v[11] +
				m.v[0] * m.v[7] * m.v[9] +
				m.v[4] * m.v[1] * m.v[11] -
				m.v[4] * m.v[3] * m.v[9] -
				m.v[8] * m.v[1] * m.v[7] +
				m.v[8] * m.v[3] * m.v[5];

			inv.v[15] = m.v[0] * m.v[5] * m.v[10] -
				m.v[0] * m.v[6] * m.v[9] -
				m.v[4] * m.v[1] * m.v[10] +
				m.v[4] * m.v[2] * m.v[9] +
				m.v[8] * m.v[1] * m.v[6] -
				m.v[8] * m.v[2] * m.v[5];

			det = m.v[0] * inv.v[0] + m.v[1] * inv.v[4] + m.v[2] * inv.v[8] + m.v[3] * inv.v[12];

			if (det == 0)
				throw std::exception("Matrix is not invertable. determinant == 0");

			det = 1.0f / det;

			return det * inv;

		}

		//Note. You can use the ~operator as well for a less verbose way of getting the transposed matrix.
		constexpr static mat4 Transpose(const mat4& m) { return { m.c1, m.c2, m.c3, m.c4 }; }

		//Sadly the math functions in the standard library aren't constexpr.
		static mat4 PerspectiveLH(float fov, float aspectratio, float nearplane, float farplane)
		{
			return
			{
				{ 1.0f / (aspectratio * std::tan(fov / 2.0f)),0.0f,0.0f,0.0f },
				{ 0.0f, 1.0f / std::tan(fov / 2.0f),0.0f,0.0f },
				{ 0.0f, 0.0f, farplane / (farplane - nearplane), 1.0f },
				{ 0.0f, 0.0f, -(farplane*nearplane) / (farplane - nearplane), 0.0f }
			};
		}

		constexpr static mat4 LookToLH(const float3& eyePos, const float3& eyeDir, const float3& upDir)
		{
			const float3 f = eyeDir;
			const float3 s = cross(upDir, eyeDir);
			const float3 u = cross(eyeDir, s);

			return
			{
				{ s.x,u.x,f.x,0.0f },
				{ s.y,u.y,f.y,0.0f },
				{ s.z,u.z,f.z,0.0f },
				{ -dot(s,eyePos), -dot(u,eyePos), -dot(f,eyePos), 1.0f }
			};
		}

		mat4 operator-() const { return { -r1,-r2,-r3,-r4 }; }
		//Might as well use this as a transpose operator seeing as it has no other use
		mat4 operator~() const { return { c1, c2, c3, c4 }; }
		//Inverse operator
		mat4 operator!() const { return mat4::Inverse(*this); }
		mat4 operator+(const mat4& rhs) const { return { { r1 + rhs.r1 },{ r2 + rhs.r2 },{ r3 + rhs.r3 },{ r4 + rhs.r4 } }; }
		mat4 operator-(const mat4& rhs) const { return { { r1 - rhs.r1 },{ r2 - rhs.r2 },{ r3 - rhs.r3 },{ r4 - rhs.r4 } }; }
		mat4 operator/(const mat4& rhs) const { return { { r1 / rhs.r1 },{ r2 / rhs.r2 },{ r3 / rhs.r3 },{ r4 / rhs.r4 } }; }
		mat4 operator*(const mat4& rhs) const {
			return {
				{ dot(r1, rhs.c1),dot(r1, rhs.c2),dot(r1, rhs.c3),dot(r1, rhs.c4) },
				{ dot(r2, rhs.c1),dot(r2, rhs.c2),dot(r2, rhs.c3),dot(r2, rhs.c4) },
				{ dot(r3, rhs.c1),dot(r3, rhs.c2),dot(r3, rhs.c3),dot(r3, rhs.c4) },
				{ dot(r4, rhs.c1),dot(r4, rhs.c2),dot(r4, rhs.c3),dot(r4, rhs.c4) }
			};
		}

		mat4 operator*(float rhs) const { return { { r1 * rhs },{ r2 * rhs },{ r3 * rhs },{ r4 * rhs } }; }
		mat4 operator/(float rhs) const { return { { r1 / rhs },{ r2 / rhs },{ r3 / rhs },{ r4 / rhs } }; }

		vec4<float> operator*(const vec4<float>& v) { return { dot(v,c1), dot(v,c2), dot(v,c3), dot(v,c4) }; }

		friend mat4 operator*(float l, const mat4& right) { return right * l; }

		mat4& operator+=(const mat4& rhs) { r1 = { r1 + rhs.r1 }; r2 = { r2 + rhs.r2 }; r3 = { r3 + rhs.r3 }; r4 = { r4 + rhs.r4 }; return *this; }
		mat4& operator-=(const mat4& rhs) { r1 = { r1 - rhs.r1 }; r2 = { r2 - rhs.r2 }; r3 = { r3 - rhs.r3 }; r4 = { r4 - rhs.r4 }; return *this; }
		mat4& operator/=(const mat4& rhs) { r1 = { r1 / rhs.r1 }; r2 = { r2 / rhs.r2 }; r3 = { r3 / rhs.r3 }; r4 = { r4 / rhs.r4 }; return *this; }
		mat4& operator*=(const mat4& rhs) {
			*this = {
				{ dot(r1, rhs.c1),dot(r1, rhs.c2),dot(r1, rhs.c3),dot(r1, rhs.c4) },
				{ dot(r2, rhs.c1),dot(r2, rhs.c2),dot(r2, rhs.c3),dot(r2, rhs.c4) },
				{ dot(r3, rhs.c1),dot(r3, rhs.c2),dot(r3, rhs.c3),dot(r3, rhs.c4) },
				{ dot(r4, rhs.c1),dot(r4, rhs.c2),dot(r4, rhs.c3),dot(r4, rhs.c4) }
			};
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const mat4& m) { stream << m.r1 << "\n" << m.r2 << "\n" << m.r3 << "\n" << m.r4; return stream; }
	};
};
#endif
