#include "mat4.h"

namespace game {
	mat4::mat4 (void) {
		for (int it = 0; it < 16; ++ it) {
			if (it % 5 == 0)
				data [it] = 1.0;
			else
				data [it] = 0.0;
		}
	}
	mat4::mat4 (double f) {
		for (int it = 0; it < 16; ++ it) {
			if (it % 5 == 0)
				data [it] = f;
			else
				data [it] = 0.0;
		}
	}
	mat4::mat4 (quat q) {
		for (int it = 0; it < 16; ++ it) {
			if (it % 5 == 0)
				data [it] = 1.0;
			else
				data [it] = 0.0;
		}

		data [0]  = 1 - (2 * (q.axis.y * q.axis.y)) - (2 * (q.axis.z * q.axis.z));
		data [1]  = (2 * q.axis.x * q.axis.y) + (2 * q.rotation * q.axis.z);
		data [2]  = (2 * q.axis.x * q.axis.z) - (2 * q.rotation * q.axis.y);

		data [4]  = (2 * q.axis.x * q.axis.y) - (2 * q.rotation * q.axis.z);
		data [5]  = 1 - (2 * (q.axis.x * q.axis.x)) - (2 * (q.axis.z * q.axis.z));
		data [6]  = (2 * q.axis.y * q.axis.z) + (2 * q.rotation * q.axis.x);

		data [8]  = (2 * q.axis.x * q.axis.z) + (2 * q.rotation * q.axis.y);
		data [9]  = (2 * q.axis.y * q.axis.z) - (2 * q.rotation * q.axis.x);
		data [10] = 1 - (2 * (q.axis.x * q.axis.x)) - (2 * (q.axis.y * q.axis.y));
	}
	mat4::mat4 (double x1, double y1, double z1, double w1,
					double x2, double y2, double z2, double w2,
					double x3, double y3, double z3, double w3,
					double x4, double y4, double z4, double w4) {
		data [0]  = x1; data [1]  = y1; data [2]  = z1; data [3]  = w1; 
		data [4]  = x2; data [5]  = y2; data [6]  = z2; data [7]  = w2; 
		data [8]  = x3; data [9]  = y3; data [10] = z3; data [11] = w3; 
		data [12] = x4; data [13] = y4; data [14] = z4; data [15] = w4; 
	}
	mat4::~mat4 (void) { }
	
	double & mat4::at (int row, int column) {
		return data [(row << 2) + column];
	}
	double & mat4::at (int i) {
		return data [i];
	}
	mat4 mat4::inverse (void) {
		mat4 m;
		m.data [0] = data [5]  * data [10] * data [15] - 
					 data [5]  * data [11] * data [14] - 
					 data [9]  * data [6]  * data [15] + 
					 data [9]  * data [7]  * data [14] +
					 data [13] * data [6]  * data [11] - 
					 data [13] * data [7]  * data [10];

		m.data [4] = -data [4]  * data [10] * data [15] + 
					  data [4]  * data [11] * data [14] + 
					  data [8]  * data [6]  * data [15] - 
					  data [8]  * data [7]  * data [14] - 
					  data [12] * data [6]  * data [11] + 
					  data [12] * data [7]  * data [10];

		m.data [8] = data [4]  * data [9]  * data [15] - 
					 data [4]  * data [11] * data [13] - 
					 data [8]  * data [5]  * data [15] + 
					 data [8]  * data [7]  * data [13] + 
					 data [12] * data [5]  * data [11] - 
					 data [12] * data [7]  * data [9];

		m.data [12] = -data [4]  * data [9]  * data [14] + 
					   data [4]  * data [10] * data [13] +
					   data [8]  * data [5]  * data [14] - 
					   data [8]  * data [6]  * data [13] - 
					   data [12] * data [5]  * data [10] + 
					   data [12] * data [6]  * data [9];

		m.data [1] = -data [1]  * data [10] * data [15] + 
					  data [1]  * data [11] * data [14] + 
					  data [9]  * data [2]  * data [15] - 
					  data [9]  * data [3]  * data [14] - 
					  data [13] * data [2]  * data [11] + 
					  data [13] * data [3]  * data [10];

		m.data [5] = data [0]  * data [10] * data [15] - 
					 data [0]  * data [11] * data [14] - 
					 data [8]  * data [2]  * data [15] + 
					 data [8]  * data [3]  * data [14] + 
					 data [12] * data [2]  * data [11] - 
					 data [12] * data [3]  * data [10];

		m.data [9] = -data [0]  * data [9]  * data [15] + 
					  data [0]  * data [11] * data [13] + 
					  data [8]  * data [1]  * data [15] - 
					  data [8]  * data [3]  * data [13] - 
					  data [12] * data [1]  * data [11] + 
					  data [12] * data [3]  * data [9];

		m.data [13] = data [0]  * data [9]  * data [14] - 
					  data [0]  * data [10] * data [13] - 
					  data [8]  * data [1]  * data [14] + 
					  data [8]  * data [2]  * data [13] + 
					  data [12] * data [1]  * data [10] - 
					  data [12] * data [2]  * data [9];

		m.data [2] = data [1]  * data [6] * data [15] - 
					 data [1]  * data [7] * data [14] - 
					 data [5]  * data [2] * data [15] + 
					 data [5]  * data [3] * data [14] + 
					 data [13] * data [2] * data [7] - 
					 data [13] * data [3] * data [6];

		m.data [6] = -data [0]  * data [6] * data [15] + 
					  data [0]  * data [7] * data [14] + 
					  data [4]  * data [2] * data [15] - 
					  data [4]  * data [3] * data [14] - 
					  data [12] * data [2] * data [7] + 
					  data [12] * data [3] * data [6];

		m.data [10] = data [0]  * data [5] * data [15] - 
					  data [0]  * data [7] * data [13] - 
					  data [4]  * data [1] * data [15] + 
					  data [4]  * data [3] * data [13] + 
					  data [12] * data [1] * data [7] - 
					  data [12] * data [3] * data [5];

		m.data [14] = -data [0]  * data [5] * data [14] + 
					   data [0]  * data [6] * data [13] + 
					   data [4]  * data [1] * data [14] - 
					   data [4]  * data [2] * data [13] - 
					   data [12] * data [1] * data [6] + 
					   data [12] * data [2] * data [5];

		m.data [3] = -data [1] * data [6] * data [11] + 
					  data [1] * data [7] * data [10] + 
					  data [5] * data [2] * data [11] - 
					  data [5] * data [3] * data [10] - 
					  data [9] * data [2] * data [7] + 
					  data [9] * data [3] * data [6];

		m.data [7] = data [0] * data [6] * data [11] - 
					 data [0] * data [7] * data [10] - 
					 data [4] * data [2] * data [11] + 
					 data [4] * data [3] * data [10] + 
					 data [8] * data [2] * data [7] - 
					 data [8] * data [3] * data [6];

		m.data [11] = -data [0] * data [5] * data [11] + 
					   data [0] * data [7] * data [9] + 
					   data [4] * data [1] * data [11] - 
					   data [4] * data [3] * data [9] - 
					   data [8] * data [1] * data [7] + 
					   data [8] * data [3] * data [5];

		m.data [15] = data [0] * data [5] * data [10] - 
					  data [0] * data [6] * data [9] - 
					  data [4] * data [1] * data [10] + 
					  data [4] * data [2] * data [9] + 
					  data [8] * data [1] * data [6] - 
					  data [8] * data [2] * data [5];

		double det = data [0] * m.data [0] + data [1] * m.data [4] + data [2] * m.data [8] + data [3] * m.data [12];

		return m * (1.f / det);
	}
	mat4 mat4::transpose (void) {
		return mat4 (data [0], data [4], data  [8], data [12], 
					   data [1], data [5], data  [9], data [13], 
					   data [2], data [6], data [10], data [14], 
					   data [3], data [7], data [11], data [15]);
	}

	mat4 & mat4::operator *= (const mat4 & b) {
		mat4 mult = mat4 (0.f);
		for (int i = 0; i < 4; ++i) { 
			for (int j = 0; j < 4; ++j) { 
				for (int k = 0; k < 4; ++k) {
					mult.data [(i << 2) + j] += data [(i << 2) + k] * b.data [(k << 2) + j]; 
				}
			} 
		} 
		data = mult.data;

		return * this;
	}
	mat4 & mat4::operator *= (const double & b) {
		for (int it = 0; it < 16; ++ it) {
			data [it] *= b;
		}

		return * this;
	}

	const mat4 operator * (mat4 lhs, const mat4 & rhs) {
		return mat4 (lhs) *= rhs;
	}
	const mat4 operator * (mat4 lhs, const double & rhs) {
		return mat4 (lhs) *= rhs;
	}
	const vec3 operator * (mat4 lhs, const vec3 & rhs) {
		vec3 v_m ((lhs.at (0)  * rhs.x) + (lhs.at (1)  * rhs.y) + (lhs.at (2)  * rhs.z) + lhs.at (3),
				  (lhs.at (4)  * rhs.x) + (lhs.at (5)  * rhs.y) + (lhs.at (6)  * rhs.z) + lhs.at (7),
				  (lhs.at (8)  * rhs.x) + (lhs.at (9)  * rhs.y) + (lhs.at (10) * rhs.z) + lhs.at (11));
		double w = (lhs.at (12) * rhs.x) + (lhs.at (13) * rhs.y) + (lhs.at (14) * rhs.z) + lhs.at (15);
		if (w == 0)
			return vec3 (0.0);
		v_m *= (1.0 / w);		

		return v_m;
	}

	const double * mat4::pointer (void) {
		return data.data ();
	}
	std::array <float, 16> mat4::as_float (void) {
		std::array <float, 16> f;
		
		std::copy (data.begin (), data.end (), f.begin ());

		return f;
	}
}