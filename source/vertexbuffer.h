#ifndef VERTEXBUFFER_h
#define VERTEXBUFFER_h

#include <array>
#include <GL/glew.h>
#include <vector>

#include "color.h"
#include "mat4.h"
#include "quat.h"

namespace game {
	class renderer; 

	enum vertexbuffer_type {
		vertexbuffer_color,
		vertexbuffer_texture
	};

	struct vertexbuffer_params {
		GLenum mode;

		vertexbuffer_params (void);
		vertexbuffer_params (vertexbuffer_type type, GLenum mode, bool scene);

		bool scene;

		vertexbuffer_type type;
	};

	class vertexbuffer {
		friend class renderer;
	public:
		void add_point (vec3 v1, color c);
		void add_point (vec3 v1, vec2 t1);
		void add_line (vec3 v1, vec3 v2, color c);
		void add_line (vec3 v1, vec3 v2, vec2 t1, vec2 t2);
		void add_tri (vec3 v1, vec3 v2, vec3 v3,
					  color c);
		void add_tri (vec3 v1, vec3 v2, vec3 v3,
					  vec2 t1, vec2 t2, vec2 t3);

		void build (void);

		void clear (void);

		void  draw (void);
		void  draw (mat4 obj_mesh);

		const bool get_drawstate (void) const;
		const quat get_orientation (void) const;
		const vertexbuffer_params get_params (void) const;
		const vec3 get_position (void) const;

		std::array <GLuint, 3> id;

		void set_orientation (quat orientation);
		void set_position (vec3 position);

		vertexbuffer & operator =  (vertexbuffer & b);

		vertexbuffer (void);
		vertexbuffer (vertexbuffer & b);
	   ~vertexbuffer (void);

	private:
		void clear_app_vertexbuffer (void);
		void clear_gpu_vertexbuffer (void);

		mat4 get_model_matrix (void);

		bool is_built;

		quat orientation;

		vertexbuffer_params params;
		vec3 position;

		renderer * r;

		std::vector <GLubyte> v_col; 
		std::vector <GLfloat> v_nrm;
		std::vector <GLfloat> v_pos;
		std::vector <GLfloat> v_tex;
		int vert_count, gpu_vert_count;
		vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, renderer * r, vec3 position); 
		vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, renderer * r, vec3 position, quat orientation); 
	};


	#pragma region Helper Functions

	void create_grid (vertexbuffer & v);

	#pragma endregion
}

#endif