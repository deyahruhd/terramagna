#ifndef BLOC_h
#define BLOC_h

#include "vec.h"
#include <vector>

#include "color.h"

#define world_bloc_width 32

namespace game {
	class vertexbuffer;

	class bloc {
	public:
		bloc (vec3 position);
		bloc (vec3 position, vec3 bounds);
		bloc (void);
	   ~bloc (void);

		void clear (void);

	    const color get (vec3 loc) const;
		const color get  (unsigned int loc) const;
		const vec3 get_bounds (void) const;
		const vec3 get_position (void) const;

		bool is_full (void) const;
		bool is_null (void) const;

		void reinitialize (void);
		void resize (void);

		bool set (vec3 loc, color c);
		bool set (unsigned int loc, color c);
		void set_bounds (vec3 bounds);
		void set_position (vec3 position);

	private:
		vec3 bounds, prev_bounds, position;

	    std::vector <int> data;

		void initialize (void);
	};

	unsigned int get_1d_index (vec3 loc, vec3 bounds);
	vec3 get_3d_index (unsigned int loc, vec3 bounds);

	void populate (vertexbuffer & ve, bloc & b);
	void populate (vertexbuffer & ve, vertexbuffer & ve_bounding_box, vertexbuffer & ve_grid, bloc & b);
}

#endif