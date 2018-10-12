#include <fstream>

#include <GL/glew.h>

#include <iostream>

#include <string>

#include "renderer.h"

#define ZNEAR 0.01
#define ZFAR 1028.0
#define TEXTURE_LIMIT 32

namespace game {
	renderer::renderer (void) {
	}
	renderer::renderer (vec2 resolution) : resolution (resolution), buffer (resolution), frametime (0.0), ft (NULL) {
		glewInit ();	
		FT_Init_FreeType (& ft);

		cam = camera (vec3 (0, 0, 0), vec3 (0, 0, -5));

		texture color  = create_texture (true, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, resolution, -1, NULL);
		texture normal = create_texture (true, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, resolution, -1, NULL);
		texture depth  = create_texture (true, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_R32F, GL_RED, GL_FLOAT, resolution, -1, NULL);
		buffer.add_texture (color);
		buffer.add_texture (normal);
		buffer.add_texture (depth);
		
		buffer.finalize ();

		shader & gbo       = create_shader ("Geometry Buffer Populator", "fbo_shader.vert", "fbo_shader.frag");
		shader & out       = create_shader ("Output Shader", "out_shader.vert", "out_shader.frag");
		shader & text      = create_shader ("Text Rendering Shader", "text_shader.vert", "text_shader.frag");
		shader & gui_color = create_shader ("Gui Color Rendering Shader", "gui_color_shader.vert", "gui_color_shader.frag");

		gbo.define_uniform ("model", uniform::mat4);
		gbo.define_uniform ("camera", uniform::mat4);
		gbo.define_uniform ("projection", uniform::mat4);
		gbo.define_uniform ("zFar", uniform::float1);

		out.define_uniform ("diffuse", uniform::int1);
		out.define_uniform ("normal", uniform::int1);
		
		out.define_uniform ("depth", uniform::int1);
		out.define_uniform ("projection", uniform::mat4);
		out.define_uniform ("resolution", uniform::vec_float2);
		text.define_uniform ("txt", uniform::int1);
		text.define_uniform ("txt_color", uniform::vec_float3);
		text.define_uniform ("txt_alpha", uniform::float1);
		text.define_uniform ("resolution", uniform::vec_float2);

		for (int it = 0; it < 16; ++ it) {
			projection.at (it) = 0;
		}

		projection.at (0, 0) = 1.0 / (resolution.x / resolution.y);
		projection.at (1, 1) = 1.0;
		projection.at (2, 2) = (ZFAR + ZNEAR) / (ZNEAR - ZFAR);
		projection.at (2, 3) = (2 * ZFAR * ZNEAR) / (ZNEAR - ZFAR);
		projection.at (3, 2) = -1.0;

		projection_inverse = projection.inverse ();
		std::array <float, 16> projection_float = projection.as_float ();

		gbo.set_uniform_mat4 ("camera", GL_TRUE, cam.get_camera_matrix ().as_float ().data ());
		gbo.set_uniform_mat4 ("projection", GL_TRUE, projection_float.data ());
		gbo.set_uniform ("zFar", (float) ZFAR);

		out.set_uniform_mat4 ("projection", GL_TRUE, projection_float.data ());

		out.set_uniform ("diffuse", color.params.unit);
		out.set_uniform ("normal", normal.params.unit);
		out.set_uniform ("depth", depth.params.unit);

		text.set_uniform ("resolution", resolution); 
	}
	renderer::~renderer (void) {
		for (auto it = shaders.begin (); it != shaders.end (); ++ it) {
			it -> second.clear ();
		}

		FT_Done_FreeType (ft);
	}

	vertexbuffer renderer::create_vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos) {
		return vertexbuffer (type, mode, scene, this, pos);
	}
	vertexbuffer renderer::create_vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos, quat orientation) {
		return vertexbuffer (type, mode, scene, this, pos);
	}
	vertexbuffer * renderer::create_vertexbuffer_ptr (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos) {
		return new vertexbuffer (type, mode, scene, this, pos);
	}
	vertexbuffer * renderer::create_vertexbuffer_ptr (vertexbuffer_type type, GLenum mode, bool scene, vec3 pos, quat orientation) {
		return new vertexbuffer (type, mode, scene, this, pos);
	}

	texture renderer::create_texture (bool renderable, GLint mag_func, GLint wrapping, GLint internalformat, GLenum format, GLenum type, vec2 resolution, int unpack, const GLvoid * data) {
		if (! renderable)
			return texture (-1, mag_func, wrapping, internalformat, format, type, resolution, data, unpack, & texture_units);

		for (int it = 0; it < TEXTURE_LIMIT; ++ it) {
			bool found = false;
			for (auto it1 = texture_units.begin (); it1 != texture_units.end (); ++ it1) {
				if (*it1 == it)
					found = true;
			}
			if (! found) {
				texture_units.push_back (it);
				return texture (it, mag_func, wrapping, internalformat, format, type, resolution, data, unpack, & texture_units);
			}
		}

		std::cout << "Error in renderer: A texture failed to be created due to all available texture units being bound" << std::endl;
		return texture ();
	}
	texture renderer::create_texture (bool renderable, GLint mag_func, GLint wrapping, GLint internalformat, int unpack, std::string name) {
		if (! renderable)
			return texture (-1, mag_func, wrapping, internalformat, name, unpack, & texture_units);

		for (int it = 0; it < TEXTURE_LIMIT; ++ it) {
			bool found = false;
			for (auto it1 = texture_units.begin (); it1 != texture_units.end (); ++ it1) {
				if (*it1 == it)
					found = true;
			}
			if (! found) {
				texture_units.push_back (it);
				return texture (it, mag_func, wrapping, internalformat, name, unpack, & texture_units);
			}
		}

		std::cout << "Error in renderer: A texture failed to be created due to all texture units being bound" << std::endl;
		return texture ();
	}
	font renderer::create_font (std::string name, int size) {
		return font (name, size, * this);
	}
	shader & renderer::create_shader (std::string name, std::string vertex_shader, std::string fragment_shader) {
		shaders.insert (std::pair <std::string, shader> (name, shader (name, vertex_shader, fragment_shader)));

		return get_shader (name);
	}
	model renderer::create_model (quat orientation, vec3 position) {
		return model (orientation, position, this);
	}
	void renderer::set_model_mat4 (mat4 mx) {
		model_mat = mx;
		model_inverse = model_mat.inverse ();

		get_shader ("Geometry Buffer Populator").set_uniform_mat4 ("model", GL_TRUE, model_mat.as_float ().data ());
	}

	camera & renderer::get_camera (void) {
		return cam;
	}
	void renderer::move_camera (vec3 movement, quat rotation) {
		cam.set_orientation (rotation);

		vec3 front = (rotation * vec3 (0, 0, 1)).normalized ();
		vec3 up = vec3 (0, 1, 0);
		vec3 right = front.cross (up).normalized ();
		vec3 append = ((front * movement.z) + (right * movement.x) + (up * movement.y));

		cam_movement_REMOVE = append;

		cam.set_position (cam.get_position () + append);

		get_shader ("Geometry Buffer Populator").set_uniform_mat4 ("camera", GL_TRUE, cam.get_camera_matrix ().as_float ().data ());
	}

	void renderer::bind_fbo (void) {
		buffer.bind ();
	}
	void renderer::unbind_fbo (void) {
		buffer.unbind ();
	}
	void renderer::clear_fbo (void) {
		buffer.clear ();
	}

	const double renderer::get_frametime (void) const {
		return frametime;
	}
	const FT_Library renderer::get_freetypelib (void) const {
		return ft;
	}
	const mat4 renderer::get_model_mat4 (void) const {
		return model_mat;
	}
	const mat4 renderer::get_modelinv_mat4 (void) const {
		return model_inverse;
	}
	const mat4 renderer::get_projection_mat4 (void) const {
		return projection;
	}
	const mat4 renderer::get_projectioninv_mat4 (void) const {
		return projection_inverse;
	}
	shader & renderer::get_shader (std::string name) {
		return shaders.at (name);
	}
	const vec2 renderer::get_resolution (void) const {
		return resolution;
	}

	void renderer::set_frametime (double ftime) {
		if (ftime > 0)
			frametime = ftime;
	}

	void renderer::update (void) {
		cam.update (frametime);
	}
}