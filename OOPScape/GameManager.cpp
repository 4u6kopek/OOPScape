#include "GameManager.h"
#include "Renderer.h"
#include <iostream>
#include <cctype>
#include <stdexcept>


// Constructor

GameManager::GameManager()
	: levelFiles_{ "levels/level1.txt", "levels/level2.txt", "levels/level3.txt" }
{
}

// Public entry point
void GameManager::run() {
	Renderer::clearScreen();
	Renderer::printColored("\n  ╔══════════════════════════════╗\n", 11);
	Renderer::printColored("  ║          O O P S c a p e      ║\n", 11);
	Renderer::printColored("  ╚══════════════════════════════╝\n\n", 11);
	Renderer::printColored("  Escape the dungeon — avoid enemies, collect coins, reach F.\n\n", 7);

	selectHero();

	for (currentLevelIdx_ = 0;
		currentLevelIdx_ < static_cast<int>(levelFiles_.size());
		++currentLevelIdx_)
	{
		try {
			loadLevel(currentLevelIdx_);
		}
		catch (const std::exception& e) {
			Renderer::printColored(std::string("\n  [ERROR] ") + e.what() + "\n", 12);
			pressEnterToContinue();
			return;
		}

		gameLoop();

		if (!hero_) return;
	}

	showFinalVictory();
}

// Hero selection
void GameManager::selectHero() {
	std::cout << "  Choose your hero role:\n\n";
	Renderer::printColored("    [1] Wizard", 11);
	std::cout << "  —  Teleport: jump 2 cells, skipping any obstacle\n";
	Renderer::printColored("    [2] Knight", 15);
	std::cout << "  —  Slash: eliminate all adjacent enemies at once\n\n";
	std::cout << "  Enter 1 or 2: ";

	std::string input;
	std::getline(std::cin, input);

	Point dummy = { 0, 0 }; // real start position is set in loadLevel
	if (!input.empty() && input[0] == '2')
		hero_ = std::make_unique<Knight>(dummy);
	else
		hero_ = std::make_unique<Wizard>(dummy);

	std::cout << "\n  You chose: ";
	Renderer::printColored(hero_->getRoleName(),
		hero_->isWizard() ? Renderer::CLR_HERO_WIZ
		: Renderer::CLR_HERO_KNT);
	std::cout << "\n\n";
	pressEnterToContinue();
}

// Level loading
void GameManager::loadLevel(int idx) {
	map_.load(levelFiles_[idx]);
	hero_->setPosition(map_.getHeroStart());
	hero_->addScore(0); // score persists across levels
	spawnEnemies(idx);
	statusMsg_.clear();
}

// Distribute enemy types based on level index
void GameManager::spawnEnemies(int levelIdx) {
	enemies_.clear();
	const auto& starts = map_.getEnemyStarts();
	for (int i = 0; i < static_cast<int>(starts.size()); ++i) {
		bool smart = (levelIdx == 0) || (i % 2 == 0);
		if (smart)
			enemies_.push_back(std::make_unique<SmartEnemy>(starts[i]));
		else
			enemies_.push_back(std::make_unique<RandomEnemy>(starts[i]));
	}
}

// Core turn loop
void GameManager::gameLoop() {
	while (true) {
		Renderer::render(map_, *hero_, enemies_,
			currentLevelIdx_ + 1,
			static_cast<int>(levelFiles_.size()),
			statusMsg_, statusColor_);
		statusMsg_.clear();
		statusColor_ = Renderer::CLR_DEFAULT;

		std::cout << "  Command (L/R/U/D or OOP): ";
		std::string cmd;
		if (!std::getline(std::cin, cmd)) return;

		// Normalize to uppercase, trim leading and trailing whitespace
		std::string normalized;
		for (char c : cmd)
			if (!std::isspace(static_cast<unsigned char>(c)))
				normalized += static_cast<char>(
					std::toupper(static_cast<unsigned char>(c)));

		if (normalized == "L" || normalized == "R" ||
			normalized == "U" || normalized == "D")
		{
			int dx = (normalized == "R") - (normalized == "L");
			int dy = (normalized == "D") - (normalized == "U");
			Point np = { hero_->getPosition().x + dx,
						 hero_->getPosition().y + dy };

			if (!map_.isWalkable(np.x, np.y)) {
				statusMsg_ = "Blocked by a wall.";
				statusColor_ = Renderer::CLR_WALL;
				continue;
			}
			hero_->setPosition(np);
		}
		else if (normalized == "OOP") {
			char dir = 0;

			// Wizard needs a direction
			if (hero_->isWizard()) {
				Renderer::render(map_, *hero_, enemies_,
					currentLevelIdx_ + 1,
					static_cast<int>(levelFiles_.size()),
					"Teleport direction (L / R / U / D):",
					Renderer::CLR_HERO_WIZ);
				std::cout << "  Direction: ";
				std::string d;
				std::getline(std::cin, d);
				if (!d.empty())
					dir = static_cast<char>(
						std::toupper(static_cast<unsigned char>(d[0])));
			}

			bool ok = hero_->useSpecialAbility(map_, enemies_, dir);
			if (ok) {
				statusMsg_ = hero_->isWizard() ? "Teleported!" : "Slash! Enemy eliminated.";
				statusColor_ = hero_->isWizard() ? Renderer::CLR_HERO_WIZ
					: Renderer::CLR_HERO_KNT;
			}
			else {
				statusMsg_ = hero_->isWizard() ? "Cannot teleport there."
					: "No adjacent enemy to slash.";
				statusColor_ = Renderer::CLR_WALL;
				// The turn still passes if the ability is missused.
			}
		}
		else {
			statusMsg_ = "Unknown command. Use L / R / U / D or OOP.";
			statusColor_ = Renderer::CLR_WALL;
			continue;
		}

		if (isOnExit()) {
			handleWin();
			return;
		}
		if (hasEnemyCollision()) {
			handleLoss("You walked into an enemy!");
			return;
		}
		collectCoinIfAny();

		// Enemy turn
		moveEnemies();

		if (hasEnemyCollision()) {
			handleLoss("An enemy caught up with you!");
			return;
		}
	}
}

// Enemy movement
void GameManager::moveEnemies() {
	for (auto& e : enemies_)
		if (e->isAlive())
			e->move(hero_->getPosition(), map_.getBoard());
}

// State queries
bool GameManager::isOnExit() const {
	const Point& p = hero_->getPosition();
	return map_.getCell(p.x, p.y) == 'F';
}

bool GameManager::hasEnemyCollision() const {
	for (const auto& e : enemies_)
		if (e->isAlive() && e->getPosition() == hero_->getPosition())
			return true;
	return false;
}

// Collect coin
void GameManager::collectCoinIfAny() {
	const Point& p = hero_->getPosition();
	if (map_.getCell(p.x, p.y) == 'C') {
		map_.setCell(p.x, p.y, ' ');
		hero_->addScore(10);
		statusMsg_ = "Coin collected!  +10";
		statusColor_ = Renderer::CLR_COIN;
	}
}

// Terminal screens
void GameManager::handleWin() {
	hero_->addScore(50); // level clear bonus
	Renderer::render(map_, *hero_, enemies_,
		currentLevelIdx_ + 1,
		static_cast<int>(levelFiles_.size()),
		"Level cleared!  +50 bonus points!",
		Renderer::CLR_WIN);
	pressEnterToContinue();
}

void GameManager::handleLoss(const std::string& reason) {
	Renderer::render(map_, *hero_, enemies_,
		currentLevelIdx_ + 1,
		static_cast<int>(levelFiles_.size()),
		reason + "  Game Over.  Final score: " +
		std::to_string(hero_->getScore()),
		Renderer::CLR_LOSE);
	pressEnterToContinue();
	hero_.reset(); // signals run() that the game ended in defeat
}

void GameManager::showFinalVictory() {
	Renderer::clearScreen();
	Renderer::printColored("\n\n  ★  YOU ESCAPED THE DUNGEON!  ★\n\n", 14);
	Renderer::printColored("  Congratulations — all levels cleared!\n\n", 10);
	std::cout << "  Final Score: ";
	Renderer::printColored(std::to_string(hero_->getScore()) + "\n\n", 6);
	pressEnterToContinue();
}

void GameManager::pressEnterToContinue() {
	std::cout << "  Press Enter to continue...";
	std::string dummy;
	std::getline(std::cin, dummy);
}
