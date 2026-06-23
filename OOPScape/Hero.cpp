#include "Hero.h"
#include "Map.h"

// Wizard
Wizard::Wizard(Point pos) : Hero(pos) {}

bool Wizard::useSpecialAbility(Map& map,
	std::vector<std::unique_ptr<Enemy>>& /*enemies*/,
	char direction)
{
	int dx = 0, dy = 0;
	switch (direction) {
	case 'L': dx = -2; break;
	case 'R': dx = 2; break;
	case 'U': dy = -2; break;
	case 'D': dy = 2; break;
	default:  return false;
	}

	Point dest = { pos_.x + dx, pos_.y + dy };

	// Destination must be in-bounds and walkable, the skipped middle cell is irrelevant.
	if (!map.inBounds(dest.x, dest.y) || !map.isWalkable(dest.x, dest.y))
		return false;

	pos_ = dest;
	return true;
}

// Knight
Knight::Knight(Point pos) : Hero(pos) {}

bool Knight::useSpecialAbility(Map& /*map*/,
	std::vector<std::unique_ptr<Enemy>>& enemies,
	char /*direction*/)
{
	const int dx[] = { -1, 1,  0, 0 };
	const int dy[] = { 0, 0, -1, 1 };

	bool killed = false;
	for (int k = 0; k < 4; ++k) {
		Point adj = { pos_.x + dx[k], pos_.y + dy[k] };
		for (auto& e : enemies) {
			if (e->isAlive() && e->getPosition() == adj) {
				e->kill();
				killed = true;
			}
		}
	}
	return killed;
}
