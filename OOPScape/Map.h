#pragma once
#include "Point.h"
#include <string>
#include <vector>

// Owns the terrain grid and entity spawn positions extracted from a level file.
// Valid terrain chars after load: '*' wall | ' ' open | 'F' exit | 'C' coin
class Map {
public:
	// Loads and validates a level file; throws runtime error on any violation.
	void load(const std::string& filename);

	bool inBounds(int x, int y)   const;
	bool isWalkable(int x, int y) const;   // true when in-bounds and not '*'

	char getCell(int x, int y)          const { return board_[y][x]; }
	void setCell(int x, int y, char c) { board_[y][x] = c; }
	int  getSize()                       const { return size_; }
	const Board& getBoard()       const { return board_; }
	const Point& getHeroStart()   const { return heroStart_; }
	const std::vector<Point>& getEnemyStarts() const { return enemyStarts_; }

private:
	int              size_ = 0;
	Board            board_;
	Point            heroStart_;
	std::vector<Point> enemyStarts_;

	static void stripCR(std::string& s);
};
