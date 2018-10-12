#include "camera.h"

#include <iostream>

#define camera_smooth 32

namespace game
{
	camera::camera (void) : position (), orientation (), s_position (), s_orientation () {
		orientation.rotate (vec3 (0, 1, 0), tau / 2.0, false);
	}
	camera::camera (vec3 position) : position (position), orientation (), s_position (position), s_orientation () {}
	camera::camera (vec3 position, quat orientation) : position (position), orientation (orientation), s_position (position), s_orientation (orientation) {}

	camera::camera (vec3 position, vec3 offset) : position (position), orientation (), s_position (position), s_orientation (), camera_offset (offset), s_camera_offset (offset) {}
	camera::camera (vec3 position, quat orientation, vec3 offset) : position (position), orientation (orientation), s_position (position), s_orientation (orientation), camera_offset (offset), s_camera_offset (offset) {}

	const quat camera::get_orientation (void) const {
		return orientation;
	}
	const vec3 camera::get_position (void) const {
		return position;
	}

	void camera::set_orientation (quat o) {
		orientation = o;
	}
	void camera::set_position (vec3 p) {
		position = p;
	}

	mat4 camera::get_camera_matrix (void) {
		mat4 m = mat4 ();
		m.at (0, 3) = s_position.x;
		m.at (1, 3) = s_position.y;
		m.at (2, 3) = s_position.z;

		mat4 offset = mat4 ();
		offset.at (0, 3) = s_camera_offset.x;
		offset.at (1, 3) = s_camera_offset.y;
		offset.at (2, 3) = s_camera_offset.z;

		return offset * mat4 (s_orientation) * m;
	}
	void camera::update (double frametime) {
		s_position = vec_lerp (s_position, position, camera_smooth * frametime);
		s_orientation = quat_lerp (s_orientation, orientation, camera_smooth * frametime);
		s_camera_offset = vec_lerp (s_camera_offset, camera_offset, camera_smooth * frametime);
	}
}