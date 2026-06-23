#pragma once
#include "Entity.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <string>

class Map; // forward declaration — only Map& is needed in the signature

// Abstract base hero. Concrete roles override the special ability.
class Hero : public Entity {
public:
	explicit Hero(Point pos) : Entity(pos, 'H') {}
	virtual ~Hero() = default;

	// Executes the hero's unique ability. Returns true on success.
	// 'direction' is meaningful for Wizard (L/R/U/D); Knight ignores it.
	virtual bool useSpecialAbility(Map& map,
		std::vector<std::unique_ptr<Enemy>>& enemies,
		char direction) = 0;

	virtual std::string getRoleName()           const = 0;
	virtual std::string getAbilityDescription() const = 0;

	// Used by Renderer and GameManager to choose display color.
	virtual bool isWizard() const = 0;

	int  getScore() const { return score_; }
	void addScore(int amount) { score_ += amount; }

protected:
	int score_ = 0;
};


class Wizard final : public Hero {
public:
	explicit Wizard(Point pos);

	bool useSpecialAbility(Map& map,
		std::vector<std::unique_ptr<Enemy>>& enemies,
		char direction) override;

	std::string getRoleName()           const override { return "Wizard"; }
	std::string getAbilityDescription() const override {
		return "Teleport (OOP): jump 2 cells in L/R/U/D, skipping any obstacle.";
	}
	bool isWizard() const override { return true; }
};


class Knight final : public Hero {
public:
	explicit Knight(Point pos);

	bool useSpecialAbility(Map& map,
		std::vector<std::unique_ptr<Enemy>>& enemies,
		char direction) override;

	std::string getRoleName()           const override { return "Knight"; }
	std::string getAbilityDescription() const override {
		return "Slash (OOP): eliminate all adjacent enemies (N/S/E/W).";
	}
	bool isWizard() const override { return false; }
};
