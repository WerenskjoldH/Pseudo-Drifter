#ifndef VECTOR_H
#define VECTOR_H

/*
	File: vector.h
	Description: This file is from my Ronin Math Library, it has been slightly extended to support "dual vectors" and projections form world to screen space.

	https://github.com/WerenskjoldH/Ronin-Math-Libary
*/

#include <math.h>
#include <iostream>

#include "matrix.h"

namespace rn
{
	class vector3f
	{
	private:
	public:
		real x;
		real y;
		real z;
		// For 4-word speed increase, most computers now-a-days use 4-word memory busses and, as such, just 3-words requires an offset
		real padding = 0;

		// Getters/Setters
		real r() const
		{
			return x;
		}

		real g() const
		{
			return y;
		}

		real b() const
		{
			return z;
		}

		// Constructors
		vector3f(real iX = 0, real iY = 0, real iZ = 0) : x(iX), y(iY), z(iZ) {};

		// Deconstructor
		~vector3f() {}

		// Copy Constructor
		vector3f(const vector3f& v) : x(v.x), y(v.y), z(v.z) {};

		// Operator Overloads
		vector3f& operator=(const vector3f& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}

		// Add components to eachother and set equal
		void operator+=(const vector3f& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}

		// Subtract components from eachother and set equal
		void operator-=(const vector3f& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		// Multiply vector by scalar and set equal
		void operator*=(const real& s)
		{
			x *= s;
			y *= s;
			z *= s;
		}

		// Divide vector by scalar and set equal
		void operator/=(const real& s)
		{
			real mag = 1.f / s;
			x = x * mag;
			y = y * mag;
			z = z * mag;
		}

		// Vector cross product
		void operator%=(const vector3f& v)
		{
			*this = cross(v);
		}

		// Calculate scalar(dot) product and return result
		real operator*(const vector3f& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		// Divide by scalar and return result
		vector3f operator/(const real s) const
		{
			real mag = 1.f / s;
			return vector3f(x * mag, y * mag, z * mag);
		}

		// Cross vectors and return result
		vector3f operator%(const vector3f& v)
		{
			return cross(v);
		}

		// Returns scalar(dot) product
		vector3f operator*(const real s) const
		{
			return vector3f(s * x, s * y, s * z);
		}

#ifdef MATRIX_H
		// Multiply by matrix and return result
		vector3f operator*(matrix m) const
		{
			return vector3f(x * m(0, 0) + y * m(1, 0) + z * m(2, 0),
				x * m(0, 1) + y * m(1, 1) + z * m(2, 1),
				x * m(0, 2) + y * m(1, 2) + z * m(2, 2));
		}
#endif

		// Subtract component-wise and return result
		vector3f operator-(const vector3f& v) const
		{
			return vector3f(x - v.x, y - v.y, z - v.z);
		}

		// Add component-wise and return result
		vector3f operator+(const vector3f& v) const
		{
			return vector3f(x + v.x, y + v.y, z + v.z);
		}

		// Negate components and return result
		vector3f operator-() const
		{
			return vector3f(-x, -y, -z);
		}

		// Access components array-wise for modification
		real& operator[](int i)
		{
			switch (i)
			{
			case 2:
				return z;
				break;
			case 1:
				return y;
				break;
			case 0:
				return x;
				break;
			default:
				std::cout << "ERROR::VECTOR::OUT OF BOUNDS REQUEST" << std::endl;
			}
		}

		// Access components array-wise for reading
		real operator[](int i) const
		{
			switch (i)
			{
			case 2:
				return z;
				break;
			case 1:
				return y;
				break;
			case 0:
				return x;
				break;
			default:
				std::cout << "ERROR::VECTOR::OUT OF BOUNDS REQUEST" << std::endl;
			}
		}

		// Vector cross product
		vector3f cross(const vector3f& v) const
		{
			return vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}

		// Calculate component-wise product and return result
		vector3f componentProduct(const vector3f& v) const
		{
			return vector3f(x * v.x, y * v.y, z * v.z);
		}

		// Calculate the unit vector and return result
		vector3f unit() const
		{
			if (this->magnitude() == 0)
				return vector3f(0);
			return vector3f(*this / this->magnitude());
		}

		// Invert components
		void invert()
		{
			x *= -1;
			y *= -1;
			z *= -1;
		}

		// Normalize the vector and set equal
		void normalize()
		{
			(*this) = (*this).unit();
		}

		// Return magnitude of vector
		real magnitude() const
		{
			return std::sqrtf(x * x + y * y + z * z);
		}

		// Calculate squared magnitude and return result
		real squaredMagnitude()
		{
			return x * x + y * y + z * z;
		}

		// Borrowing this straight from Ian Millington
		// Add given vector scaled by float and set equal
		void addScaledVector(const vector3f& v, real t)
		{
			x += v.x * t;
			y += v.y * t;
			z += v.z * t;
		}

		real angleBetween(const vector3f& v) const
		{
			vector3f aU = this->unit();
			vector3f bU = v.unit();
			return acosf(aU * bU);
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const vector3f& v)
	{
		return os << v.x << " " << v.y << " " << v.z;
	}

	inline vector3f operator*(real s, const vector3f& v)
	{
		return vector3f(s * v.x, s * v.y, s * v.z);
	}

	// Or scalar product
	inline real dot(const vector3f& o, const vector3f& v)
	{
		return o.x * v.x + o.y * v.y + o.z * v.z;
	}

	inline vector3f unitVector(vector3f v)
	{
		return v / v.magnitude();
	}

	inline vector3f reflect(const vector3f v, const vector3f n)
	{
		return v - 2.0f * dot(v, n) * n;
	}

	// Returns a viable direction in a unit sphere
	inline vector3f randomInUnitSphere()
	{
		vector3f p;
		do
		{
			p = 2.0f * vector3f(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0)) - vector3f(1, 1, 1);
		} while (p.squaredMagnitude() >= 1.0f);
		return p;
	}

	struct dualVector {
		dualVector()
		{
			wV = 0;
			sV = 0;
		}

		dualVector(vector3f v) : wV{ v }
		{
			sV = vector3f(0);
		}

		// This stores the world vertex position
		vector3f wV;
		// This stores the screen position
		vector3f sV;
	};

	inline void project(dualVector& vec, const vector3f& camera, float depth, int windowWidth, int windowHeight, int roadWidth)
	{
		// Transform the point to camera space
		// Essentially, we just find the distance from the point to the camera
		float cX = vec.wV.x - camera.x, cY = vec.wV.y - camera.y, cZ = vec.wV.z - camera.z;
		float scaling = depth / cZ;

		// Transform the point from camera space to screen space x:[0 : 1] y:[0 : 1] from the top-left to the bottom-right and apply proper scaling
		vec.sV.x = round((windowWidth / 2.f) + (scaling * cX * windowWidth / 2.f));
		vec.sV.y = round((windowHeight / 2.f) - (scaling * cY * windowHeight / 2.f));
		vec.sV.z = round((scaling * (float)roadWidth * (float)windowWidth / 2.f));
	}

	// Based on the algorithm written by Ian Millington in "Game Physics Engine Development"
	// Right-handed coordinate system
#ifdef MATRIX_H
	inline void orthonormalBasis(vector3f* v1, vector3f* v2, vector3f* v3)
	{
		v1->normalize();
		(*v3) = (*v1) % (*v2);
		if (v3->squaredMagnitude() == 0.0)
			return;

		v3->normalize();
		(*v2) = (*v3) % (*v1);
	}
#endif

	class vector4f
	{
	private:
	public:
		real x;
		real y;
		real z;
		real w;

		// Getters/Setters
		real r() const
		{
			return x;
		}

		real g() const
		{
			return y;
		}

		real b() const
		{
			return z;
		}

		real a() const
		{
			return w;
		}

		// Outside function declerations
		real dot(const vector4f& o, const vector4f& v);

		// Constructors
		vector4f() : x(0.f), y(0.f), z(0.f), w(1.f) {};
		vector4f(real iX = 0, real iY = 0, real iZ = 0, real iW = 1) : x(iX), y(iY), z(iZ), w(iW) {};

		// Deconstructor
		~vector4f() {}

		// Copy Constructor
		vector4f(const vector4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {};

		// Operator Overloads
		vector4f& operator=(const vector4f& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;

			return *this;
		}

		void operator+=(const vector4f& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
		}

		void operator-=(const vector4f& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
		}

		void operator*=(const real& s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}

		void operator/=(const real& s)
		{
			real mag = 1.f / s;
			x = x * mag;
			y = y * mag;
			z = z * mag;
			w = w * mag;
		}

		// Scalar(dot) product
		real operator*(const vector4f& v) const
		{
			return x * v.x + y * v.y + z * v.z + w * v.w;
		}

		vector4f operator/(const real s) const
		{
			real mag = 1.f / s;
			return vector4f(x * mag, y * mag, z * mag, w * mag);
		}

		vector4f operator*(const real s) const
		{
			return vector4f(s * x, s * y, x * z, w * z);
		}

#ifdef MATRIX_H
		vector4f operator*(matrix m) const
		{
			return vector4f(x * m(0, 0) + y * m(1, 0) + z * m(2, 0) + w * m(3, 0),
				x * m(0, 1) + y * m(1, 1) + z * m(2, 1) + w * m(3, 1),
				x * m(0, 2) + y * m(1, 2) + z * m(2, 2) + w * m(3, 2),
				x * m(0, 3) + y * m(1, 3) + z * m(2, 3) + w * m(3, 3));
		}
#endif

		vector4f operator-(const vector4f& v) const
		{
			return vector4f(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		vector4f operator+(const vector4f& v) const
		{
			return vector4f(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		vector3f operator-() const
		{
			return vector3f(-x, -y, -z);
		}

		real& operator[](int i)
		{
			switch (i)
			{
			case 3:
				return w;
				break;
			case 2:
				return z;
				break;
			case 1:
				return y;
				break;
			case 0:
				return x;
				break;
			default:
				std::cout << "ERROR::VECTOR::OUT OF BOUNDS REQUEST" << std::endl;
			}
		}

		real operator[](int i) const
		{
			switch (i)
			{
			case 3:
				return w;
				break;
			case 2:
				return z;
				break;
			case 1:
				return y;
				break;
			case 0:
				return x;
				break;
			default:
				std::cout << "ERROR::VECTOR::OUT OF BOUNDS REQUEST" << std::endl;
			}
		}

		vector4f componentProduct(const vector4f& v) const
		{
			return vector4f(x * v.x, y * v.y, z * v.z, w * v.w);
		}

		vector4f unit()
		{
			return vector4f(*this / this->magnitude());
		}

		void invert()
		{
			x *= -1;
			y *= -1;
			z *= -1;
			w *= -1;
		}

		void normalize()
		{
			(*this) = (*this).unit();
		}

		real magnitude()
		{
			return std::sqrtf(x * x + y * y + z * z + w * w);
		}
		real squaredMagnitude()
		{
			return x * x + y * y + z * z + w * w;
		}

		// Borrowing this straight from Ian Millington
		// Add given vector scaled by float and set equal
		void addScaledVector(const vector4f& v, real t)
		{
			x += v.x * t;
			y += v.y * t;
			z += v.z * t;
			w += v.w * t;
		}
	};

	inline real dot(const vector4f& o, const vector4f& v)
	{
		return o.x * v.x + o.y * v.y + o.z * v.z + o.w * v.w;
	}

	inline std::ostream& operator<<(std::ostream& os, const vector4f& v)
	{
		return os << v.x << " " << v.y << " " << v.z << " " << v.w;
	}

	inline vector4f operator*(real s, const vector4f& v)
	{
		return vector4f(s * v.x, s * v.y, s * v.z, s * v.w);
	}

} // namespace rn

#endif