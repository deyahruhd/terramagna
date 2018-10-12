#include "material.h"

#include <random>

namespace game {
	color material::get_color (void) {
		return _color;
	}

	material::material (void) {
	}

	material::material (material_manager::material_base b) : _color (b.random_color ()) {
	}

	color material_manager::material_base::random_color (void) {
		return colors [rand () % colors.size ()];
	}

	material material_manager::get_a_material (unsigned int id) {
		std::string name = "";
		if (id > material_map.size ()) {
			// ID is greater than the map size, dereferencing its iterator will end badly!

			// Load missing materials up to the requested ID, including itself.
			// To do this, we calculate the requested ID and the last loaded greatest id, which gives us 
			// the number of materials to load including the one with the corresponding requested ID.

			// Unordered maps do not have an "end" element due to the state of being unordered. Thus, we
			// must calculate the number of IDs to be loaded instead of using the "end" element to take 
			// advantage of O(1) lookup times.

			int number = id - last_gtr_material;

			for (int i = 1; i <= number; ++ i) {
				// Now, the number i corresponds with the "last_material + i"th material. Proceed to load it.
				load_material (/*getname*/ name);
			}
		}

		return material_map.at (name);
	}
	material material_manager::get_a_material (std::string id) {
		auto mat_it = material_map.find (id);
		if (mat_it == material_map.end ())
			return material (load_material (id));

		return material ((*mat_it).second);
	}

	material_manager::material_base::material_base (std::string id, std::vector <color> colors) : id (id), colors (colors) {
	}

	material_manager::material_base material_manager::load_material (std::string id) {
	}

	material_manager::material_manager (void) {

	}
}
