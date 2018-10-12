#ifndef QUAT_h
#define QUAT_h

#include "vec.h"

#define tau 3.14159 * 2

namespace game {
	struct quat {
		vec3 axis; // Imaginary component of the quaternion.

		quat (double rot, double x, double y, double z); // Initializes the quaternion with the rotation and three doubles representing the axis.
		quat (double rot, vec3 axis); // Initiaizes the quaternion with the rotation and axis vector.
		quat (double rot, float x, float y, float z); // Initializes the quaternion with the rotation and three floating point numbers representing the axis.
		quat (double rot, int x, int y, int z); // Initializes the quaternion with the rotation and three integers representing the axis.
		quat (void);

		const double dot (quat & b) const; // Returns the dot product of the quaternion and this one.

		const double magnitude (void) const; // Returns the magnitude of this quaternion.

		quat & operator =  (const quat & b); // Assigns the quaternion.
		quat & operator += (const quat & b); // Adds this quaternion and the other, then assigns the result.
		quat & operator *= (const quat & b); // Multiplies this quaternion and the other, then assigns the result.
		quat & operator *= (const double f); // Multiplies this quaternion and the double, then assigns the result.

		void normalize (void); // Normalizes the quaternion.
		quat normalized (void) const; // Returns the normalized quaternion

		void rotate (vec3 axis, double rotation, bool object_space); /** Performs a rotation on this quaternion around the axis and the amount in radians. **/
		double rotation; /* The real scalar part of the quaternion. */

	private:
		const double magnitude_product (void) const; // Wrap the power and multiplication part of the magnitude formula
	};

	const quat operator *  (quat lhs, const quat & rhs);
	const quat operator *  (quat lhs, const double f);
	const vec3 operator *  (quat lhs, const vec3 rhs);
	const quat operator -  (quat lhs);
	const quat operator +  (quat lhs, quat rhs);
	const bool operator == (quat lhs, const quat & rhs);
	const bool operator != (quat lhs, const quat & rhs);

	quat quat_lerp (quat lhs, quat rhs, double frac);
}

#endif