#include "Enemy.h"
#include <queue>
#include <vector>
#include <random>


// Shared terrain helper
static bool isWalkable(int x, int y, const Board& board) {
	int n = static_cast<int>(board.size());
	return x >= 0 && x < n && y >= 0 && y < n && board[y][x] != '*';
}

// Moves enemyPos one step toward heroPos; returns false if no path exists.
static bool tryMoveTowardsHeroBFS(Point& enemyPos,
	const Point& heroPos,
	const Board& board)
{
	int n = static_cast<int>(board.size());
	int W = n;
	std::vector<int> parent(W * W, -1);
	std::queue<Point> q;

	int start = enemyPos.y * W + enemyPos.x;
	int goal = heroPos.y * W + heroPos.x;
	parent[start] = start;
	q.push(enemyPos);

	const int dx[4] = { -1, 1,  0, 0 };
	const int dy[4] = { 0, 0, -1, 1 };

	bool found = false;
	while (!q.empty()) {
		Point cur = q.front();
		q.pop();
		int curI = cur.y * W + cur.x;
		if (curI == goal) { found = true; break; }
		for (int k = 0; k < 4; ++k) {
			int nx = cur.x + dx[k];
			int ny = cur.y + dy[k];
			if (!isWalkable(nx, ny, board)) continue;
			int ni = ny * W + nx;
			if (parent[ni] != -1) continue;
			parent[ni] = curI;
			q.push({ nx, ny });
		}
	}

	if (!found) return false;

	// Trace back one step from goal to the cell right after start
	int cur = goal;
	while (parent[cur] != start && cur != start)
		cur = parent[cur];
	if (cur == start) return false;

	enemyPos.x = cur % W;
	enemyPos.y = cur / W;
	return true;
}

SmartEnemy::SmartEnemy(Point pos) : Enemy(pos, 'E') {}

void SmartEnemy::move(const Point& heroPos, const Board& board) {
	if (!alive_) return;
	tryMoveTowardsHeroBFS(pos_, heroPos, board);
}

RandomEnemy::RandomEnemy(Point pos) : Enemy(pos, 'E') {}

void RandomEnemy::move(const Point& heroPos, const Board& board) {
	if (!alive_) return;

	const int dx[] = { -1, 1,  0, 0 };
	const int dy[] = { 0, 0, -1, 1 };

	std::vector<Point> valid;
	for (int k = 0; k < 4; ++k) {
		int nx = pos_.x + dx[k];
		int ny = pos_.y + dy[k];
		if (isWalkable(nx, ny, board))
			valid.push_back({ nx, ny });
	}
	if (valid.empty()) return;

	// Thread-safe, seeded once on first call
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(0, static_cast<int>(valid.size()) - 1);
	pos_ = valid[dist(rng)];
}
