#pragma once
#include "Map.h"
#include "Hero.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <string>

// Orchestrates the full game: hero selection, level loading, turn loop,
// win/loss detection, and sequential level progression
class GameManager {
public:
	GameManager();
	void run();

private:
	std::vector<std::string>            levelFiles_;
	int                                 currentLevelIdx_ = 0;
	std::unique_ptr<Hero>               hero_;
	std::vector<std::unique_ptr<Enemy>> enemies_;
	Map                                 map_;

	// Status message carried into the next render call
	std::string statusMsg_;
	int         statusColor_ = 7;

	void selectHero();
	void loadLevel(int idx);
	void spawnEnemies(int levelIdx);
	void gameLoop();
	void moveEnemies();

	// Post-action state queries
	bool isOnExit()          const;
	bool hasEnemyCollision() const;
	void collectCoinIfAny();

	// Terminal screen helpers
	void handleWin();
	void handleLoss(const std::string& reason);
	void showFinalVictory();
	void pressEnterToContinue();
};
