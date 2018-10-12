#ifndef VEC_h
#define VEC_h

namespace game {
	struct vec3; // Forward declaration; vec2 constructor uses a vec3 object

	struct vec2 {
		// Preprocessor statements for UV coordinates
		#define u x
		#define v y 

		
		vec2 absolute (void) const;

		vec2 ceil (void) const;

		double dot       (vec2 & b) const;

		vec2 floor (void) const;

		const double magnitude (void) const;

		void  normalize (void);
		vec2 normalized (void) const;

		vec2 & operator *= (const double & b);
		vec2 & operator =  (const vec2   & b);
		vec2 & operator += (const vec2   & b);
		vec2 & operator -= (const vec2   & b);

		vec2 (const vec2 & b);
		vec2 (double f);
		vec2 (double x, double y);
		vec2 (float x, float y);
		vec2 (int x, int y);
		vec2 (unsigned int x, unsigned int y);
		vec2 (const vec3 & v);
		vec2 (void);

		vec2 project    (vec2 & b) const;

		void   resize (double f);

		double x, y;
	};

	const vec2 operator +  (vec2 lhs, const vec2   & rhs);
	const vec2 operator -  (vec2 lhs);
	const vec2 operator -  (vec2 lhs, const vec2   & rhs);
	const vec2 operator *  (vec2 lhs, const double & rhs);

	const bool operator <  (vec2 lhs, const vec2   & rhs);
	const bool operator <= (vec2 lhs, const vec2   & rhs);
	const bool operator == (vec2 lhs, const vec2   & rhs);
	const bool operator != (vec2 lhs, const vec2   & rhs);

	struct vec3 {
		vec3 absolute (void) const;

		vec3 ceil (void) const;
		vec3 cross (vec3 & b) const;

		double dot (vec3 & b) const;

		vec3 floor (void) const;

		const double magnitude (void) const;

		void  normalize (void);
		vec3 normalized (void) const;

		vec3 & operator *= (const double & b);
		vec3 & operator =  (const vec3   & b);
		vec3 & operator += (const vec3   & b);
		vec3 & operator -= (const vec3   & b);

		vec3 project    (vec3 & b) const; // Projects vector B onto this vector.

		void resize (double f);

		vec3 (const vec3 & b);
		vec3 (double f);
		vec3 (double x, double y, double z);
		vec3 (float x, float y, float z);
		vec3 (int x, int y, int z);
		vec3 (unsigned int x, unsigned int y, unsigned int z);
		vec3 (const vec2 & v, double z);
		vec3 (void);

		double x, y, z;
	};

	const double distance (vec2 lhs, vec2 rhs);
	const double distance (vec3 lhs, vec3 rhs);

	const vec3 operator *  (vec3 lhs, const double & rhs);
	const vec3 operator +  (vec3 lhs, const vec3   & rhs);
	const vec3 operator -  (vec3 lhs);
	const vec3 operator -  (vec3 lhs, const vec3   & rhs);

	const bool operator <  (vec3 lhs, const vec3   & rhs);
	const bool operator <= (vec3 lhs, const vec3   & rhs);
	const bool operator == (vec3 lhs, const vec3   & rhs);
	const bool operator != (vec3 lhs, const vec3   & rhs);

	vec2 vec_lerp (vec2 lhs, const vec2 rhs, double factor);
	vec3 vec_lerp (vec3 lhs, const vec3 rhs, double factor);
}

#endif