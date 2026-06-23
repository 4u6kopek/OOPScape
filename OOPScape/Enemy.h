#pragma once
#include "Entity.h"

// Abstract base for all autonomous enemies.
class Enemy : public Entity {
public:
	explicit Enemy(Point pos, char symbol = 'E')
		: Entity(pos, symbol) {
	}
	virtual ~Enemy() = default;

	// Moves this enemy exactly one step according to its strategy.
	virtual void move(const Point& heroPos, const Board& board) = 0;

	// Used by the Renderer to pick a display color.
	virtual bool isSmart() const = 0;

	bool isAlive() const { return alive_; }
	void kill() { alive_ = false; }

protected:
	bool alive_ = true;
};


// Pursues the hero via BFS shortest-path (from the assignment).
class SmartEnemy final : public Enemy {
public:
	explicit SmartEnemy(Point pos);
	void move(const Point& heroPos, const Board& board) override;
	bool isSmart() const override { return true; }
};


// Moves to a random valid adjacent cell each turn.
class RandomEnemy final : public Enemy {
public:
	explicit RandomEnemy(Point pos);
	void move(const Point& heroPos, const Board& board) override;
	bool isSmart() const override { return false; }
};
