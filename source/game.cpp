#include "bloc.h"

#include "game.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "input.h"
#include "file.h"
#include <iostream>

#include <noise/noise.h>

#include "renderer.h"

#include <Windows.h>
#include <windef.h>
#include "world.h"

#define FPS_TIME 1.0
#define sensitivity 0.001
/*
To Do:
- Implement world generation / loading-saving
- Change fonts to use predefined accented characters
- Implement skeletal animation / loading-saving
- Implement entities

*/

namespace game {
	int start_world (GLFWwindow * window, renderer & re, input & in, vec2 & resolution) {
		#pragma region Initialization
		vertexbuffer screen_quad = re.create_vertexbuffer (vertexbuffer_texture, GL_TRIANGLES, false, vec3 ());
		screen_quad.add_tri (vec3 (-1,  1, 0), vec3 (-1, -1, 0), vec3 ( 1,  1, 0), 
							 vec2 ( 0,  1   ), vec2 ( 0,  0   ), vec2 ( 1,  1   ));
		screen_quad.add_tri (vec3 ( 1,  1, 0), vec3 (-1, -1, 0), vec3 ( 1, -1, 0),
							 vec2 ( 1,  1   ), vec2 ( 0,  0   ), vec2 ( 1,  0   ));
		screen_quad.build ();

		font f = re.create_font ("tahoma.ttf", 16);

		vertexbuffer grid = re.create_vertexbuffer (vertexbuffer_color, GL_LINES, true, vec3 ());

		create_grid (grid);

		grid.build ();

		vertexbuffer frametime_line = re.create_vertexbuffer (vertexbuffer_color, GL_LINES, false, vec3 ());
		vertexbuffer frametime_line_prev = re.create_vertexbuffer (vertexbuffer_color, GL_LINES, false, vec3 ());
		std::vector <std::pair <double, double>> frametime_graph_lines = std::vector <std::pair <double, double>> ();

		world wo = world (re);

		wo.set_cube_world (2);

		boolean render_world = false;

		#pragma region FPS Initialization
		double fpstime = 0, p_deltatime = 0, deltatime = 0, best_frame_time = 0;
		int frames = 0, fps = 0;
		#pragma endregion

		vec2 click_mouse = vec2 ();

		#pragma endregion	

		while (! glfwWindowShouldClose (window)) {
			#pragma region Updating Stage
			#pragma region FPS Updating
			p_deltatime = deltatime;
			deltatime = glfwGetTime ();
			re.set_frametime (deltatime - p_deltatime);

			if (best_frame_time == 0 || best_frame_time > deltatime - p_deltatime)
				best_frame_time = deltatime - p_deltatime;
			
			++ frames;


			frametime_line.add_line (vec3 (-0.95 + ((deltatime - fpstime) * 0.5), 0.65, 0.0),
									 vec3 (-0.95 + ((deltatime - fpstime) * 0.5), 0.75 + (0.2 / ((deltatime - p_deltatime) / best_frame_time)), 0.0),
									 color (argbtoint (0, 255, 0, 255)));
			frametime_line.build ();

			for (auto i = frametime_graph_lines.begin (); i != frametime_graph_lines.end (); ++ i) {
				frametime_line_prev.add_line (vec3 (-0.95 + (i -> first), 0.65, 0.0),
											  vec3 (-0.95 + (i -> first), 0.75 + i -> second, 0.0),
											  color (argbtoint (255, 0, 0, 255)));
			}

			frametime_line_prev.build ();

			frametime_graph_lines.push_back (std::make_pair (((deltatime - fpstime) * 0.5), (0.2 / ((deltatime - p_deltatime) / best_frame_time))));

			if (deltatime - fpstime >= FPS_TIME) {
				fpstime = deltatime;
				fps = frames;
				frames = 0;
				frametime_graph_lines.clear ();
				frametime_line_prev.clear ();
			}
			#pragma endregion

			#pragma region Input Updating
			in.update ();

			keyboard_container keyboard = in.get_keyboard ();
			keyboard_container pkeyboard = in.get_pkeyboard ();
			mouse_container mouse = in.get_mouse ();
			mouse_container pmouse = in.get_pmouse ();

			vec3 camera_move = vec3 ();
			quat camera_rotate = re.get_camera ().get_orientation ();

			if (keyboard.key_states [key_w] || keyboard.key_states [key_a] || keyboard.key_states [key_s] || keyboard.key_states [key_d] ||
				keyboard.key_states [key_space] || keyboard.key_states [key_left_shift]) {
				if (keyboard.key_states [key_w])
					camera_move.z += 1;
				if (keyboard.key_states [key_s])
					camera_move.z -= 1;

				if (keyboard.key_states [key_d])
					camera_move.x += 1;
				if (keyboard.key_states [key_a])
					camera_move.x -= 1;

				if (keyboard.key_states [key_left_shift])
					camera_move.y += 1;
				if (keyboard.key_states [key_space])
					camera_move.y -= 1;

				camera_move *= re.get_frametime () * (keyboard.key_states [key_left_control] ? 80 : 20);
			}
			if (keyboard.key_states [key_u] != pkeyboard.key_states [key_u] && keyboard.key_states [key_u])
				render_world = !render_world;

			if (mouse.right) {
				if (mouse.right != pmouse.right) {
					in.set_mouse (in.get_resolution () * 0.5);
					glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				} else if (mouse.pos != pmouse.pos) {
					vec2 delta = (in.get_resolution () * 0.5) - mouse.pos;
					camera_rotate.rotate (vec3 (0, 1, 0), delta.x * tau * sensitivity, true);
					camera_rotate.rotate (vec3 (1, 0, 0), delta.y * tau * sensitivity, false);
					in.set_mouse (in.get_resolution () * 0.5);
				}
			} else
				glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			#pragma endregion

			re.move_camera (camera_move, camera_rotate);

			re.update ();

			wo.update ();

			#pragma endregion

			#pragma region Drawing Stage
			vec3 cam_pos = vec3 (re.get_camera ().get_position ().x, 
								 - re.get_camera ().get_position ().y,
								 re.get_camera ().get_position ().z);

			#pragma region Scene Drawing
			glEnable (GL_DEPTH_TEST);
			glEnable (GL_CULL_FACE);

			re.clear_fbo ();
			re.bind_fbo ();

			if (render_world)
				wo.render ();

			grid.draw ();

			re.unbind_fbo ();

			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			re.get_shader ("Output Shader").bind ();
			screen_quad.draw ();
			re.get_shader ("Output Shader").unbind ();

			glDisable (GL_DEPTH_TEST);
			glDisable (GL_CULL_FACE);

			#pragma endregion

			#pragma region GUI Drawing
			re.get_shader ("Gui Color Rendering Shader").bind ();

			frametime_line_prev.draw ();
			frametime_line.draw ();

			re.get_shader ("Gui Color Rendering Shader").unbind ();

			re.get_shader ("Text Rendering Shader").bind ();
			f.draw ("Frame Time / Frames per " + std::to_string (FPS_TIME) 
					+ " * Second : " + std::to_string (re.get_frametime ()) 
					+ " / " + std::to_string (fps), 
					vec2 (5, 5), color (argbtoint (255, 255, 255, 255)));
			re.get_shader ("Text Rendering Shader").unbind ();

			glfwSwapBuffers (window);
			glfwPollEvents ();
			#pragma endregion
			#pragma endregion
		}

		return 0;
	}

	input static_in;

	#pragma region Callback Functions
	void mouse_button_callback_ptr (GLFWwindow * w, int button, int action, int mods) { 
		static_in.pend_mouse_event (vec2 (0, 0), button, action); 
	}
	void mouse_move_callback_ptr (GLFWwindow * w, double x, double y) { 
		static_in.pend_mouse_event (vec2 (x, y), -1, -1); 
	}
	void key_press_callback_ptr (GLFWwindow * w, int key, int scode, int action, int mod) { 
		static_in.pend_keyboard_event (key, action); 
	}
	void key_type_callback_ptr (GLFWwindow * w, unsigned int code) { 
		static_in.pend_typed_char (code); 
	}
	#pragma endregion

	int start (void) {
		#pragma region Console Modification
		HWND hwind = GetConsoleWindow ();
		HMENU hmenu = GetSystemMenu (hwind, FALSE);
		EnableMenuItem (hmenu, SC_CLOSE, MF_GRAYED);
		#pragma endregion

		#pragma region OpenGL Initialization
		vec2 resolution (1000, 600);
		GLFWwindow * window;

		if (! glfwInit ())
			return -1;
		glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow (resolution.x, (int) resolution.y, "Voxel Editor", NULL, NULL);
		if (! window) {
			glfwTerminate ();
			return -1;
		}
		
		if (! glewInit ())
			return -1;

		glfwMakeContextCurrent (window);

		glClearColor (0.15f, 0.15f, 0.15f, 1.0f);
		glEnable (GL_DEPTH_TEST);
		glEnable (GL_CULL_FACE);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth (2.f);

		#pragma endregion
		#pragma region Game Component Initialization
		static_in = input (window, resolution);
		renderer re = renderer (resolution);

		glfwSetMouseButtonCallback (window, mouse_button_callback_ptr);
		glfwSetCursorPosCallback (window, mouse_move_callback_ptr);
		glfwSetKeyCallback (window, key_press_callback_ptr);
		glfwSetCharCallback (window, key_type_callback_ptr);
		#pragma endregion

		int g = start_world (window, re, static_in, resolution);
		glfwTerminate ();
		return g;
	}
}