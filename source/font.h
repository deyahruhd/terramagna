#ifndef FONT_H
#define FONT_H

#include "color.h"

#define generic __identifier (generic);
#include <ft2build.h>
#include <freetype.h>
#undef generic

#include <iostream>

#include <string>

#include "texture.h"

#include <unordered_map>

#include "vertexbuffer.h"

namespace game {
	class renderer;

	class shader;

	struct glyph_params {
		vec2 advance, size, bearing;

		glyph_params (vec2 advance, vec2 size, vec2 bearing, double texture_offset);
		glyph_params (void);

		double texture_offset;
	};

	class font {
		friend class renderer;

	public:
		void clear (void);		
		void draw (std::string text, vec2 position, color c);
		void draw (std::wstring text, vec2 position, color c);

		font (void);
	   ~font (void);

		const int get_height (void) const;
		const int get_skip (void) const;
		const int get_tabwidth (void) const;
		const int get_width (std::wstring text) const;

		const static std::wstring endl;
		const static std::wstring qmark_invert;
		const static std::wstring emark_invert;

		const static std::wstring diacritic_accent;
		const static std::wstring diacritic_breve;
		const static std::wstring diacritic_caron;
		const static std::wstring diacritic_circumflex;
		const static std::wstring diacritic_doubleaccent;
		const static std::wstring diacritic_doublegrave;
		const static std::wstring diacritic_grave;
		const static std::wstring diacritic_macron;
		const static std::wstring diacritic_tilde;
		const static std::wstring diacritic_titlo;
		const static std::wstring diacritic_umlaut;
		
	private:
		FT_Face f;	

		font (std::string name, int size, renderer & r);

		std::unordered_map <wchar_t, glyph_params> glyphs;
		std::vector <wchar_t> glyphs_diacritics, glyphs_special;

		int height, skip, tabwidth;

		bool is_built;

		vec2 previous_pos;
		std::wstring previous_text;

		vec2 resolution;

		texture t;	
		shader * text_shad;

		vertexbuffer v;
	};
}

#endif