#include "Map.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void Map::stripCR(std::string& s) {
	if (!s.empty() && s.back() == '\r') s.pop_back();
}

void Map::load(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Cannot open level file: " + filename);

	// Read grid size
	std::string line;
	if (!std::getline(file, line))
		throw std::runtime_error("Level file is empty: " + filename);
	stripCR(line);

	try {
		size_ = std::stoi(line);
	}
	catch (...) {
		throw std::runtime_error("First line of level file must be an integer n.");
	}
	if (size_ < 2 || size_ > 64)
		throw std::runtime_error("Level size n must satisfy 2 <= n <= 64.");

	// Read n x n grid
	board_.assign(size_, std::vector<char>(size_, ' '));
	enemyStarts_.clear();
	int sCount = 0, fCount = 0;

	for (int row = 0; row < size_; ++row) {
		if (!std::getline(file, line))
			throw std::runtime_error("Level file has fewer rows than declared.");
		stripCR(line);

		if (static_cast<int>(line.size()) != size_) {
			std::ostringstream oss;
			oss << "Row " << row << " has " << line.size()
				<< " characters; expected " << size_ << ".";
			throw std::runtime_error(oss.str());
		}

		for (int col = 0; col < size_; ++col) {
			char c = line[col];
			switch (c) {
			case '*': case ' ': case 'C':
				board_[row][col] = c;
				break;
			case 'F':
				board_[row][col] = 'F';
				++fCount;
				break;
			case 'S':
				board_[row][col] = ' ';
				heroStart_ = { col, row };
				++sCount;
				break;
			case 'E':
				board_[row][col] = ' ';
				enemyStarts_.push_back({ col, row });
				break;
			default:
				throw std::runtime_error(
					std::string("Invalid character '") + c +
					"' at row " + std::to_string(row) +
					", col " + std::to_string(col) + ".");
			}
		}
	}

	if (sCount != 1) throw std::runtime_error("Level must contain exactly one 'S'.");
	if (fCount != 1) throw std::runtime_error("Level must contain exactly one 'F'.");
}

bool Map::inBounds(int x, int y) const {
	return x >= 0 && x < size_ && y >= 0 && y < size_;
}

bool Map::isWalkable(int x, int y) const {
	return inBounds(x, y) && board_[y][x] != '*';
}
