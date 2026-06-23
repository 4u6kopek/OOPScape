// Windows headers must come first to avoid macro conflicts.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

#include "Renderer.h"
#include <iostream>
#include <iomanip>

// Internal helpers
void Renderer::setColor(int attr) {
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
#else
	// Minimal ANSI fallback for non-Windows builds
	const char* codes[] = {
		"30","34","32","36","31","35","33","37",
		"90","94","92","96","91","95","93","97"
	};
	if (attr >= 0 && attr < 16)
		std::cout << "\033[" << codes[attr] << "m";
#endif
}

void Renderer::resetColor() { setColor(CLR_DEFAULT); }

void Renderer::clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	std::cout << "\033[2J\033[H" << std::flush;
#endif
}

void Renderer::printColored(const std::string& text, int colorAttr) {
	setColor(colorAttr);
	std::cout << text;
	resetColor();
}

// Main render function
void Renderer::render(const Map& map,
	const Hero& hero,
	const std::vector<std::unique_ptr<Enemy>>& enemies,
	int  currentLevel,
	int  totalLevels,
	const std::string& statusMsg,
	int                statusColor)
{
	clearScreen();
	int n = map.getSize();

	// Title / status bar
	setColor(15);
	std::cout << "\n  === OOPScape ===";
	resetColor();
	std::cout << "  Level " << currentLevel << "/" << totalLevels << "  |  Role: ";
	setColor(hero.isWizard() ? CLR_HERO_WIZ : CLR_HERO_KNT);
	std::cout << hero.getRoleName();
	resetColor();
	std::cout << "  |  Score: ";
	setColor(CLR_COIN);
	std::cout << hero.getScore();
	resetColor();
	std::cout << "\n\n";

	// Column header
	setColor(CLR_WALL);
	std::cout << "     ";
	for (int x = 0; x < n; ++x)
		std::cout << std::setw(2) << x;
	std::cout << "\n    +";
	for (int x = 0; x < n; ++x) std::cout << "--";
	std::cout << "\n";

	// Map rows
	for (int y = 0; y < n; ++y) {
		setColor(CLR_WALL);
		std::cout << " " << std::setw(2) << y << " |";

		for (int x = 0; x < n; ++x) {
			Point p = { x, y };

			// Hero takes rendering priority
			if (hero.getPosition() == p) {
				setColor(hero.isWizard() ? CLR_HERO_WIZ : CLR_HERO_KNT);
				std::cout << " H";
				continue;
			}

			// First alive enemy at this cell
			const Enemy* here = nullptr;
			for (const auto& e : enemies) {
				if (e->isAlive() && e->getPosition() == p) {
					here = e.get();
					break;
				}
			}
			if (here) {
				setColor(here->isSmart() ? CLR_ENEMY_S : CLR_ENEMY_R);
				std::cout << " E";
				continue;
			}

			// Terrain
			char c = map.getCell(x, y);
			switch (c) {
			case '*': setColor(CLR_WALL);    std::cout << " *"; break;
			case 'F': setColor(CLR_EXIT);    std::cout << " F"; break;
			case 'C': setColor(CLR_COIN);    std::cout << " C"; break;
			default:  setColor(CLR_DEFAULT); std::cout << "  "; break;
			}
		}
		resetColor();
		std::cout << "\n";
	}

	// Bottom border
	setColor(CLR_WALL);
	std::cout << "    +";
	for (int x = 0; x < n; ++x) std::cout << "--";
	resetColor();
	std::cout << "\n";

	// Ability hint 
	std::cout << "\n  ";
	setColor(hero.isWizard() ? CLR_HERO_WIZ : CLR_HERO_KNT);
	std::cout << hero.getAbilityDescription();
	resetColor();
	std::cout << "\n";

	// Status message (result of last action)
	if (!statusMsg.empty()) {
		std::cout << "  ";
		setColor(statusColor);
		std::cout << statusMsg;
		resetColor();
		std::cout << "\n";
	}
	std::cout << "\n";
}
