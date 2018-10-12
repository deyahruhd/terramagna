#include "renderer.h"

#include "world.h"

#define bloc_processed_limit 5

namespace game {
	color generate (vec3 pos) {
		if ((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z) < pow ((world_bloc_width / 2.f), 2))
			return argbtoint (255, 255, 255, 255);

		return argbtoint (0, 0, 0, 0);
	}

	world::world (renderer & re) : re (re) {

	}

	color world::get (vec3 coordinates) {
		
	}
	bloc world::get_bloc (vec3 coordinates) {
		return bloc (vec3 ());
	}

	void world::set (vec3 coordinates, color c) {
		
	}
		
	void world::update (void) {
		int processed = 0;

		for (auto i1 = load_blocs.begin (); i1 != load_blocs.end ();) {
			if (processed >= bloc_processed_limit) {
				processed = 0;
				break;
			}

			bloc * b = new bloc (* i1);
			for (unsigned int in = 0; in < (world_bloc_width * world_bloc_width * world_bloc_width); ++ in) {
				vec3 loc = get_3d_index (in, vec3 (world_bloc_width)) - vec3 ((world_bloc_width / 2));

				b->set (in, generate (loc));
			}

			loaded_blocs.push_back (std::unique_ptr <bloc> (b));
			i1 = load_blocs.erase (i1);
		}

		for (auto i2 = loaded_blocs.begin (); i2 != loaded_blocs.end ();) {
			if (processed >= bloc_processed_limit) {
				processed = 0;
				break;
			}

			std::unique_ptr <vertexbuffer> v = std::unique_ptr <vertexbuffer> 
				(re.create_vertexbuffer_ptr (vertexbuffer_color, GL_TRIANGLES, true, vec3 ((* i2)->get_position ())));

			populate (* v, * (* i2));

			builded_blocs.insert (std::make_pair (builded_blocs.size (), std::make_pair (std::move (* i2), std::move (v))));
			
			i2 = loaded_blocs.erase (i2);
		}
	}

	void world::render (void) {
		if (builded_blocs.size () <= 0)
			return;

		for (auto i = builded_blocs.begin ();
			 i != builded_blocs.end (); ++ i) {
			(* i).second.second -> draw ();
		}
	}
	
	void world::set_cube_world (int rad) {
		for (int i = - rad; i <= rad; ++ i) {
			for (int j = - rad; j <= rad; ++ j) {
				for (int k = - rad; k <= rad; ++ k) {

					vec3 v = vec3 (-i, j, -k) * world_bloc_width;

					load_blocs.push_back (v);
				}
			}
		}
	}
}