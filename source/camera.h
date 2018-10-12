#ifndef CAMERA_h
#define CAMERA_h

#include "mat4.h"
#include "quat.h"

namespace game {
	class camera {
	public:
		camera (void);
		camera (vec3 position);
		camera (vec3 position, quat orientation);
		camera (vec3 position, vec3 offset);
		camera (vec3 position, quat orientation, vec3 offset);

		mat4 get_camera_matrix (void);
		const quat get_orientation (void) const;
		const vec3 get_position (void) const;

		void set_orientation (quat orientation);
		void set_position (vec3 position);

		void update (double frametime);

	private:
		quat orientation, s_orientation;
		vec3 position, s_position;
		vec3 camera_offset, s_camera_offset;
	};
}

#endif