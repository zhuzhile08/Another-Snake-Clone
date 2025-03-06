/********************
 * @file GameObjects.hpp
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Game objects
 * 
 * @date 2025-02-20
 * @copyright Copyright (c) 2025
 ********************/

#pragma once

#include <Constants.hpp>

#include <random>

namespace snake {

class Board;
class Snake;
class Food;

class Food {
public:
	void placeTile(Board& board);

	int x = constants::boardWidth - 3;
	int y = constants::boardHeight / 2;
};

enum Direction {
	none,
	up,
	down,
	left,
	right
};

class Snake {
public:
	bool grew = false;

	bool move() noexcept;
	bool updateAndCheckCollisions() noexcept;
	bool ateFood(const Food& food) noexcept;

	void placeTile(Board& board);

	void checkAndSetDirection(Direction dir, Direction inv) noexcept;

	bool collided(int x, int y) const noexcept;

private:
	int m_x = constants::boardWidth / 2 - 1;
	int m_y = constants::boardHeight / 2;
	std::size_t m_length = constants::startingLength;

	Direction m_direction = Direction::none;
	Direction m_directionQueue = Direction::none;
	Direction m_lastDirection = Direction::none;

	std::size_t m_speedUp = 0;
	std::size_t m_currentFrame = constants::startingSpeed - 1;

	void setDirectionAndQueue(Direction newDir = Direction::none) noexcept;
	void popDirectionQueue(Direction newDir = Direction::none) noexcept;
};

class Board {
private:
	using distributer_type = std::uniform_int_distribution<std::size_t>;

public:
	enum TileMode {
		empty,
		body,
		food
	};

	struct Tile {
		TileMode mode = TileMode::empty;
		std::size_t lifeSpan = 0;
	};

	Board();

	bool updateAndCheckCollisions(const Snake& snake);
	bool newFood(Food& food);

	void clear();

	Tile& operator[](std::size_t index);
	const Tile& operator[](std::size_t index) const;

private:
	std::array<Tile, constants::boardWidth * constants::boardHeight> m_board;
	std::vector<std::size_t> m_emptyTiles;

	std::random_device m_rand;
	std::mt19937_64 m_gen;
	distributer_type m_dist;
};

} // namespace snake
