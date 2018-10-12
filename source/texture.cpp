#include <fstream>
#include <iostream>
#include <png.h>

#include "texture.h"

namespace game {
	texture_params::texture_params (void) {
	}
	texture_params::texture_params (int unit, GLint mag_func, GLint wrapping, GLint internalformat, GLenum format, GLenum type, vec2 resolution) : unit (unit), mag_func (mag_func), wrapping (wrapping), internalformat (internalformat), format (format), type (type), resolution (resolution) {
	}

	void sread (png_structp pngptr, png_bytep data, png_size_t length) {
		png_voidp a = png_get_io_ptr (pngptr);
		((std::ifstream *) a) -> read ((char*) data, length);
	}

	texture::texture (void) : id (NULL), v_ (NULL), is_built (false) {
	}
	texture::texture (int unit, GLint mag_func, GLint wrapping, GLint internalformat, GLenum format, GLenum type, vec2 resolution, const GLvoid * data, int unpack, std::vector <int> * v_) : params (texture_params (unit, mag_func, wrapping, internalformat, format, type, resolution)), id (NULL), v_ (v_), is_built (false) {
		glGenTextures (1, &id);

		if (params.unit > -1)
			glActiveTexture (GL_TEXTURE0 + params.unit);

		glBindTexture (GL_TEXTURE_2D, id);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.mag_func);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.mag_func);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapping);
		glPixelStorei (GL_UNPACK_ALIGNMENT, unpack > 0 ? unpack : 4);
		glTexImage2D (GL_TEXTURE_2D, 0, params.internalformat, params.resolution.x, params.resolution.y, 0, params.format, params.type, data);

		is_built = true;
	}

	texture::texture (int unit, GLint mag_func, GLint wrapping, GLint internalformat, std::string name, int unpack, std::vector <int> * v_) : id (NULL), v_ (v_) {
		png_byte header [8];

		std::ifstream s ("files\\assets\\images\\" + name + ".png", std::ios::in | std::ios::binary);
		if (! s.is_open ()) {
			std::cerr << "Error in texture '" << name << "': File 'files\\assets\\images\\" << name << ".png" << "' could not be found" << std::endl;
			return;
		}

		s.read ((char *) header, 8);

		if (png_sig_cmp (header, 0, 8)) {
			std::cerr << "Error in texture '" << name << "': File 'files\\assets\\images\\" << name << ".png" << "' is not a PNG file" << std::endl;
			return;
		}

		png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (! png_ptr) {
			std::cerr << "Error in texture '" << name << "': Function png_create_read_struct returned a NULL pointer" << std::endl;
			return;
		}
		png_infop info_ptr = png_create_info_struct (png_ptr);
		if (! info_ptr) {
			std::cerr << "Error in texture '" << name << "': Function png_create_info_struct returned a NULL pointer" << std::endl;
			png_destroy_read_struct (& png_ptr, (png_infopp) NULL, (png_infopp) NULL);
			return;
		}
		if (setjmp (png_jmpbuf (png_ptr))) {
			std::cerr << "Error in texture '" << name << "': libpng encountered an error - refer above" << std::endl;
			png_destroy_read_struct (& png_ptr, & info_ptr, (png_infopp) NULL);
			return;
		}
		png_set_read_fn (png_ptr, (png_voidp) & s, sread);
		png_set_sig_bytes (png_ptr, 8);
		png_read_info (png_ptr, info_ptr);

		int bit_depth, color_type;
		png_uint_32 width, height;
		GLint png_format;

		png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
					  NULL, NULL, NULL);

		if (bit_depth != 8) {
			std::cerr << "Error in texture '" << name << "': PNG 'files\\assets\\images\\" << name << ".png' has an unsupported bit-depth" << std::endl;
			png_destroy_read_struct (& png_ptr, & info_ptr, (png_infopp) NULL);
			return;
		}

		switch (color_type) {
		case PNG_COLOR_TYPE_RGB:
			png_format = GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			png_format = GL_RGBA;
			break;
		default:
			std::cerr << "Error in texture '" << name << "': PNG 'files\\assets\\images\\" << name << ".png' has an unknown color type" << std::endl;
			png_destroy_read_struct (& png_ptr, & info_ptr, (png_infopp) NULL);
			return;
		}

		png_read_update_info (png_ptr, info_ptr);

		int row_bytes = png_get_rowbytes (png_ptr, info_ptr);

		row_bytes += 3 - ((row_bytes - 1) % 4);

		png_byte * image_data = new png_byte [row_bytes * height * sizeof (png_byte) + 15];
		png_byte ** row_pointers = new png_byte * [height * sizeof (png_byte *)];

		for (unsigned int it = 0; it < height; ++ it) {
			row_pointers [it] = image_data + it * row_bytes;
		}

		png_read_image (png_ptr, row_pointers);
		
		png_destroy_read_struct (& png_ptr, & info_ptr, (png_infopp) NULL);
		delete [] row_pointers;

		params = texture_params (unit, mag_func, wrapping, internalformat, png_format, GL_UNSIGNED_BYTE, vec2 ((int) width, (int) height));
		glGenTextures (1, &id);

		if (params.unit > -1)
			glActiveTexture (GL_TEXTURE0 + params.unit);

		glBindTexture (GL_TEXTURE_2D, id);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.mag_func);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.mag_func);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapping);
		glPixelStorei (GL_UNPACK_ALIGNMENT, unpack > 0 ? unpack : 4);
		glTexImage2D (GL_TEXTURE_2D, 0, params.internalformat, params.resolution.x, params.resolution.y, 0, params.format, params.type, image_data);

		delete [] image_data;
		
		is_built = true;
	}
	texture::~texture (void) {
	}

	void texture::sub (vec2 pos, vec2 dim, GLvoid * data) {
		if (! is_built)
			return;

		glBindTexture (GL_TEXTURE_2D, id);
		glTexSubImage2D (GL_TEXTURE_2D, 0, pos.x, pos.y, dim.x, dim.y, params.format, params.type, data);
	}

	void texture::bind (void) {
		glBindTexture (GL_TEXTURE_2D, id);
	}
	void texture::unbind (void) {
		glBindTexture (GL_TEXTURE_2D, 0);
	}
	void texture::clear (void) {
		if (! is_built)
			return;

		unbind ();
		glDeleteTextures (1, &id);
		if (params.unit > -1 && v_ != NULL && v_ -> size () > 0) {
			for (auto it = v_ -> begin (); it != v_ -> end (); ++ it) {
				if (* it == params.unit) {
					v_ -> erase (it);
					break;
				}
			}
		}
		v_ = NULL;
		is_built = false;
	}

	const GLuint texture::get_id (void) const {
		return id;
	}
	const bool texture::get_completed (void) const {
		return is_built;
	}
	const texture_params texture::get_params (void) const {
		return params;
	}
}