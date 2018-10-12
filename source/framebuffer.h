#ifndef FRAMEBUFFER_h
#define FRAMEBUFFER_h

#include <GL/glew.h>

#include "texture.h"

#include <vector>
#include "vec.h"

namespace game {
	class renderer;

	class framebuffer {
		friend class renderer;
	public:
		void add_texture (texture t);

		void bind (void);

		void clear (void);

		void finalize (void);

		framebuffer (void);
	   ~framebuffer (void);

		void unbind (void);

	private:
		GLuint depth_id;

		framebuffer (vec2 resolution);
		GLuint framebuffer_id;

		bool is_built;

		vec2 resolution;

		std::vector <texture> textures;
	};
}

#endif