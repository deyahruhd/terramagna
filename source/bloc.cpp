#include "bloc.h"
#include "vertexbuffer.h"

#include <iostream>

#define NULL 0

namespace game {
	unsigned int get_1d_index (vec3 loc, vec3 bounds) {
		return ((unsigned int) loc.z + (unsigned int) bounds.z * ((unsigned int) loc.y + (unsigned int) bounds.y * (unsigned int) loc.x));
	}
	vec3 get_3d_index (unsigned int loc, vec3 bounds) {
		return vec3 ((unsigned int) loc / ((unsigned int) bounds.x * (unsigned int) bounds.y),
					 (unsigned int) ((loc / (unsigned int) bounds.x) % (unsigned int) bounds.y), 
					 (unsigned int) (loc % (unsigned int) bounds.x));
	}

	bloc::bloc (void) {
	}
	bloc::bloc (vec3 position) : position (position), bounds (vec3 (world_bloc_width)), prev_bounds (bounds), data () {
	}
	bloc::bloc (vec3 position, vec3 bounds) : position (position), bounds (bounds), prev_bounds (bounds), data () {
	}
	bloc::~bloc (void) {
		clear ();
	}

	bool bloc::is_full (void) const {
		for (auto i = data.begin (); i != data.end (); ++ i) {
			if ((* i) == 0)
				return false;
		}

		return true;
	}
	bool bloc::is_null (void) const {
		return data.size () == 0;
	}

	const color bloc::get (vec3 loc) const {
		if (is_null ())
			return color (0);

		if (loc.x >= bounds.x || loc.x < 0 ||
			loc.y >= bounds.y || loc.y < 0 ||
			loc.z >= bounds.z || loc.z < 0)
			return color (0);
		
		return get (get_1d_index (loc, bounds));
	}
	const color bloc::get (unsigned int loc) const {
		if (is_null ())
			return color (0);

		return color (data.at (loc));
	}
	const vec3 bloc::get_bounds (void) const {
		return bounds;
	}
	const vec3 bloc::get_position (void) const {
		return position;
	}
	bool bloc::set (vec3 loc, color c) {
		if (is_null ())
			initialize ();
		
		if (loc.x >= bounds.x || loc.x <= -1 ||
			loc.y >= bounds.y || loc.y <= -1 ||
			loc.z >= bounds.z || loc.z <= -1)
			return false;

		return set (get_1d_index (loc, bounds), c);
	}
	bool bloc::set (unsigned int loc, color c) {
		if (is_null ())
			initialize ();

		if (loc >= (bounds.x * bounds.y * bounds.z))
			return false;
		
		data.at (loc) = c.get_value ();

		return true;
	}

	void bloc::initialize (void) {
		if (! is_null ())
			return;

		data.reserve ((unsigned int) bounds.x * (unsigned int) bounds.y * (unsigned int) bounds.z);
		for (unsigned int it = 0; it < bounds.x * bounds.y * bounds.z; ++ it) {
			data.push_back (0);
		}

		data.shrink_to_fit ();
	}

	void bloc::clear (void) {
		if (is_null ())
			return;

		data.clear ();
		data.shrink_to_fit ();
	}

	

	void populate (vertexbuffer & ve, bloc & b) {
		if (b.is_null ())
			return;
		if (b.is_full ())
			return;
		if (ve.get_params ().type == vertexbuffer_texture)
			return;
		if (ve.get_params ().mode != GL_TRIANGLES)
			return;

		vec3 bounds = b.get_bounds ();

		for (unsigned int it = 0; it < (bounds.x * bounds.y * bounds.z); ++ it) {
			vec3 index = get_3d_index (it, bounds);

			bool left, right, top, bottom, front, back;
			left = right = top = bottom = front = back = true;

			if (b.get (index).a () == 0)
				continue;

			if (index.x > 0)
				left = b.get (index + vec3 (-1, 0, 0)).a () == 0;
			if (index.x < bounds.x - 1)
				right = b.get (index + vec3 (1, 0, 0)).a () == 0;

			if (index.y > 0)
				bottom = b.get (index + vec3 (0, -1, 0)).a () == 0;
			if (index.y < bounds.y - 1)
				top = b.get (index + vec3 (0, 1, 0)).a () == 0;

			if (index.z > 0)
				back = b.get (index + vec3 (0, 0, -1)).a () == 0;
			if (index.z < bounds.z - 1)
				front = b.get (index + vec3 (0, 0, 1)).a () == 0;

			color c = b.get (index);

			vec3 bounds_n_index = index + (bounds * -0.5);

			if (right) {
				ve.add_tri (bounds_n_index + vec3 (1, 0, 1), bounds_n_index + vec3 (1, 0, 0), bounds_n_index + vec3 (1, 1, 0), c); 
				ve.add_tri (bounds_n_index + vec3 (1, 1, 1), bounds_n_index + vec3 (1, 0, 1), bounds_n_index + vec3 (1, 1, 0), c);
			}
			if (left) {
				ve.add_tri (bounds_n_index + vec3 (0, 0, 0), bounds_n_index + vec3 (0, 0, 1), bounds_n_index + vec3 (0, 1, 0), c);
				ve.add_tri (bounds_n_index + vec3 (0, 1, 1), bounds_n_index + vec3 (0, 1, 0), bounds_n_index + vec3 (0, 0, 1), c); 
			}
			if (top) {
				ve.add_tri (bounds_n_index + vec3 (1, 1, 0), bounds_n_index + vec3 (0, 1, 0), bounds_n_index + vec3 (1, 1, 1), c);
				ve.add_tri (bounds_n_index + vec3 (0, 1, 1), bounds_n_index + vec3 (1, 1, 1), bounds_n_index + vec3 (0, 1, 0), c); 
			}
			if (bottom) {
				ve.add_tri (bounds_n_index + vec3 (1, 0, 0), bounds_n_index + vec3 (1, 0, 1), bounds_n_index + vec3 (0, 0, 0), c);
				ve.add_tri (bounds_n_index + vec3 (1, 0, 1), bounds_n_index + vec3 (0, 0, 1), bounds_n_index + vec3 (0, 0, 0), c);
			}
			if (front) {
				ve.add_tri (bounds_n_index + vec3 (1, 1, 1), bounds_n_index + vec3 (0, 0, 1), bounds_n_index + vec3 (1, 0, 1), c);
				ve.add_tri (bounds_n_index + vec3 (0, 1, 1), bounds_n_index + vec3 (0, 0, 1), bounds_n_index + vec3 (1, 1, 1), c);
			}
			if (back) {
				ve.add_tri (bounds_n_index + vec3 (1, 1, 0), bounds_n_index + vec3 (1, 0, 0), bounds_n_index + vec3 (0, 0, 0), c);
				ve.add_tri (bounds_n_index + vec3 (0, 1, 0), bounds_n_index + vec3 (1, 1, 0), bounds_n_index + vec3 (0, 0, 0), c);
			}
		}

		ve.set_position (b.get_position ());

		ve.build ();
	}

	void bloc::set_bounds (vec3 bo) {
		prev_bounds = bounds;
		bounds = bo;
	}
	void bloc::set_position (vec3 po) {
		position = po;
	}

	void bloc::resize (void) {
		if (prev_bounds == bounds)
			return;
	}

}