#include <algorithm>

#include "color.h"

#include <math.h>

namespace game {
	int argbtoint (int r, int g, int b, int a) {
		int re = 0;
		re = (a << 24) | (r << 16) | (g << 8) | b;
		return re;
	}

	vec3 rgbtohsv (int r, int g, int b) {
		float r_prime = (float) r / 255.f;
		float g_prime = (float) g / 255.f;
		float b_prime = (float) b / 255.f;

		float maximum = std::max (std::max (g_prime, b_prime), r_prime);
		float minimum = std::min (std::min (g_prime, b_prime), r_prime);
		float delta = maximum - minimum;

		int hue = 0;

		if (delta == 0)
			hue = 0.0;
		else if (maximum == r_prime)
			hue = 60.0 * (fmodf ((g_prime - b_prime) / delta, 6));
		else if (maximum == g_prime)
			hue = 60.0 * (((b_prime - r_prime) / delta) + 2);
		else if (maximum == b_prime)
			hue = 60.0 * (((r_prime - g_prime) / delta) + 4);

		int saturation = 0;

		if (delta != 0)
			saturation = (delta / maximum) * 100;

		if (hue < 0)
			hue = 360 + hue;

		return vec3 (hue, saturation, (int) floor ((maximum * 100) + 0.5));
	}

	int ahsvtoint (int h, int s1, int v1, int a) {
		if (h == 360)
			h = 0;
		float s = s1 / 100.f;
		float v = v1 / 100.f;
		float chroma = v * s;
		float inter_1 = h / 60.f;
		float inter_2 = chroma * (1 - abs (fmodf (inter_1, 2) - 1));
		float r1, g1, b1;
		
		switch ((int)inter_1) {
		case 0:
			r1 = chroma; g1 = inter_2; b1 = 0;
			break;
		case 1:
			r1 = inter_2; g1 = chroma; b1 = 0;
			break;
		case 2:
			r1 = 0; g1 = chroma; b1 = inter_2;
			break;
		case 3:
			r1 = 0; g1 = inter_2; b1 = chroma;
			break;
		case 4:
			r1 = inter_2; g1 = 0; b1 = chroma;
			break;
		case 5:
			r1 = chroma; g1 = 0; b1 = inter_2;
			break;
		default:
			r1 = 0; g1 = 0; b1 = 0;
			break;
		}

		float l_mod = v - chroma;
		r1 += l_mod; g1 += l_mod; b1 += l_mod;

		int r = (int)(r1 * 255);
		int g = (int)(g1 * 255);
		int b = (int)(b1 * 255);

		return argbtoint (r, g, b, a);
	}
	int ahsltoint (int h, int s1, int l1, int a) {
		float s = s1 / 100.f;
		float l = l1 / 100.f;
		float chroma = (1 - abs( (2*l) - 1)) * s;
		float inter_1 = h / 60.f;
		float inter_2 = chroma * (1 - abs(fmodf(inter_1, 2) - 1));
		float r1, g1, b1;
		
		switch ((int)inter_1) {
		case 0:
			r1 = chroma; g1 = inter_2; b1 = 0;
			break;
		case 1:
			r1 = inter_2; g1 = chroma; b1 = 0;
			break;
		case 2:
			r1 = 0; g1 = chroma; b1 = inter_2;
			break;
		case 3:
			r1 = 0; g1 = inter_2; b1 = chroma;
			break;
		case 4:
			r1 = inter_2; g1 = 0; b1 = chroma;
			break;
		case 5:
			r1 = chroma; g1 = 0; b1 = inter_2;
			break;
		default:
			r1 = 0; g1 = 0; b1 = 0;
			break;
		}

		float l_mod = l - (chroma/2);
		r1 += l_mod; g1 += l_mod; b1 += l_mod;

		int r = (int)(r1 * 255);
		int g = (int)(g1 * 255);
		int b = (int)(b1 * 255);

		return argbtoint (r, g, b, a);
	}

	color::color (void) { }
	color::color (int value) : value (value) {
	}

	const unsigned char color::a (void) const {
		return (value >> 24) & 255;
	}
	const unsigned char color::r (void) const {
		return (value >> 16) & 255;
	}
	const unsigned char color::g (void) const {
		return (value >>  8) & 255;
	}
	const unsigned char color::b (void) const {
		return  value        & 255;
	}
	const int color::get_value (void) const {
		return value;
	}

	color & color::operator = (const color & b) {
		if (this != & b) {
			value = b.value;
		}

		return * this;
	}

	bool color::operator == (color & b) {
		return value == b.value;
	}
	bool color::operator != (color & b) {
		return value != b.value;
	}
}