#ifndef TEXTURE_h
#define TEXTURE_h

#include <GL/glew.h>
#include <string>
#include <vector>

#include "vec.h"

namespace game {
	struct texture_params {
		GLenum format, type;

		GLint mag_func, wrapping, internalformat;
		
		vec2 resolution;

		texture_params (int unit, GLint mag_func, GLint wrapping, GLint internalformat, GLenum format, GLenum type, vec2 resolution);
		texture_params (void);

		int unit;
	};

	class texture {
		friend class renderer;

	public:
		void bind (void);

		void clear (void);

		void sub (vec2 pos, vec2 dim, GLvoid * data);

		const bool get_completed (void) const;
		const GLuint get_id (void) const;
		const texture_params get_params (void) const;

		texture (void);
	   ~texture (void);

		void unbind (void);

	private:
		GLuint id;
		bool is_built;

		texture_params params;

		texture (int unit, GLint mag_func, GLint wrapping, GLint internalformat, GLenum format, GLenum type, vec2 resolution, const GLvoid * data, int unpack, std::vector <int> * v_);
		texture (int unit, GLint mag_func, GLint wrapping, GLint internalformat, std::string name, int unpack, std::vector <int> * v_);

		std::vector <int> * v_;
	};
}

#endif