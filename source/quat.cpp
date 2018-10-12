#include <cmath>

#include "quat.h"

#define NORMALIZATION_TOLERANCE 0.01f

namespace game {
	quat::quat (void) : rotation (0), axis (vec3 (0, 1, 0)) { 
	}
	quat::quat (double rotation, float x, float y, float z) : rotation (rotation), axis (vec3 (x, y, z)) { 
	}
	quat::quat (double rotation, int x, int y, int z) : rotation (rotation), axis (vec3 (x, y, z)) { 
	}
	quat::quat (double rotation, double x, double y, double z) : rotation (rotation), axis (vec3 (x, y, z)) { 
	}
	quat::quat (double rotation, vec3 axis) : rotation (rotation), axis (axis) { 
	}

	const double quat::magnitude_product (void) const {
		return (axis.x * axis.x) + (axis.y * axis.y) + (axis.z * axis.z) + (rotation * rotation);
	}
	const double quat::magnitude (void) const {
		return sqrt (magnitude_product ());
	}
	void  quat::normalize (void) {
		* this *= (1.0 / magnitude ());
	}
	quat quat::normalized (void) const {
		return * this * (1.0 / magnitude ());
	}
	const double quat::dot (quat & b) const {
		return (axis.x * b.axis.x) +
			   (axis.y * b.axis.y) +
			   (axis.z * b.axis.z) +
			   (rotation * rotation);
	}

	void quat::rotate (vec3 axis, double rot, bool object_space) {
		quat local (cos (rot / 2), axis.normalized () * sin (rot / 2.f));

		if (object_space)
			* this = (local * (* this));
		else
			* this *= local;
	}

	quat & quat::operator = (const quat & b) {
		if (* this != b) {
			axis = b.axis;
			rotation = b.rotation;
		}

		return * this;
	}

	quat & quat::operator *= (const quat & b) {
		quat temp = * this;

		axis.x   = (temp.rotation * b.axis.x)   + (temp.axis.x * b.rotation) + (temp.axis.y * b.axis.z)   - (temp.axis.z * b.axis.y);
		axis.y   = (temp.rotation * b.axis.y)   - (temp.axis.x * b.axis.z)   + (temp.axis.y * b.rotation) + (temp.axis.z * b.axis.x);
		axis.z   = (temp.rotation * b.axis.z)   + (temp.axis.x * b.axis.y)   - (temp.axis.y * b.axis.x)   + (temp.axis.z * b.rotation);
		rotation = (temp.rotation * b.rotation) - (temp.axis.x * b.axis.x)   - (temp.axis.y * b.axis.y)   - (temp.axis.z * b.axis.z);

		normalize ();

		return * this;
	}
	quat & quat::operator *= (const double f) {
		axis *= f;
		rotation *= f;

		return * this;
	}

	quat & quat::operator += (const quat & b) {
		axis += b.axis;
		rotation += b.rotation;

		return * this;
	}

	const quat operator * (quat lhs, const quat & rhs) {
		return quat (lhs) *= rhs;
	}
	const quat operator * (quat lhs, const double f) {
		return quat (lhs) *= f;
	}
	const vec3 operator * (quat lhs, vec3 rhs) {
		vec3 t = lhs.axis.cross (rhs) * 2;
		return rhs + t * lhs.rotation + lhs.axis.cross (t);
	}

	const quat operator - (quat lhs) {
		return quat (- lhs.rotation, - lhs.axis);
	}
	const quat operator +  (quat lhs, quat rhs) {
		return quat (lhs) += rhs;
	}

	const bool operator == (quat lhs, const quat & rhs) {
		if (lhs.axis == rhs.axis && lhs.rotation == rhs.rotation)
			return true;

		return false;
	}
	const bool operator != (quat lhs, const quat & rhs) {
		return ! (lhs == rhs);
	}

	quat quat_lerp (quat lhs, quat rhs, double frac) {
		if (frac >= 1.0)
			return rhs;

		quat c;
		double dot = lhs.dot (rhs);

		if (dot < 0.0) {
			dot = - dot;
			c = - rhs;
		} else 
			c = rhs;
		
		if (dot < 0.95) {
			double angle = acos (dot);
			return ((lhs * sin (angle * (1.0 - frac)) + rhs * sin (angle * frac)) * (1.0 / sin (angle))).normalized ();
		} else					
			return (lhs + ((rhs + - lhs) * frac)).normalized ();	
	}
}
