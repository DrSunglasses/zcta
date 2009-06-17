#include "VerticalComparators.hpp"
#include <iostream>

using namespace std;

void testVerticalPointComparator() {
	VerticalPointComparator v;
	Point a(0, 0), b(1, 0), c(0, 1), d(1, 1);
	//c < a < d < b
	assert(v(c, a) < 0);
	assert(v(a, c) > 0);
	
	assert(v(a, d) < 0);
	assert(v(d, a) > 0);
	
	assert(v(d, b) < 0);
	assert(v(b, d) > 0);
	
	assert(v(c, d) < 0);
	assert(v(d, c) > 0);
	
	assert(v(c, b) < 0);
	assert(v(b, c) > 0);
	
	assert(v(a, b) < 0);
	assert(v(b, a) > 0);
}

void testVerticalLineComparator_thetaAdjustment() {
	VerticalLineComparator c;
	c.update(Point(1, 0));
	Line l(Point(0, -1), Point(2, 1));
	Line g(Point(0, 1), Point(2, -1));
	
	cout << l.theta() << endl << g.theta();
	
	assert(c(l, g) < 0);
	assert(c(g, l) > 0);
}

void testVerticalLineComparator() {
	testVerticalLineComparator_thetaAdjustment();
}
