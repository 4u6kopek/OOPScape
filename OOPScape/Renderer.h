#pragma once
#include "Map.h"
#include "Hero.h"
#include "Enemy.h"
#include <string>
#include <vector>
#include <memory>


class Renderer {
public:
	// Clears the screen and redraws the full game state.
	static void render(const Map& map,
		const Hero& hero,
		const std::vector<std::unique_ptr<Enemy>>& enemies,
		int  currentLevel,
		int  totalLevels,
		const std::string& statusMsg = "",
		int                statusColor = CLR_DEFAULT);

	static void printColored(const std::string& text, int colorAttr);
	static void clearScreen();

	// Windows foreground color attribute constants.
	static constexpr int CLR_DEFAULT = 7;   // Gray
	static constexpr int CLR_WALL = 8;   // Dark Gray
	static constexpr int CLR_HERO_WIZ = 11;   // Cyan  (Wizard)
	static constexpr int CLR_HERO_KNT = 15;   // Bright White (Knight)
	static constexpr int CLR_ENEMY_S = 12;   // Bright Red   (SmartEnemy)
	static constexpr int CLR_ENEMY_R = 13;   // Magenta      (RandomEnemy)
	static constexpr int CLR_EXIT = 14;   // Bright Yellow
	static constexpr int CLR_COIN = 6;   // Dark Yellow
	static constexpr int CLR_WIN = 10;   // Bright Green
	static constexpr int CLR_LOSE = 12;   // Bright Red

private:
	static void setColor(int attr);
	static void resetColor();
};
