#include <cmath>

#include "vec.h"

#define vec_lerp_tolerance 0.00001

namespace game {
	vec2::vec2 (const vec2 & b) : x (b.x), y (b.y) {
	}
	vec2::vec2 (const vec3 & b) : x (b.x), y (b.y) {
	}
	vec2::vec2 (void) : x (0), y (0) { 
	}
	vec2::vec2 (double f) : x (f), y (f) {
	}
	vec2::vec2 (float x, float y) : x ((double) x), y ((double) y) {
	}
	vec2::vec2 (int x, int y) : x ((double) x), y ((double) y) {
	}
	vec2::vec2 (unsigned int x, unsigned int y) : x ((double) x), y ((double) y) {
	}
	vec2::vec2 (double x, double y) : x (x), y (y) {
	}

	const double vec2::magnitude (void) const {
		return sqrt ((x * x) +
					 (y * y));
	}
	void vec2::normalize (void) {
		resize (1.0);
	}
	vec2 vec2::normalized (void) const {
		vec2 v = * this;
		v.resize (1.0);
		return v;
	}
	void  vec2::resize (double f) {
		double w = f / magnitude ();
		x *= w;
		y *= w;
	}

	double vec2::dot (vec2 & b) const {
		return (x * b.x) +
			   (y * b.y);
	}
	vec2 vec2::project (vec2 & b) const {
		vec2 v = (* this);
		double scalar_project = b.dot (v) / v.dot (v);
		return v * scalar_project;
	}

	vec2 & vec2::operator = (const vec2 & b) {
		if (this != & b) {
			x = b.x;
			y = b.y;
		}

		return * this;
	}

	vec2 & vec2::operator += (const vec2 & b) {
		x += b.x;
		y += b.y;

		return * this;
	}
	vec2 & vec2::operator -= (const vec2 & b) {
		x -= b.x;
		y -= b.y;

		return * this;
	}
	vec2 & vec2::operator *= (const double & b) {
		x *= b;
		y *= b;

		return * this;
	}

	const vec2 operator + (vec2 lhs, const vec2 & rhs) {
		return vec2 (lhs) += rhs;
	}
	const vec2 operator - (vec2 lhs, const vec2 & rhs) {
		return vec2 (lhs) -= rhs;
	}
	const vec2 operator -  (vec2 lhs) {
		return lhs * -1;
	}
	const vec2 operator * (vec2 lhs, const double & rhs) {
		return vec2 (lhs) *= rhs;
	}

	const bool operator <  (vec2 lhs, const vec2 & rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y;
	}
	const bool operator <= (vec2 lhs, const vec2 & rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y;
	}
	const bool operator == (vec2 lhs, const vec2 & rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	const bool operator != (vec2 lhs, const vec2 & rhs) {
		return ! (lhs == rhs);
	}

	vec2 vec2::floor (void) const {
		return vec2 (std::floor (x), std::floor (y));
	}
	vec2 vec2::ceil (void) const {
		return vec2 (std::ceil (x), std::ceil (y));
	}
	vec2 vec2::absolute (void) const {
		return vec2 (abs (x), abs (y));
	}

	vec3::vec3 (const vec3 & b) : x (b.x), y (b.y), z (b.z) {
	}
	vec3::vec3 (void) : x (0), y (0), z (0) { 
	}
	vec3::vec3 (double f) : x (f), y (f), z (f) {
	}
	vec3::vec3 (const vec2 & v, double f) : x (v.x), y (v.y), z (f) {
	}
	vec3::vec3 (float x, float y, float z) : x ((double) x), y ((double) y), z ((double) z) {
	}
	vec3::vec3 (int x, int y, int z) : x ((double) x), y ((double) y), z ((double) z) {
	}
	vec3::vec3 (unsigned int x, unsigned int y, unsigned int z) : x ((double) x), y ((double) y), z ((double) z) {
	}
	vec3::vec3 (double x, double y, double z) : x (x), y (y), z (z) {
	}

	const double vec3::magnitude (void) const {
		return sqrt ((x * x) +
					 (y * y) +
					 (z * z));
	}
	void vec3::normalize (void) {
		resize (1.0);
	}
	vec3 vec3::normalized (void) const {
		vec3 v = * this;
		v.resize (1.0);
		return v;
	}
	void  vec3::resize (double f) {
		double w = f / magnitude ();
		x *= w;
		y *= w;
		z *= w;
	}
	double vec3::dot (vec3 & b) const {
		return (x * b.x) +
			   (y * b.y) +
			   (z * b.z);
	}
	vec3 vec3::project (vec3 & b) const {
		vec3 v = (* this);
		double scalar_project = b.dot (v) / v.dot (v);
		return v * scalar_project;
	}
	vec3 vec3::cross (vec3 & b) const {
		return vec3 ((y * b.z) - (z * b.y),
					 (z * b.x) - (x * b.z),
					 (x * b.y) - (y * b.x));
	}
	vec3 vec3::absolute (void) const {
		return vec3 (abs (x), abs (y), abs (z));
	}

	vec3 & vec3::operator = (const vec3 & b) {
		if (* this != b) {
			x = b.x;
			y = b.y;
			z = b.z;
		}

		return * this;
	}

	vec3 & vec3::operator += (const vec3 & b) {
		x += b.x;
		y += b.y;
		z += b.z;

		return * this;
	}
	vec3 & vec3::operator -= (const vec3 & b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;

		return * this;
	}
	vec3 & vec3::operator *= (const double & b) {
		x *= b;
		y *= b;
		z *= b;

		return * this;
	}

	const vec3 operator + (vec3 lhs, const vec3 & rhs) {
		return vec3 (lhs) += rhs;
	}
	const vec3 operator -  (vec3 lhs) {
		return lhs * -1;
	}
	const vec3 operator - (vec3 lhs, const vec3 & rhs) {
		return vec3 (lhs) -= rhs;
	}
	const vec3 operator * (vec3 lhs, const double & rhs) {
		return vec3 (lhs) *= rhs;
	}

	const bool operator <  (vec3 lhs, const vec3 & rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
	}
	const bool operator <= (vec3 lhs, const vec3 & rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
	}
	const bool operator == (vec3 lhs, const vec3 & rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
	const bool operator != (vec3 lhs, const vec3 & rhs) {
		return ! (lhs == rhs);
	}
	vec3 vec3::floor (void) const {
		return vec3 (std::floor (x), std::floor (y), std::floor (z));
	}
	vec3 vec3::ceil (void) const {
		return vec3 (std::ceil (x), std::ceil (y), std::ceil (z));
	}

	const double distance (vec2 lhs, vec2 rhs) {
		vec2 mag = (rhs - lhs);
		return mag.magnitude ();
	}
	const double distance (vec3 lhs, vec3 rhs) {
		vec3 mag = (rhs - lhs);
		return mag.magnitude ();
	}

	vec2 vec_lerp (vec2 lhs, vec2 rhs, double factor) {
		if (rhs == lhs || factor >= 1.0 || distance (lhs, rhs) < vec_lerp_tolerance)
			return rhs;

		return lhs + ((rhs - lhs) * factor);
	}
	vec3 vec_lerp (vec3 lhs, vec3 rhs, double factor) {
		if (rhs == lhs || factor >= 1.0 || distance (lhs, rhs) < vec_lerp_tolerance)
			return rhs;

		return lhs + ((rhs - lhs) * factor);
	}
}