#include <fstream>
#include <iostream>
#include <string>

#include "shader.h"

namespace game {
GLuint compile_shader (GLenum type, const std::string name) {
	glewInit ();
	std::ifstream s ("files\\assets\\shaders\\" + name);

	if (! s.is_open ()) {
		return NULL;
	}

	std::string line, buffer;
	while (s.good ()) {
		getline (s, line);
		buffer += line;
		buffer += '\n';
	}
	buffer += '\0';

	GLuint shader = glCreateShader (type);
	const char * source_cstr = buffer.c_str ();
	glShaderSource (shader, 1, & source_cstr, NULL);
	glCompileShader (shader);

	GLint status;
	glGetShaderiv (shader, GL_COMPILE_STATUS, & status);
	if (status == GL_FALSE) {
		GLint loglength;
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, & loglength);

		GLchar * log = new GLchar [loglength];
		glGetShaderInfoLog (shader, loglength, NULL, log);

		std::cout << log << std::endl;
		delete [] log;
	}
	return shader;
}

GLuint compile_program (GLuint vertex_shader, GLuint fragment_shader) {
	glewInit ();
	if (vertex_shader == NULL || fragment_shader == NULL)
		return NULL;
	
	GLuint program = glCreateProgram ();
	glAttachShader (program, vertex_shader);
	glAttachShader (program, fragment_shader);
	glLinkProgram (program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, & status);
	if (status == GL_FALSE) {
		GLint loglength;
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, & loglength);

		GLchar * log = new GLchar [loglength];
		glGetProgramInfoLog (program, loglength, NULL, log);

		std::cout << log << std::endl;
		delete [] log;
	}

	glDetachShader (program, vertex_shader);
	glDetachShader (program, fragment_shader);
	glDeleteShader (vertex_shader);
	glDeleteShader (fragment_shader);

	return program;
}
	shader::shader (void) {
	}
	shader::shader (std::string name, std::string vertex_shader, std::string fragment_shader) : shader_name (name), id (0) {
		id = compile_program (compile_shader (GL_VERTEX_SHADER, vertex_shader), 
							  compile_shader (GL_FRAGMENT_SHADER, fragment_shader));
	}
	shader::~shader (void) {
	}

	void shader::bind (void) {
		glUseProgram (id);
	}
	void shader::unbind (void) {
		glUseProgram (NULL);
	}

	void shader::clear (void) {
		unbind ();
		glDeleteProgram (id);
	}
	
	void shader::define_uniform (std::string name, uniform::uniform_type type) {
		const char * str = name.c_str ();
		GLint loc = glGetUniformLocation (id, str);
		if (loc == -1) {
			std::cout << "Error in shader '" << shader_name << "': Uniform variable '" << name << "' could not be resolved" << std::endl;
			return;
		}

		uniforms.push_back (uniform (type, loc, name));
	}

	void shader::set_uniform (std::string name, int i) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		if (u.type != uniform::int1) {
			std::cout << "Error in shader '" << shader_name << "': Function type '1i' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl;
			return;
		}

		bind ();
		glUniform1i (u.location, i);
	}
	void shader::set_uniform (std::string name, float f) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		if (u.type != uniform::float1) {
			std::cout << "Error in shader '" << shader_name << "': Function type '1f' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl;
			return;
		}

		bind ();
		glUniform1f (u.location, f);
	}
	void shader::set_uniform (std::string name, vec2 ve) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		bind ();
		switch (u.type) {
		case uniform::vec_int2: glUniform2i (u.location, (int) ve.x, (int) ve.y); break;
		case uniform::vec_float2: glUniform2f (u.location, (float) ve.x, (float) ve.y); break;
		default: std::cout << "Error in shader '" << shader_name << "': Function type 'vec2' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl; break;
		}
	}
	void shader::set_uniform (std::string name, vec3 ve) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		bind ();
		switch (u.type) {
		case uniform::vec_int3: glUniform3i (u.location, (int) ve.x, (int) ve.y, (int) ve.z); break;
		case uniform::vec_float3: glUniform3f (u.location, (float) ve.x, (float) ve.y, (float) ve.z); break;
		default: std::cout << "Error in shader '" << shader_name << "': Function type 'vec3' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl; break;
		}
	}

	void shader::set_uniform_mat2 (std::string name, bool transpose, float * m) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		if (u.type != uniform::mat2) {
			std::cout << "Error in shader '" << shader_name << "': Function type 'mat2' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl;
			return;
		}

		bind ();
		glUniformMatrix2fv (u.location, 1, transpose, m);
	}
	void shader::set_uniform_mat3 (std::string name, bool transpose, float * m) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		if (u.type != uniform::mat3) {
			std::cout << "Error in shader '" << shader_name << "': Function type 'mat3' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl;
			return;
		}

		bind ();
		glUniformMatrix3fv (u.location, 1, transpose, m);
	}
	void shader::set_uniform_mat4 (std::string name, bool transpose, float * m) {
		uniform u = get_uniform (name);
		if (u.location == -1)
			return;

		if (u.type != uniform::mat4) {
			std::cout << "Error in shader '" << shader_name << "': Function type 'mat4' conflicts with supplied uniform type of uniform '" << name << "'" << std::endl;
			return;
		}

		bind ();
		glUniformMatrix4fv (u.location, 1, transpose, m);
	}

	uniform shader::get_uniform (std::string name) {
		for (auto it = uniforms.begin (); it != uniforms.end (); ++ it) {
			if (it -> name == name)
				return *it;
		}

		return uniform (uniform::int1, -1, "error");
	}

	uniform::uniform (uniform_type type, GLint location, std::string name) : type (type), location (location), name (name) {
	}
}