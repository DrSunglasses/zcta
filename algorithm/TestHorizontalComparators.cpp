#include "HorizontalComparators.hpp"
#include <iostream>

using namespace std;

void testHorizontalPointComparator() {
	HorizontalPointComparator h;
	Point a(0, 0), b(1, 0), c(0, 1), d(1, 1);
	// a < b < c < d

	assert(h(a, b) < 0);
	assert(h(b, a) > 0);

	assert(h(b, c) < 0);
	assert(h(c, b) > 0);

	assert(h(c, d) < 0);
	assert(h(d, c) > 0);

	assert(h(a, d) < 0);
	assert(h(d, a) > 0);

	assert(h(b, d) < 0);
	assert(h(d, b) > 0);

	assert(h(a, c) < 0);
	assert(h(c, a) > 0);
}

void testHorizontalLineComparator_bothNegative() {
	HorizontalLineComparator h;
	Point intersection(1, 2);
	h.update(intersection);
	Line l(Point(3, 1), Point(-1, 3)), g(Point(2, 1), Point(0, 3));

	assert(l.intersectionWith(g) == intersection);
	assert(h(l, g) < 0);
	assert(h(g, l) > 0);
}

void testHorizontalLineComparator_bothPositive() {
	HorizontalLineComparator h;
	Point intersection(4, 6);
	h.update(intersection);
	Line l(Point(3, 5), Point(5, 7)), g(Point(2, 5), Point(6, 7));

	assert(l.intersectionWith(g) == intersection);
	assert(h(l, g) < 0);
	assert(h(g, l) > 0);
}

void testHorizontalLineComparator_oppositeSlopes() {
	HorizontalLineComparator h;
	h.update(Point(1, 1));
	Line l(Point(2, -1), Point(0, 1)), g(Point(2, 1), Point(0, -1));

	assert(h(l, g) < 0);
	assert(h(g, l) > 0);
}

void testHorizontalLineComparator_oneVertical() {
	HorizontalLineComparator h;
	h.update(Point(1, 1));
	Line a(Point(1, 1), Point(1, 2)), l(Point(2, -1), Point(0, 1)), g(Point(2, 1), Point(0, -1));

	assert(h(l, a) < 0);
	assert(h(a, l) > 0);

	assert(h(g, a) > 0);
	assert(h(a, g) < 0);
}

void testHorizontalLineComparator_bothVertical() {
	HorizontalLineComparator h;
	Line a(Point(1, 1), Point(1, 2)), b(Point(2, 2), Point(2, 3));

	assert(h(a, b) < 0);
	assert(h(b, a) > 0);
}

void testHorizontalLineComparator() {
	testHorizontalLineComparator_bothVertical();
	testHorizontalLineComparator_oneVertical();
	testHorizontalLineComparator_oppositeSlopes();
	testHorizontalLineComparator_bothNegative();
	testHorizontalLineComparator_bothPositive();
}
