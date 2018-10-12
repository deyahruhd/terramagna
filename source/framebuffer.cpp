#include <iostream>

#include "color.h"
#include "framebuffer.h"

#define CLEAR_COLOR color (argbtoint (255, 0, 0, 255))

GLuint uc [4] = { 0, 0, 0, 255 };
GLint  ic [4] = { 0, 0, 0, 255 };
GLfloat f = 1.f;

namespace game {
	framebuffer::framebuffer (void) {
	}
	framebuffer::framebuffer (vec2 resolution) : resolution (resolution), is_built (false) {
		glewInit ();
	}
	framebuffer::~framebuffer (void) {
		if (textures.size () > 0) {
			for (auto it = textures.begin (); it != textures.end (); ++ it)
				it -> clear ();
			textures.clear ();
		}
		if (is_built) {
			glDeleteRenderbuffers (1, & depth_id);
			glDeleteFramebuffers (1, & framebuffer_id);
		}
	}

	void framebuffer::clear (void) {
		bind ();
		for (unsigned int it = 0; it < textures.size (); ++ it) {
			texture_params p = textures [it].get_params ();

			switch (p.type) {
			case GL_INT:
				glClearBufferiv (GL_COLOR, it, ic); break;
			case GL_UNSIGNED_BYTE:
				glClearBufferuiv (GL_COLOR, it, uc); break;
			case GL_FLOAT:
				glClearBufferfv (GL_COLOR, it, & f); break;
			default:
				break;
			}
		}
		glClearBufferfv  (GL_DEPTH, 0, & f); 
		unbind ();
	}

	void framebuffer::bind (void) {
		glBindFramebuffer (GL_FRAMEBUFFER, framebuffer_id);
	}
	void framebuffer::unbind (void) {
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}

	void framebuffer::finalize (void) {
		if (is_built)
			return;

		std::vector <GLenum> targ;
		
		glGenRenderbuffers (1, & depth_id);
		glBindRenderbuffer (GL_RENDERBUFFER, depth_id);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, (int) resolution.x, (int) resolution.y);
		glBindRenderbuffer (GL_RENDERBUFFER, 0);

		glGenFramebuffers (1, & framebuffer_id);
		glBindFramebuffer (GL_FRAMEBUFFER, framebuffer_id);

		for (GLuint it = 0; it < textures.size (); ++ it) {
			textures [it].bind ();
			texture_params params = textures [it].get_params ();

			glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + params.unit, GL_TEXTURE_2D, textures [it].get_id (), 0);
			targ.push_back (GL_COLOR_ATTACHMENT0 + params.unit);
		}
		glBindRenderbuffer (GL_RENDERBUFFER, depth_id);
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_id);

		glDrawBuffers (textures.size (), & targ [0]);

		if (glCheckFramebufferStatus (GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "ERROR: Initializing frame buffer object " << framebuffer_id << ": Frame buffer not complete" << std::endl;
			return;
		}
		glBindFramebuffer (GL_FRAMEBUFFER, 0);

		is_built = true;
	}
	void framebuffer::add_texture (texture t) {
		if (is_built)
			return;
		textures.push_back (t);
	}
}