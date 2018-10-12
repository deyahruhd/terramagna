#include "file.h"

#include "model.h"

#include "renderer.h"

namespace game {
	simple_bone::simple_bone (void) : orientation (), offset (), name () {
	}
	simple_bone::simple_bone (quat orientation, vec3 offset, std::string name) : orientation (orientation), offset (offset), name (name) {
	}

	simple_bone keyframe::get_bone (std::string name) {
		for (auto it = bones.begin (); it != bones.end (); ++ it) {
			if ((* it).name == name)
				return (* it);
		}

		return simple_bone ();
	}
	int keyframe::get_num_bones (void) {
		return bones.size ();
	}

	bool keyframe::has_bone (std::string name) {
		for (auto it = bones.begin (); it != bones.end (); ++ it) {
			if ((* it).name == name)
				return true;
		}

		return false;
	}
	
	anim::anim (void) : frame_length (0) {
	}
	anim::anim (std::vector <keyframe> keyframes_v) : frame_length (keyframes_v.size ()) {
		int id = 0;

		for (auto i = keyframes_v.begin (); i != keyframes_v.end (); ++ i) {
			keyframes.insert (std::pair <int, keyframe> (id, * i));
			++ id;
		}
	}

	int anim::get_frame_length (void) {
		return frame_length;
	}
	keyframe anim::get_keyframe (int frame) {
		return keyframes.at (frame);
	}

	bone * bone::add_child (quat orientation, vec3 offset, std::string name) {
		if (re == NULL)
			return NULL;

		bone * ptr = new bone (orientation, offset, re, name);

		children.push_back (std::unique_ptr <bone> (ptr));

		return ptr;
	}

	bone * bone::add_child (simple_bone smpl_bone) {
		if (re == NULL)
			return NULL;

		bone * ptr = new bone (smpl_bone, re);

		children.push_back (std::unique_ptr <bone> (ptr));
		  
		return ptr;
	}

	void bone::attach_bloc (std::string name) {
		io::open ("files\\assets\\models\\blocs\\" + name, std::ios::in);

		io::read_bloc (data);

		io::close ();

		buffer = re -> create_vertexbuffer (vertexbuffer_color, GL_TRIANGLES, true, vec3 ());

		populate (buffer, data);
	}

	bone::bone (void) : orientation (), offset (0), re (NULL), data (), name () {
	}
	bone::bone (quat orientation, vec3 offset, renderer * re, std::string name) : orientation (orientation), offset (offset), re (re), data (), name (name) {
	}
	bone::bone (simple_bone bone, renderer * re) : orientation (bone.orientation), offset (bone.offset), re (re), data (), name (bone.name) {
	}

	bone::~bone (void) {
		re = NULL;

		children.clear ();
	}

	void bone::draw (mat4 loc_mat) {
		if (re == NULL)
			return;

		quat n_orientation = orientation;
		n_orientation.rotate (vec3 (0, 1, 0), tau / 2.0, true);

		mat4 orient_mat = mat4 (n_orientation);
		mat4 transl_mat = mat4 (1.0);

		transl_mat.at (0, 3) = 0.5 * offset.x;
		transl_mat.at (1, 3) = 0.5 * offset.y;
		transl_mat.at (2, 3) = 0.5 * data.get_bounds ().z;

		mat4 buf_mat = orient_mat * transl_mat;

		buffer.draw (loc_mat * buf_mat);

		transl_mat.at (0, 3) = offset.x;
		transl_mat.at (1, 3) = offset.y;
		transl_mat.at (2, 3) = offset.z;

		orient_mat *= transl_mat;

		for (auto it = children.begin (); it != children.end (); ++ it) {
			(* it) -> draw (loc_mat * orient_mat);
		}
	}

	bloc * bone::get_bloc (void) {
		if (data.is_null ())
			return NULL;

		return & data;
	}
	const quat bone::get_orientation (void) const {
		return orientation;
	}
	const vec3 bone::get_offset (void) const {
		return offset;
	}

	void bone::set_orientation (quat or) {
		orientation = or;
	}
	void bone::set_offset (vec3 of) {
		offset = of;
	}
	void bone::apply_animation (keyframe k) {
		if (k.has_bone (name)) {
			simple_bone b = k.get_bone (name);

			orientation = b.orientation;
			offset = b.offset;
		}

		for (auto it = children.begin (); it != children.end (); ++ it) {
			(* it) -> apply_animation (k);
		}
	}

	model::model (void) : orientation (), position (0), re (NULL) {
	}
	model::model (quat orientation, vec3 position, renderer * re) : orientation (orientation), position (position), re (re) {
	}
	model::~model (void) {
		re = NULL;

		children.clear ();
	}

	bone * model::add_child (quat orientation, vec3 offset, std::string name) {
		if (re == NULL)
			return NULL;

		bone * ptr = new bone (orientation, offset, re, name);

		children.push_back (std::unique_ptr <bone> (ptr));

		return ptr;
	}
	bone * model::add_child (simple_bone smpl_bone) {
		if (re == NULL)
			return NULL;

		bone * ptr = new bone (smpl_bone, re);

		children.push_back (std::unique_ptr <bone> (ptr));

		return ptr;
	}

	void model::draw (void) {
		if (re == NULL)
			return;

		mat4 new_mat = mat4 (orientation);

		new_mat.at (0, 3) = position.x;
		new_mat.at (1, 3) = position.y;
		new_mat.at (2, 3) = position.z;

		for (auto it = children.begin (); it != children.end (); ++ it) {
			(* it) -> draw (new_mat);
		}
	}

	const quat model::get_orientation (void) const {
		return orientation;
	}
	const vec3 model::get_position (void) const {
		return position;
	}

	void model::set_orientation (quat or) {
		orientation = or;
	}
	void model::set_position (vec3 po) {
		position = po;
	}

	void model::apply_animation (keyframe k) {
		for (auto it = children.begin (); it != children.end (); ++ it) {
			(* it) -> apply_animation (k);
		}
	}
}