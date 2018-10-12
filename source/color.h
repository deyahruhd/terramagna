#ifndef COLOR_h
#define COLOR_h

#include "vec.h"

namespace game {
	int ahsltoint (int h, int s, int l, int a);
	int ahsvtoint (int h, int s, int v, int a);

	vec3 rgbtohsv (int r, int g, int b);

	int argbtoint (int r, int g, int b, int a);

	struct color {
	public:
		color (int value);
		color (void);

		const unsigned char a (void) const;

		const unsigned char b (void) const;

		const unsigned char g (void) const;
		const int get_value (void) const;

		color & operator = (const color & b);
		bool operator == (color & c);
		bool operator != (color & c);

		const unsigned char r (void) const;

	private:
		int value;
	};
}

#endif