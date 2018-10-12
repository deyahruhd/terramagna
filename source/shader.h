#ifndef SHADER_h
#define SHADER_h

#include <GL/glew.h>
#include <string>
#include <vector>

#include "vec.h"

namespace game {
	struct uniform {
		enum uniform_type;

		uniform (uniform_type type, GLint location, std::string name);
		enum uniform_type {
			int1, vec_int2, vec_int3, vec_int4,
			float1, vec_float2, vec_float3, vec_float4,
			mat2, mat3, mat4
		};
		uniform_type type;
		GLint location;
		std::string name;
	};

	class shader {
	public:
		shader (void);
		shader (std::string name, std::string vertex_shader, std::string fragment_shader);
	   ~shader (void);

		void bind (void);
		void unbind (void);

		void clear (void);
	
		void define_uniform (std::string name, uniform::uniform_type type);

		void set_uniform (std::string name, int i);
		void set_uniform (std::string name, float f);
		void set_uniform (std::string name, vec2 v);
		void set_uniform (std::string name, vec3 v);

		void set_uniform_mat2 (std::string name, bool transpose, float * f);
		void set_uniform_mat3 (std::string name, bool transpose, float * f);
		void set_uniform_mat4 (std::string name, bool transpose, float * f);
	private:
		uniform get_uniform (std::string name);

		std::vector <uniform> uniforms;

		std::string shader_name;
		GLuint id;
	};
}

#endif