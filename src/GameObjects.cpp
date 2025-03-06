#include <GameObjects.hpp>

namespace snake {

// Food

void Food::placeTile(Board& board) {
	board[y * constants::boardWidth + x] = Board::Tile { Board::TileMode::food, 0 };
}


// Snake

bool Snake::move() noexcept {
	if (m_direction == Direction::none || ++m_currentFrame < (constants::startingSpeed - m_speedUp)) return false;

	m_currentFrame = 0;

	switch (m_direction) {
		case Direction::up:
			m_y -= 1;

			break;

		case Direction::down:
			m_y += 1;
			
			break;

		case Direction::left:
			m_x -= 1;
			
			break;

		case Direction::right:
			m_x += 1;
			
			break;

		default:;
	}

	m_lastDirection = m_direction;
	if (m_directionQueue != Direction::none)
		popDirectionQueue();
	else
		m_directionQueue = Direction::none;

	return true;
}

bool Snake::updateAndCheckCollisions() noexcept {
	if (m_x < 0 || m_x >= constants::boardWidth || m_y < 0 || m_y >= constants::boardHeight)
		return false;
	
	return true;
}

bool Snake::ateFood(const Food& food) noexcept {
	grew = false;

	// Check food collision
	if (collided(food.x, food.y)) {
		grew = true;
		++m_length;

		if (m_speedUp = static_cast<std::size_t>(constants::snakeSpeedFactor * (m_length - constants::startingLength)); m_speedUp >= constants::startingSpeed)
			m_speedUp = constants::startingSpeed;

		return true;
	}

	return false;
}

void Snake::placeTile(Board& board) {
	board[m_y * constants::boardWidth + m_x] = Board::Tile { Board::TileMode::body, m_length };
}

void Snake::checkAndSetDirection(Direction dir, Direction inv) noexcept {
	if (m_lastDirection != inv && m_direction != inv) {
		if (m_directionQueue != Direction::none && m_directionQueue != inv)
			popDirectionQueue(dir);
		else
			setDirectionAndQueue(dir);
	} else if (m_direction != inv)
		m_directionQueue = dir;
}

bool Snake::collided(int x, int y) const noexcept {
	return m_x == x && m_y == y;
}

void Snake::setDirectionAndQueue(Direction newDir) noexcept {
	m_direction = newDir;
	m_directionQueue = newDir;
}

void Snake::popDirectionQueue(Direction newDir) noexcept {
	m_direction = m_directionQueue;
	m_directionQueue = newDir;
}


// Game board

Board::Board() : m_gen(m_rand()), m_dist(0, 0) {
	m_emptyTiles.reserve(constants::boardWidth * constants::boardHeight);
}

bool Board::updateAndCheckCollisions(const Snake& snake) {
	m_emptyTiles.clear();

	for (int tileIndex = 0; auto& tile : m_board) {
		if (tile.mode == TileMode::body) {
			// Collision
			if (snake.collided(tileIndex % constants::boardWidth, tileIndex / constants::boardWidth))
				return false;

			// Body lifespan check
			if (!snake.grew && --tile.lifeSpan == 0)
				tile.mode = TileMode::empty;
		} else if (tile.mode != TileMode::food) m_emptyTiles.push_back(tileIndex);

		++tileIndex;
	}

	return true;
}

bool Board::newFood(Food& food) {
	if (m_emptyTiles.empty()) return false;

	(*this)[food.y * constants::boardWidth + food.x] = Tile { TileMode::empty, 0 };

	m_dist.param(std::uniform_int_distribution<std::size_t>::param_type { 0, m_emptyTiles.size() });
	std::size_t pos = m_emptyTiles[m_dist(m_gen)];

	food.x = pos % constants::boardWidth;
	food.y = pos / constants::boardWidth;

	return true;
}

void Board::clear() {
	m_board = { };
	m_emptyTiles.clear();
}

Board::Tile& Board::operator[](std::size_t index) {
#ifdef NDEBUG
	return m_board[index];
#else
	return m_board.at(index);
#endif
}

const Board::Tile& Board::operator[](std::size_t index) const {
#ifdef NDEBUG
	return m_board[index];
#else
	return m_board.at(index);
#endif
}

} // namespace snake
