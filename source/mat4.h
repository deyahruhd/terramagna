#ifndef MAT4_h
#define MAT4_h

#include <array>

#include "quat.h"

#include "vec.h"

namespace game {
	class mat4 {
	public:
		std::array <float, 16> as_float (void); // Returns the matrix as an array of 16 floats.
		double & at (int i, int j); // Returns the number at the 2D indice of this matrix.
		double & at (int i); // Returns the number at the 1D indice of this matrix.

		mat4 inverse (void); // Returns the inverse of this matrix.

	   ~mat4 (void);

		mat4 (double f);   // Initilizes an identity matrix with the double.
		mat4 (double x1, double y1, double z1, double w1,
			  double x2, double y2, double z2, double w2,
			  double x3, double y3, double z3, double w3,
			  double x4, double y4, double z4, double w4); // Initializes a matrix from the float arguments.
		mat4 (quat q); // Initializes a matrix representation of the quaternion.
		mat4 (void);   // Initializes a standard identity matrix.

		mat4 & operator *= (const mat4   & b); // Multiplies this matrix with the matrix and assigns the result.
		mat4 & operator *= (const double & b); // Multiplies this matrix with the double and assigns the result.

		const double * pointer (void); // Returns the pointer to the internal array.

		mat4 transpose (void); // Returns the transpose of this matrix.

	private:
		std::array <double, 16> data; // Array of 16 doubles storing the values of this matrix.
	};

	const mat4 operator * (mat4 lhs, const double & rhs); // Returns the product of the matrix and double.
	const mat4 operator * (mat4 lhs, const mat4   & rhs); // Returns the product of the two matrices.
	const vec3 operator * (mat4 lhs, const vec3   & rhs); // Returns the product of the matrix and vector.
}

#endif