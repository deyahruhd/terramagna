#ifndef MODEL_h
#define MODEL_h

#include "bloc.h"

#include <memory>

#include <unordered_map>

#include "vertexbuffer.h"

namespace game {
	class renderer;

	#pragma region Animation Declarations

	struct simple_bone { // Only describes a name, rotation and offset of a bone. Mainly for usage with the animation system
		simple_bone (void);
		simple_bone (quat orientation, vec3 effect, std::string name);

		quat orientation;
		vec3 offset;

		std::string name;
	};

	class keyframe {
	public:
		simple_bone get_bone (std::string name);
		int get_num_bones (void);

		bool has_bone (std::string name);

		keyframe (std::vector <simple_bone> bones);

	private:
		std::vector <simple_bone> bones;
	};

	class anim {
		friend class renderer;
	public:
		anim (void);

		int get_frame_length (void);
		keyframe get_keyframe (int frame);

	private:
		anim (std::vector <keyframe> keyframes);
		int frame_length;

		std::unordered_map <int, keyframe> keyframes;
	};

	class bone {
		friend class model;

	public:
		bone * add_child (quat orientation, vec3 offset, std::string name); // Adds a bone to the children of this bone, and returns a pointer to it.
		bone * add_child (simple_bone bone); // Adds a bone to the children of this bone, and returns a pointer to it.
		void apply_animation (keyframe k); // Applies the animation to the bone.
		void attach_bloc (std::string name); // Connects a bloc onto this bone for rendering.

		bone (void); // Empty constructor, do not use!
		~bone (void);

		void draw (mat4 loc_mat); // Draws the bone.

		bloc * get_bloc (void); // Returns a reference to the attached block, if any exist.
		const quat get_orientation (void) const; // Returns the orientation factor of the bone.
		const vec3 get_offset (void) const; // Returns the offset of the bone.

		void set_orientation (quat orientation); // Sets the orientation of the bone.
		void set_offset (vec3 offset); // Sets the offset of the bone.

	private:
		bone (quat orientation, vec3 offset, renderer * re, std::string name);
		bone (simple_bone bone, renderer * re);
		vertexbuffer buffer;

		std::vector <std::unique_ptr <bone>> children;

		bloc data;

		std::string name;

		quat orientation;

		vec3 offset;

		renderer * re;
	};

	#pragma endregion

	class model {
		friend class renderer;

	public:
		bone * add_child (quat orientation, vec3 offset, std::string name);
		bone * add_child (simple_bone bone);

		void draw (void);
		
		const quat get_orientation (void) const;
		const vec3 get_position (void) const;

		model (void);
	   ~model (void);
		
		void set_orientation (quat orientation);
		void set_position (vec3 offset);

		void apply_animation (keyframe k); // Applies the keyframe to the model.

	private:
		std::vector <std::unique_ptr <bone>> children;

		model (quat orientation, vec3 position, renderer * re);

		quat orientation;

		vec3 position;

		renderer * re;
	};
}

#endif
