#pragma once
#include "Point.h"

// Abstract base for every placeable game object (Hero, Enemy, etc.)
class Entity {
public:
	Entity(Point pos, char symbol) : pos_(pos), symbol_(symbol) {}
	virtual ~Entity() = default;

	const Point& getPosition() const { return pos_; }
	void         setPosition(const Point& p) { pos_ = p; }
	char         getSymbol()   const { return symbol_; }

protected:
	Point pos_;
	char  symbol_;
};
