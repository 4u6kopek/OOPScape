#pragma once
#include <vector>

struct Point {
	int x = 0;
	int y = 0;
	bool operator==(const Point& o) const { return x == o.x && y == o.y; }
	bool operator!=(const Point& o) const { return !(*this == o); }
};

// The raw terrain grid: '*', ' ', 'F', 'C'
using Board = std::vector<std::vector<char>>;
