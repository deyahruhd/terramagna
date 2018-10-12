#ifndef RENDERER_h
#define RENDERER_h

#include "camera.h"

#include "font.h"
#include "framebuffer.h"

#include <ft2build.h>
#include <freetype.h>

#include <GL/glew.h>

#include "mat4.h"
#include "model.h"

#include "shader.h"

#include "texture.h"

#include "vec.h"
#include "vertexbuffer.h"

namespace game {
	class font;
	class framebuffer;

	class io;

	class vertexbuffer;

	class renderer {
		friend int start (void);
	public:
		void bind_fbo (void);

		void clear_fbo (void);
		font create_font (std::string name, int size);
		model create_model (quat orientation, vec3 position);
		shader & create_shader (std::string name, std::string vertex, std::string fragment);
		texture create_texture (bool renderable, GLint mag_func, GLint wrapping, GLint internalformat, int unpack, std::string name);
		texture create_texture (bool renderable, GLint mag_func, GLint wrapping, GLint internalformat, GLenum format, GLenum type, vec2 resolution, int unpack, const GLvoid * data);
		vertexbuffer create_vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos);
		vertexbuffer create_vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos, quat orientation);
		vertexbuffer * create_vertexbuffer_ptr (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos);
		vertexbuffer * create_vertexbuffer_ptr (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos, quat orientation);

		camera & get_camera (void);
		const double get_frametime (void) const;
		const FT_Library get_freetypelib (void) const;
		shader & get_shader (std::string name);
		const mat4 get_model_mat4 (void) const;
		const mat4 get_modelinv_mat4 (void) const;
		const mat4 get_projection_mat4 (void) const;
		const mat4 get_projectioninv_mat4 (void) const;
		const vec2 get_resolution (void) const;
		
		void move_camera (vec3 movement, quat rotation);

		void set_frametime (double frametime);
		void set_model_mat4 (mat4 m);

		void update (void);
		void unbind_fbo (void);

		vec3 cam_movement_REMOVE;
	private:

		framebuffer buffer;

		camera cam;

		double frametime;
		FT_Library ft;

		mat4 model_mat;
		mat4 model_inverse;

		mat4 projection;
		mat4 projection_inverse;

		renderer (void);
		renderer (vec2 resolution);
	   ~renderer (void);
		vec2 resolution;

		std::unordered_map <std::string, shader> shaders;

		std::vector <int> texture_units;
	};
}

#endif