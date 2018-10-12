#ifndef INPUT_h
#define INPUT_h

#include <array>

#include <string>

#include "vec.h"
#include <vector>

struct GLFWwindow;

namespace game {
	enum keys { 
		/* Enumerators for the defined keys. Multiple reasons lie behind its design.
		 *  1. Maps the haphazard GLFW key IDs to a simpler range that can be easily used due to the properties of enumerators.
		 *  2. Creates a flexible set of recognizable keys the input system can use, instead of looking up more GLFW IDs and adding 
		 *     more if statements/case clauses.
		 *  3. Provides other functionalities that using native IDs won't grant. 
		 */
		key_escape = 0,
		key_f1,
		key_f2,
		key_f3,
		key_f4,
		key_f5,
		key_f6,
		key_f7,
		key_f8,
		key_f9,
		key_f10,
		key_f11,
		key_f12,
		key_tilde,
		key_one,
		key_two,
		key_three,
		key_four,
		key_five,
		key_six,
		key_seven,
		key_eight,
		key_nine,
		key_zero,
		key_minus,
		key_equals,
		key_backspace,
		key_tab,
		key_q,
		key_w,
		key_e,
		key_r,
		key_t,
		key_y,
		key_u,
		key_i,
		key_o,
		key_p,
		key_left_bracket,
		key_right_bracket,
		key_backslash,
		key_caps_lock,
		key_a,
		key_s,
		key_d,
		key_f,
		key_g,
		key_h,
		key_j,
		key_k,
		key_l,
		key_semicolon,
		key_apostrophe,
		key_enter,
		key_left_shift,
		key_z,
		key_x,
		key_c,
		key_v,
		key_b,
		key_n,
		key_m,
		key_comma,
		key_period,
		key_forwardslash,
		key_right_shift,
		key_left_control,
		key_left_alt,
		key_space,
		key_right_alt,
		key_right_control,
		key_up_arrow,
		key_down_arrow,
		key_left_arrow,
		key_right_arrow,

		// New key definitions must be inserted above this comment.
		key_amount, // Placed after all actual keys keys to give a value equal to the amount of supported keys. 
				    // This is not a key and shouldn't be used like one.
		unknown_key // Placed after the key_amount to signify unrecognized keys.
	};

	struct keyboard_container { // Wraps the GLFW key input system into a simple keyboard object.
		std::array <int, keys::key_amount> key_states;  // Contains all key states for all recognizable keys on the keyboard.
		keyboard_container (void);
	};
	struct keyboard_event { // Used to update the key_states member in the keyboard_container struct.
		keys key; // The enumerator that identifies the key whose state changed.
		keyboard_event (keys key, int state);

		int state; // The integer that represents the key's state (0 = released, 1 = pressed, 2 = held
	};

	struct mouse_container { // Wraps the GLFW mouse input system into a simple mouse object.
		bool left, right; // Booleans which represent the state of the mouse buttons

		mouse_container (void);
		mouse_container (vec2 pos, bool left, bool right);

		vec2 pos; // The position of the mouse relative to the game window.
	};
	struct mouse_event {
		int action, button; // Identifies whether the buttons were pressed or the mouse was moved. Button is used as an integer boolean if a button was pressed.

		mouse_event (vec2 pos, int button, int action);

		vec2 pos; // Used to represent the new mouse position.
	};

	class input {
		friend int start (void);
	public:
		const std::string get_clipboard (void) const; // Returns either a string from the clipboard if it has anything, or an empty string otherwise
		const keyboard_container & get_keyboard (void) const;  // Returns an immutable reference of the keyboard container.
		const mouse_container & get_mouse (void) const;	       // Returns an immutable reference of the mouse container.
		const keyboard_container & get_pkeyboard (void) const; // Returns an immutable reference of the keyboard container one frame earlier.
		const mouse_container & get_pmouse (void) const;       // Returns an immutable reference of the mouse container one frame earlier.
		const vec2 get_resolution (void) const; // Returns the resolution of the game window.
		const std::string get_typed (void); // Returns either the internal string buffer if the type_watch flag is true, or an empty string otherwise

		input (void);

		void pend_keyboard_event (int key, int state); // Adds a keyboard event to the queue.
		void pend_mouse_event (vec2 pos, int button, int action); // Adds a mouse event to the queue.
		void pend_typed_char (int codepoint); // Adds a typed character to the queue. Do not use the keys enumerators for this.

		void set_mouse (vec2 pos); // Sets the mouse container and actual mouse's position relative to the game window's position.
		void set_typing_watch (bool state); // Sets the type watch flag.

		void update (void); // Updates the input class. pkeyboard and pmouse are updated, and all events in the queue are applied to the
		                    // keyboard and mouse containers.

	private:
		input (GLFWwindow * window, vec2 resolution); // Private constructor for only the start function to use.

		std::vector <keyboard_event> key_pressed; // Queue of keyboard events.
		keyboard_container keyboard; // Keyboard object.

		mouse_container mouse; // Mouse object.
		std::vector <mouse_event> mouse_click; // Queue of mouse click events.
		std::vector <mouse_event> mouse_move; // Queue of mouse move events.

		keyboard_container pkeyboard; // Stored keyboard object which represents the keyboard's state one frame earlier.
		mouse_container pmouse; // Stored mouse object which represents the keyboard's state one frame earlier.

		vec2 resolution; // Resolution of the screen.

		bool typing_watch; // Typing flag used to interpret input either as key presses or character input.
		std::string typed; // String buffer storing the typed string.

		GLFWwindow * window; // Pointer to the game window.
	};
}

#endif
