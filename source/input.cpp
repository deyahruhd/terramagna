#include <GLFW/glfw3.h>

#include "input.h"
#include <iostream>

namespace game {
	input::input (void) {
	}
	input::input (GLFWwindow * window, vec2 resolution) : mouse (mouse_container (vec2 (0, 0), false, false)), window (window), resolution (resolution), typing_watch (false), typed ("") {
	}

	keys translate (int key) {
		switch (key) {
		default: 
			return unknown_key;
		case GLFW_KEY_ESCAPE:
			return key_escape;
		case GLFW_KEY_F1:
			return key_f1;
		case GLFW_KEY_F2:
			return key_f2;
		case GLFW_KEY_F3:
			return key_f3;
		case GLFW_KEY_F4:
			return key_f4;
		case GLFW_KEY_F5:
			return key_f5;
		case GLFW_KEY_F6:
			return key_f6;
		case GLFW_KEY_F7:
			return key_f7;
		case GLFW_KEY_F8:
			return key_f8;
		case GLFW_KEY_F9:
			return key_f9;
		case GLFW_KEY_F10:
			return key_f10;
		case GLFW_KEY_F11:
			return key_f11;
		case GLFW_KEY_F12:
			return key_f12;
		case GLFW_KEY_GRAVE_ACCENT:
			return key_tilde;
		case GLFW_KEY_1:
			return key_one;
		case GLFW_KEY_2:
			return key_two;
		case GLFW_KEY_3:
			return key_three;
		case GLFW_KEY_4:
			return key_four;
		case GLFW_KEY_5:
			return key_five;
		case GLFW_KEY_6:
			return key_six;
		case GLFW_KEY_7:
			return key_seven;
		case GLFW_KEY_8:
			return key_eight;
		case GLFW_KEY_9:
			return key_nine;
		case GLFW_KEY_0:
			return key_zero;
		case GLFW_KEY_MINUS:
			return key_minus;
		case GLFW_KEY_EQUAL:
			return key_equals;
		case GLFW_KEY_BACKSPACE:
			return key_backspace;
		case GLFW_KEY_TAB:
			return key_tab;
		case GLFW_KEY_Q:
			return key_q;
		case GLFW_KEY_W:
			return key_w;
		case GLFW_KEY_E:
			return key_e;
		case GLFW_KEY_R:
			return key_r;
		case GLFW_KEY_T:
			return key_t;
		case GLFW_KEY_Y:
			return key_y;
		case GLFW_KEY_U:
			return key_u;
		case GLFW_KEY_I:
			return key_i;
		case GLFW_KEY_O:
			return key_o;
		case GLFW_KEY_P:
			return key_p;
		case GLFW_KEY_LEFT_BRACKET:
			return key_left_bracket;
		case GLFW_KEY_RIGHT_BRACKET:
			return key_right_bracket;
		case GLFW_KEY_BACKSLASH:
			return key_backslash;
		case GLFW_KEY_CAPS_LOCK:
			return key_caps_lock;
		case GLFW_KEY_A:
			return key_a;
		case GLFW_KEY_S:
			return key_s;
		case GLFW_KEY_D:
			return key_d;
		case GLFW_KEY_F:
			return key_f;
		case GLFW_KEY_G:
			return key_g;
		case GLFW_KEY_H:
			return key_h;
		case GLFW_KEY_J:
			return key_j;
		case GLFW_KEY_K:
			return key_k;
		case GLFW_KEY_L:
			return key_l;
		case GLFW_KEY_SEMICOLON:
			return key_semicolon;
		case GLFW_KEY_APOSTROPHE:
			return key_apostrophe;
		case GLFW_KEY_ENTER:
			return key_enter;
		case GLFW_KEY_LEFT_SHIFT:
			return key_left_shift;
		case GLFW_KEY_Z:
			return key_z;
		case GLFW_KEY_X:
			return key_x;
		case GLFW_KEY_C:
			return key_c;
		case GLFW_KEY_V:
			return key_v;
		case GLFW_KEY_B:
			return key_b;
		case GLFW_KEY_N:
			return key_n;
		case GLFW_KEY_M:
			return key_m;
		case GLFW_KEY_COMMA:
			return key_comma;
		case GLFW_KEY_PERIOD:
			return key_period;
		case GLFW_KEY_SLASH:
			return key_forwardslash;
		case GLFW_KEY_RIGHT_SHIFT:
			return key_right_shift;
		case GLFW_KEY_LEFT_CONTROL:
			return key_left_control;
		case GLFW_KEY_LEFT_ALT:
			return key_left_alt;
		case GLFW_KEY_SPACE:
			return key_space;
		case GLFW_KEY_RIGHT_ALT:
			return key_right_alt;
		case GLFW_KEY_RIGHT_CONTROL:
			return key_right_control;
		case GLFW_KEY_UP:
			return key_up_arrow;
		case GLFW_KEY_DOWN:
			return key_down_arrow;
		case GLFW_KEY_LEFT:
			return key_left_arrow;
		case GLFW_KEY_RIGHT:
			return key_right_arrow;
		}
	}

	mouse_container::mouse_container (void) {
	}
	mouse_container::mouse_container (vec2 pos, bool left, bool right) : pos (pos), left (left), right (right) {
	}
	mouse_event::mouse_event (vec2 pos, int button, int action) : pos (pos), button (button), action (action) {
	}
	keyboard_container::keyboard_container (void) : key_states () {
	}
	keyboard_event::keyboard_event (keys key, int state) : key (key), state (state) {
	}

	void input::pend_mouse_event (vec2 pos, int button, int action) {
		if (button == 0 || button == 1)
			mouse_click.push_back (mouse_event (vec2 (0, 0), button, action));
		else
			mouse_move.push_back (mouse_event (vec2 (pos.x, pos.y), -1, -1));
	}
	void input::pend_keyboard_event (int key, int state) {
		keys k = translate (key);

		if (keyboard.key_states [k] != state && k != keys::unknown_key)
			key_pressed.push_back (keyboard_event (k, state));
	}
	void input::pend_typed_char (int codepoint) {
		if (! typing_watch)
			return;

		typed.push_back ((char) codepoint);
	}
	void input::set_mouse (vec2 pos) {
		glfwSetCursorPos (window, pos.x, pos.y);
		mouse.pos = pos;
	}
	void input::update (void) {
		pmouse.left = mouse.left;
		pmouse.right = mouse.right;
		pmouse.pos = mouse.pos;

		for (unsigned int it = 0; it < pkeyboard.key_states.size (); ++ it) {
			pkeyboard.key_states [it] = keyboard.key_states [it];
		}

		if (mouse_click.size () > 0) {
			mouse_event e = mouse_click.back ();
			switch (e.button) {
			case 0:
				mouse.left = e.action; break;
			case 1:
				mouse.right = e.action; break;
			}
			mouse_click.pop_back ();
		}
		if (mouse_move.size () > 0) {
			mouse_event e = mouse_move.back ();
			mouse.pos = e.pos;
			mouse_move.pop_back ();
		}
		if (key_pressed.size () > 0) {
			keyboard_event e = key_pressed.back ();
			keyboard.key_states [e.key] = e.state;
		}
	}

	const keyboard_container & input::get_keyboard (void) const {
		return keyboard;
	}
	const mouse_container & input::get_mouse (void) const {
		return mouse;
	}
	const keyboard_container & input::get_pkeyboard (void) const {
		return pkeyboard;
	}
	const mouse_container & input::get_pmouse (void) const {
		return pmouse;
	}
	const vec2 input::get_resolution (void) const {
		return resolution;
	}
	const std::string input::get_typed (void) {
		if (! typing_watch)
			return "";

		const std::string w = typed;
		typed = "";

		return w;
	}
	const std::string input::get_clipboard (void) const {
		const char * ptr = glfwGetClipboardString (window);

		if (ptr == NULL)
			return "";

		return ptr;
	}

	void input::set_typing_watch (bool state) {
		typing_watch = state;

		if (! state)
			typed = "";
	}
}	