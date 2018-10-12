#ifndef MATERIAL_h
#define MATERIAL_h

#include <string>
#include <vector>
#include <unordered_map>

#include "color.h"

namespace game {
	class material;

	class material_manager {
		friend int start (void);
		friend class material;

	public:
		material get_a_material (unsigned int id); // if material is not in the map load it
		material get_a_material (std::string id); // if material is not in the map load it

	private:
		struct material_base {
			std::vector <color> colors;

			std::string id;

			material_base (std::string id, std::vector <color> colors);

			color random_color (void);
		};
		material_manager (void);
		std::unordered_map <std::string, material_base> material_map;
		int last_gtr_material; // The greatest id currently loaded in the material map.

		material_base load_material (std::string id);
	};

	class material {
		friend class material_manager;

	public:
		color get_color (void);

	private:
		material (void);
		material (material_manager::material_base b);
		color _color;
	};
}

class material {
public:
	material ();
	~material ();
};

#endif



