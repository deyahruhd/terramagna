#include "renderer.h"
#include "vertexbuffer.h"

namespace game {
	vertexbuffer_params::vertexbuffer_params (void) {
	}
	vertexbuffer_params::vertexbuffer_params (vertexbuffer_type type, GLenum mode, bool scene) : type (type), mode (mode), scene (scene) {
	}

	vertexbuffer::vertexbuffer (void) : r (NULL), id (), is_built (false) {
	}
	vertexbuffer::vertexbuffer (vertexbuffer & b) : position (b.position), orientation (b.orientation), 
												   vert_count (0), gpu_vert_count (0),
												   is_built (false), params (b.params), r (b.r), id (b.id) {
		b.clear_app_vertexbuffer ();
		b.gpu_vert_count = 0;
		b.id [0] = NULL;
		b.id [1] = NULL;
		b.id [2] = NULL;
		b.params.scene = false;
		b.orientation = quat ();
		b.position = vec3 ();
		b.is_built = false;
		b.r = NULL;
		
		glewInit ();
	}
	vertexbuffer::vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, renderer * r, vec3 position) : position (position), orientation (quat ()),
																												params (type, mode, scene),
																												vert_count (0), gpu_vert_count (0),
																												is_built (false),  r (r), id () {
		glewInit ();
	}
	vertexbuffer::vertexbuffer (vertexbuffer_type type, GLenum mode, bool scene, renderer * r, vec3 position, quat orientation) : position (position), orientation (orientation),
																																  params (type, mode, scene),
																																  vert_count (0), gpu_vert_count (0),
																																  is_built (false), r (r), id () {
		glewInit ();
	}


	vertexbuffer::~vertexbuffer(void) {
		r = NULL;
		clear ();
	}

	void vertexbuffer::clear_app_vertexbuffer (void) {
		if (! is_built)
			return;

		v_pos.clear ();
		v_col.clear ();
		v_tex.clear ();
		v_nrm.clear ();

		vert_count = 0;
	}

	void vertexbuffer::clear_gpu_vertexbuffer (void) {
		if (! is_built)
			return;

		glDeleteBuffers (3, id.data ());
		gpu_vert_count = 0;
	}

	void vertexbuffer::clear (void) {
		if(! is_built)
			return;

		clear_gpu_vertexbuffer ();
		clear_app_vertexbuffer ();

		is_built = false;
	}
	void vertexbuffer::build (void) {
		if (is_built)
			clear_gpu_vertexbuffer ();
		if (v_pos.size () == 0)
			return;



		glGenBuffers (3, id.data ());

		glBindBuffer (GL_ARRAY_BUFFER, id [0]);
		glBufferData (GL_ARRAY_BUFFER, v_pos.size () * sizeof (GLfloat), & v_pos [0], GL_STATIC_DRAW);

		glBindBuffer (GL_ARRAY_BUFFER, id [2]);
		glBufferData (GL_ARRAY_BUFFER, v_nrm.size () * sizeof (GLfloat), & v_nrm [0], GL_STATIC_DRAW);

		glBindBuffer (GL_ARRAY_BUFFER, id [1]);
		switch (params.type) {
		case vertexbuffer_color:
			glBufferData (GL_ARRAY_BUFFER, v_col.size () * sizeof (GLubyte), & v_col [0], GL_STATIC_DRAW);
			break;
		case vertexbuffer_texture:
			glBufferData (GL_ARRAY_BUFFER, v_tex.size () * sizeof (GLfloat), & v_tex [0], GL_STATIC_DRAW);
			break;
		default:
			break;
		}
		glBindBuffer (GL_ARRAY_BUFFER, NULL);
		is_built = true;

		gpu_vert_count = vert_count;
		clear_app_vertexbuffer ();
	}
	void vertexbuffer::draw (void) {
		if (params.scene && r != NULL)
			r -> set_model_mat4 (get_model_matrix ());

		if (! is_built)
			return;

		glBindBuffer (GL_ARRAY_BUFFER, id [0]);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer (GL_ARRAY_BUFFER, id [1]);
		glEnableVertexAttribArray (1);

		switch (params.type) {
		case vertexbuffer_color:
			glVertexAttribPointer (1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
			break;
		case vertexbuffer_texture:
			glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
			break;
		default:
			break;
		}

		glBindBuffer (GL_ARRAY_BUFFER, id [2]);
		glEnableVertexAttribArray (2);
		glVertexAttribPointer (2, 3, GL_FLOAT, GL_TRUE, 0, 0);

		glDrawArrays (params.mode, 0, gpu_vert_count);

		glBindBuffer (GL_ARRAY_BUFFER, NULL);
	}
	void vertexbuffer::draw (mat4 obj_mat) {
		if (params.scene && r != NULL)
			r -> set_model_mat4 (obj_mat);

		if (! is_built)
			return;

		glBindBuffer (GL_ARRAY_BUFFER, id [0]);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer (GL_ARRAY_BUFFER, id [1]);
		glEnableVertexAttribArray (1);

		switch (params.type) {
		case vertexbuffer_color:
			glVertexAttribPointer (1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
			break;
		case vertexbuffer_texture:
			glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
			break;
		default:
			break;
		}

		glBindBuffer (GL_ARRAY_BUFFER, id [2]);
		glEnableVertexAttribArray (2);
		glVertexAttribPointer (2, 3, GL_FLOAT, GL_TRUE, 0, 0);

		glDrawArrays (params.mode, 0, gpu_vert_count);

		glBindBuffer (GL_ARRAY_BUFFER, NULL);
	}

	void vertexbuffer::add_tri (vec3 v1, vec3 v2, vec3 v3,
						color c) {
		if (params.mode != GL_TRIANGLES || params.type != vertexbuffer_color)
			return;

		vec3 v_norm_1 = (v2 - v1);
		vec3 v_norm_2 = (v3 - v1);
		vec3 v_norm = v_norm_1.cross (v_norm_2).normalized ();

		v_pos.push_back ((GLfloat) v1.x); v_pos.push_back ((GLfloat) v1.y); v_pos.push_back ((GLfloat) v1.z);
		v_col.push_back (c.r ()); v_col.push_back (c.g ()); v_col.push_back (c.b ()); v_col.push_back (c.a ());
		v_nrm.push_back ((GLfloat) v_norm.x); v_nrm.push_back ((GLfloat) v_norm.y); v_nrm.push_back ((GLfloat) v_norm.z); 

		v_pos.push_back ((GLfloat) v2.x); v_pos.push_back ((GLfloat) v2.y); v_pos.push_back ((GLfloat) v2.z);
		v_col.push_back (c.r ()); v_col.push_back (c.g ()); v_col.push_back (c.b ()); v_col.push_back (c.a ());
		v_nrm.push_back ((GLfloat) v_norm.x); v_nrm.push_back ((GLfloat) v_norm.y); v_nrm.push_back ((GLfloat) v_norm.z); 

		v_pos.push_back ((GLfloat) v3.x); v_pos.push_back ((GLfloat) v3.y); v_pos.push_back ((GLfloat) v3.z);
		v_col.push_back (c.r ()); v_col.push_back (c.g ()); v_col.push_back (c.b ()); v_col.push_back (c.a ());
		v_nrm.push_back ((GLfloat) v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		vert_count += 3;
	}
	void vertexbuffer::add_tri (vec3 v1, vec3 v2, vec3 v3,
						vec2 t1, vec2 t2, vec2 t3) {
		if (params.mode != GL_TRIANGLES || params.type != vertexbuffer_texture)
			return;

		vec3 v_norm_1 = (v2 - v1);
		vec3 v_norm_2 = (v3 - v1);
		vec3 v_norm = v_norm_1.cross (v_norm_2).normalized ();

		v_pos.push_back ((GLfloat) v1.x); v_pos.push_back ((GLfloat) v1.y); v_pos.push_back ((GLfloat) v1.z);
		v_tex.push_back ((GLfloat) t1.u); v_tex.push_back ((GLfloat) t1.v);
		v_nrm.push_back ((GLfloat) v_norm.x); v_nrm.push_back ((GLfloat) v_norm.y); v_nrm.push_back ((GLfloat) v_norm.z); 

		v_pos.push_back ((GLfloat) v2.x); v_pos.push_back ((GLfloat) v2.y); v_pos.push_back (v2.z);
		v_tex.push_back ((GLfloat) t2.u); v_tex.push_back ((GLfloat) t2.v);
		v_nrm.push_back ((GLfloat) v_norm.x); v_nrm.push_back ((GLfloat) v_norm.y); v_nrm.push_back ((GLfloat) v_norm.z); 

		v_pos.push_back ((GLfloat) v3.x); v_pos.push_back ((GLfloat) v3.y); v_pos.push_back ((GLfloat) v3.z);
		v_tex.push_back ((GLfloat) t3.u); v_tex.push_back ((GLfloat) t3.v);
		v_nrm.push_back ((GLfloat) v_norm.x); v_nrm.push_back ((GLfloat) v_norm.y); v_nrm.push_back ((GLfloat) v_norm.z); 

		vert_count += 3;
	}
	void vertexbuffer::add_line (vec3 v1, vec3 v2,
						color c) {
		if (params.mode != GL_LINES || params.type != vertexbuffer_color)
			return;

		vec3 v_norm = vec3 (0, 0, 0);

		v_pos.push_back ((GLfloat) v1.x); v_pos.push_back ((GLfloat) v1.y); v_pos.push_back ((GLfloat) v1.z);
		v_col.push_back (c.r ()); v_col.push_back (c.g ()); v_col.push_back (c.b ()); v_col.push_back (c.a ());
		v_nrm.push_back (v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		v_pos.push_back ((GLfloat) v2.x); v_pos.push_back ((GLfloat) v2.y); v_pos.push_back ((GLfloat) v2.z);
		v_col.push_back (c.r ()); v_col.push_back (c.g ()); v_col.push_back (c.b ()); v_col.push_back (c.a ());
		v_nrm.push_back (v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		vert_count += 2;
	}
	void vertexbuffer::add_line (vec3 v1, vec3 v2,
								 vec2 t1, vec2 t2) {
		if (params.mode != GL_LINES || params.type != vertexbuffer_texture)
			return;

		vec3 v_norm = vec3 (0, 0, 0);

		v_pos.push_back ((GLfloat) v1.x); v_pos.push_back ((GLfloat) v1.y); v_pos.push_back ((GLfloat) v1.z);
		v_tex.push_back ((GLfloat) t1.u); v_tex.push_back ((GLfloat) t1.v); 
		v_nrm.push_back (v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		v_pos.push_back ((GLfloat) v2.x); v_pos.push_back ((GLfloat) v2.y); v_pos.push_back ((GLfloat) v2.z);
		v_tex.push_back ((GLfloat) t2.u); v_tex.push_back ((GLfloat) t2.v); 
		v_nrm.push_back (v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		vert_count += 2;
	}
	void vertexbuffer::add_point (vec3 v1,
								  color c) {
		if (params.mode != GL_POINTS || params.type != vertexbuffer_color)
			return;

		vec3 v_norm = vec3 (0, 0, 0);

		v_pos.push_back ((GLfloat) v1.x); v_pos.push_back ((GLfloat) v1.y); v_pos.push_back ((GLfloat) v1.z);
		v_col.push_back (c.r ()); v_col.push_back (c.g ()); v_col.push_back (c.b ()); v_col.push_back (c.a ());
		v_nrm.push_back (v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		vert_count += 1;
	}
	void vertexbuffer::add_point (vec3 v1,
								  vec2 t1) {
		if (params.mode != GL_POINTS || params.type != vertexbuffer_texture)
			return;

		vec3 v_norm = vec3 (0, 0, 0);

		v_pos.push_back ((GLfloat) v1.x); v_pos.push_back ((GLfloat) v1.y); v_pos.push_back ((GLfloat) v1.z);
		v_tex.push_back ((GLfloat) t1.u); v_tex.push_back ((GLfloat) t1.v);
		v_nrm.push_back (v_norm.x); v_nrm.push_back (v_norm.y); v_nrm.push_back (v_norm.z); 

		vert_count += 1;
	}

	mat4 vertexbuffer::get_model_matrix (void) {
		mat4 m = mat4 (orientation);
		m.at (0, 3) = position.x;
		m.at (1, 3) = position.y;
		m.at (2, 3) = position.z;
		
		return m;
	}
	const quat vertexbuffer::get_orientation (void) const {
		return orientation;
	}
	const vertexbuffer_params vertexbuffer::get_params (void) const {
		return params;
	}
	const vec3 vertexbuffer::get_position (void) const {
		return position;
	}
	
	vertexbuffer & vertexbuffer::operator =  (vertexbuffer & b) {
		position = b.position;
		orientation = b.orientation;
		params = b.params;
		vert_count = 0; 
		gpu_vert_count = 0;										   
		is_built = false; 
		r = b.r; 
		id = b.id;

		b.clear_app_vertexbuffer ();
		b.gpu_vert_count = 0;
		b.id [0] = NULL;
		b.id [1] = NULL;
		b.id [2] = NULL;
		b.params.scene = false;
		b.orientation = quat ();
		b.position = vec3 ();
		b.is_built = false;
		return * this;
	}

	const bool vertexbuffer::get_drawstate (void) const {
		return is_built;
	}

	void vertexbuffer::set_position (vec3 pos) {
		position = pos;
	}
	void vertexbuffer::set_orientation (quat or) {
		orientation = or;
	}
	
	void create_grid (vertexbuffer & v) {
		if (v.get_params ().type != vertexbuffer_color || v.get_params ().mode != GL_LINES)
			return;

		v.add_line (vec3 (0), vec3 (1, 0, 0), color (0xffff0000)); // X axis
		v.add_line (vec3 (0), vec3 (0, 1, 0), color (0xff00ff00)); // Y axis
		v.add_line (vec3 (0), vec3 (0, 0, 1), color (0xff0000ff)); // Z axis

		for (int i = 0; i < 17; ++ i) {
			v.add_line (vec3 (i - 8, 0, -8), vec3 (i - 8, 0, 8), color (0xaaffffff));
		}
		for (int i = 0; i < 17; ++ i) {
			v.add_line (vec3 (-8, 0, i - 8), vec3 (8, 0, i - 8), color (0xaaffffff));
		}
	}
}