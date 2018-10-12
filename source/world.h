#ifndef WORLD_h
#define WORLD_h

#include "bloc.h"

#include <memory>

#include <map>

#include <vector>

namespace game {
	class renderer; // Forward declare the renderer class to avoid inclusion of render headers in voxel headers.

	class world {
	public:
		world (renderer & re); 

		color get (vec3 coordinates);
		bloc get_bloc (vec3 coordinates);

		void render (void);

		void set (vec3 coordinates, color c);
		
		void update (void);

		void set_cube_world (int rad);
	private:
		//std::vector <std::unique_ptr <vec3>> sources; // A vector of vertex pointers that indicate various locations in the world where blocs should be loaded.
		
		renderer & re;

		std::vector <vec3> load_blocs;
		std::vector <std::unique_ptr <bloc>> loaded_blocs;
		std::vector <std::unique_ptr <bloc>> unload_blocs;
	
		std::unordered_map <int, std::pair <std::unique_ptr <bloc>, std::unique_ptr <vertexbuffer>>> builded_blocs;
	};
}

#endif