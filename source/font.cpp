#include <algorithm>

#include "font.h"
#include "renderer.h"
#include "shader.h"

namespace game {
	const std::wstring font::endl		  = std::wstring (L"\x0085", 1);
	const std::wstring font::emark_invert = std::wstring (L"\x00A1", 1);
	const std::wstring font::qmark_invert = std::wstring (L"\x00BF", 1);
	
	const std::wstring font::diacritic_accent		= std::wstring (L"\x0301", 1);
	const std::wstring font::diacritic_breve		= std::wstring (L"\x0306", 1);
	const std::wstring font::diacritic_caron		= std::wstring (L"\x030C", 1);
	const std::wstring font::diacritic_circumflex   = std::wstring (L"\x0302", 1);
	const std::wstring font::diacritic_doubleaccent = std::wstring (L"\x030B", 1);
	const std::wstring font::diacritic_doublegrave  = std::wstring (L"\x030F", 1);
	const std::wstring font::diacritic_grave		= std::wstring (L"\x0300", 1);
	const std::wstring font::diacritic_macron		= std::wstring (L"\x0304", 1);
	const std::wstring font::diacritic_tilde        = std::wstring (L"\x0303", 1);
	const std::wstring font::diacritic_titlo		= std::wstring (L"\x0483", 1);
	const std::wstring font::diacritic_umlaut       = std::wstring (L"\x0308", 1);

	glyph_params::glyph_params (void) : advance (vec2 ()), size (vec2 ()), bearing (vec2 ()), texture_offset (0.0) {
	}
	glyph_params::glyph_params (vec2 advance, vec2 size, vec2 bearing, double texture_offset) : advance (advance), size (size), bearing (bearing), texture_offset (texture_offset) {
	}
	
	font::font (void) : is_built (false), text_shad (NULL) {
	}
	font::~font (void) {
	}
	font::font (std::string name, int size, renderer & r) : is_built (false), text_shad (& r.get_shader ("Text Rendering Shader")), resolution (r.get_resolution ()) {
		FT_Library ft = r.get_freetypelib ();
		if (ft == NULL || text_shad == NULL)
			return;

		std::string s = "files\\assets\\fonts\\" + name;
		if (FT_New_Face (ft, s.c_str (), 0, & f)) {
			std::cout << "Error in font '" << name << "': File '" << name << "' could not be loaded" << std::endl;
			return;
		}

		FT_Set_Pixel_Sizes (f, 0, size);
		FT_Set_Charmap (f, f -> charmaps [0]);

		vec2 ve = vec2 ();
		std::vector <wchar_t> uglyphs;

		uglyphs.push_back (0x0009);
		for (wchar_t it = 0x0020; it < 0x007F; ++ it) {
			uglyphs.push_back (it);
		}

		// Inverted question and exclamation marks
		uglyphs.push_back (0x00A1);
		uglyphs.push_back (0x00BF);

		// Dotless I and J
		uglyphs.push_back (0x0131);
		uglyphs.push_back (0x0237);

		// Diacritics
		uglyphs.push_back (0x0300);
		uglyphs.push_back (0x0301);
		uglyphs.push_back (0x0302);
		uglyphs.push_back (0x0303);
		uglyphs.push_back (0x0304);
		uglyphs.push_back (0x0306);
		uglyphs.push_back (0x0308);
		uglyphs.push_back (0x030B);
		uglyphs.push_back (0x030C);
		uglyphs.push_back (0x030F);
		uglyphs.push_back (0x0483);

		for (auto it = uglyphs.begin (); it != uglyphs.end (); ++ it) {
			if (FT_Load_Glyph (f, FT_Get_Char_Index (f, * it),  FT_LOAD_RENDER)) {
				std::cout << "Error in font '" << name << "': Char U-" << * it << " could not be loaded" << std::endl;
				continue;
			}

			if (* it == 0x41)
				height = f -> glyph -> bitmap.rows;

			if (* it < 0x0020 || * it > 0x007F)
				if (* it > 0x0299 && * it < 0x0484)
					glyphs_diacritics.push_back (* it);
				else
					glyphs_special.push_back (* it);

			ve.x += f -> glyph -> bitmap.width;
			ve.y = std::max ((int) ve.y, (int) f -> glyph -> bitmap.rows);
		}

		t = r.create_texture (true, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_RED, GL_RED, GL_UNSIGNED_BYTE, ve, 1, NULL);
		v = r.create_vertexbuffer (vertexbuffer_texture, GL_TRIANGLES, false, vec3 ());

		vec2 ve1 = vec2 ();
		for (auto it = uglyphs.begin (); it != uglyphs.end (); ++ it) {
			if (FT_Load_Glyph (f, FT_Get_Char_Index (f, * it),  FT_LOAD_RENDER))
				continue;

			t.sub (ve1, vec2 (f -> glyph -> bitmap.width, f -> glyph -> bitmap.rows), f -> glyph -> bitmap.buffer);

			glyphs.insert (std::pair <wchar_t, glyph_params> (* it, glyph_params (vec2 (f -> glyph -> advance.x >> 6, f -> glyph -> advance.y >> 6),
														    vec2 (f -> glyph -> bitmap.width, f -> glyph -> bitmap.rows),
														    vec2 (f -> glyph -> bitmap_left, f -> glyph -> bitmap_top),
														    ve1.x / ve.x)));
			
			ve1.x += f -> glyph -> bitmap.width;
		}

		skip = size;

		is_built = true;
	}

	void font::clear (void) {
		if (! is_built)
			return;
		t.clear ();
		v.clear ();
		glyphs.clear ();
		FT_Done_Face (f);
		text_shad = NULL;

		is_built = false;
	}

	void font::draw (std::string text, vec2 position, color col) {
		if (! is_built || text == "")
			return;

		std::wstring wtext (text.length(), L' ');

		std::copy (text.begin (), text.end (), wtext.begin ());

		draw (wtext, position, col);
	}
	void font::draw (std::wstring text, vec2 position, color col) {
		if (! is_built || text == L"")
			return;

		size_t spos = 0;
		while ((spos = text.find (L"i", spos)) != std::wstring::npos) {
			std::wstring dot = L"i";
			std::wstring dotless = L"\x0131";

			wchar_t next = text.length () > spos + 1 ? text.at (spos + 1) : 0x0;

			dot += next;
			dotless += next;

			if (std::find (glyphs_diacritics.begin (), glyphs_diacritics.end (), next) != glyphs_diacritics.end ()) { 
				text.replace (spos, dot.length (), dotless);
				spos += dotless.length ();
			} else
				++ spos;
		}
		spos = 0;
		while ((spos = text.find (L"j", spos)) != std::wstring::npos) {
			std::wstring dot = L"j";
			std::wstring dotless = L"\x0237";
			wchar_t next = text.length () > spos + 1 ? text.at (spos + 1) : 0x0;

			dot += next;
			dotless += next;

			if (std::find (glyphs_diacritics.begin (), glyphs_diacritics.end (), next) != glyphs_diacritics.end ()) { 
				text.replace (spos, dot.length (), dotless);
				spos += dotless.length ();
			} else
				++ spos;
		}
		
		vec2 cursor = vec2 (position.x, resolution.y - position.y - height);
		glyph_params gp_previous;

		texture_params t_params = t.get_params ();

		if (! (text == previous_text && position == previous_pos)) {
			for (auto it = text.begin (); it != text.end (); ++ it) {
				if (* it == 0x0085) {
					cursor = vec2 (position.x, cursor.y - skip);
					continue;
				}

				glyph_params gp = glyphs.at (* it);
				vec2 glyph_pos;

				if (* it == 0x0009) {
					cursor += gp.advance;
					continue;
				}
				if (std::find (glyphs_diacritics.begin (), glyphs_diacritics.end (), * it) != glyphs_diacritics.end ()) {
					glyph_pos = cursor + vec2 (- gp_previous.size.x, gp_previous.bearing.y + gp.size.y + 1.0);
				} else {
					glyph_pos = cursor + gp.bearing;
					cursor += gp.advance;
				}
				
				gp_previous = gp;

				if (gp.size.x == 0 || gp.size.y == 0)
					continue;

				v.add_tri (vec3 (glyph_pos, 0.0),		  vec3 (glyph_pos.x, glyph_pos.y - gp.size.y, 0.0),			   vec3 (glyph_pos.x + gp.size.x, glyph_pos.y, 0.0), 
						   vec2 (gp.texture_offset, 0.0), vec2 (gp.texture_offset, gp.size.y / t_params.resolution.y), vec2 (gp.texture_offset + gp.size.x / t_params.resolution.x, 0.0));
				v.add_tri (vec3 (glyph_pos.x + gp.size.x, glyph_pos.y, 0.0),					 vec3 (glyph_pos.x, glyph_pos.y - gp.size.y, 0.0),			   vec3 (glyph_pos + vec2 (gp.size.x, -gp.size.y), 0.0), 
						   vec2 (gp.texture_offset + (gp.size.x / t_params.resolution.x), 0.0), vec2 (gp.texture_offset, gp.size.y / t_params.resolution.y), vec2 (gp.texture_offset + (gp.size.x / t_params.resolution.x), (gp.size.y / t_params.resolution.y)));
			}

			v.build ();
			previous_text = text;
			previous_pos = position;
		}
		
		text_shad -> set_uniform ("txt", (int) t_params.unit);
		text_shad -> set_uniform ("txt_color", vec3 (col.r () / 255.0, col.g () / 255.0, col.b () / 255.0));
		text_shad -> set_uniform ("txt_alpha", col.a () / 255.f);
		v.draw ();
	}

	const int font::get_width (std::wstring text) const {
		if (! is_built)
			return 0;
		if (text == L"")
			return 0;

		int width = 0;

		for (auto it = text.begin (); it != text.end (); ++ it) {
			glyph_params t = glyphs.at (* it);

			width += t.advance.x;
		}

		return width;
	}
	const int font::get_skip (void) const {
		return skip;
	}
	const int font::get_height (void) const {
		return height;
	}
	const int font::get_tabwidth (void) const {
		return tabwidth;
	}
}