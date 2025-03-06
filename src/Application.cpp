#include <Application.hpp>

#include <stdexcept>

namespace snake {

Application::Application() {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		throw std::runtime_error("Failed to init SDL!");

	SDL_CreateWindowAndRenderer(
		"Snake", 
		constants::tileSize * constants::boardWidth, constants::tileSize * constants::boardHeight, 
		SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE, 
		&m_window, &m_renderer
	);

	SDL_SetRenderLogicalPresentation(
		m_renderer, 
		constants::tileSize * constants::boardWidth, constants::tileSize * constants::boardHeight, 
		SDL_LOGICAL_PRESENTATION_LETTERBOX
	);
}

Application::~Application() {
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

void Application::run() {
	m_beginTime = SDL_GetTicks();

	while (m_running) {
		updateTimeStep();

		while (m_accumulator >= constants::deltaTime)
			update();

		render();
	}
}

void Application::updateTimeStep() {
	auto currTime = SDL_GetTicks();
	m_accumulator += currTime - m_beginTime;
	m_beginTime = currTime;
}

void Application::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_KEY_DOWN:
				if (m_deathPauseTimer != 0) continue;

				switch (event.key.scancode) {
					case SDL_SCANCODE_W: case SDL_SCANCODE_UP:
						m_paused = false;
						m_snake.checkAndSetDirection(Direction::up, Direction::down);

						break;
					
					case SDL_SCANCODE_S: case SDL_SCANCODE_DOWN:
						m_paused = false;
						m_snake.checkAndSetDirection(Direction::down, Direction::up);

						break;

					case SDL_SCANCODE_A: case SDL_SCANCODE_LEFT:
						m_paused = false;
						m_snake.checkAndSetDirection(Direction::left, Direction::right);

						break;

					case SDL_SCANCODE_D: case SDL_SCANCODE_RIGHT:
						m_paused = false;
						m_snake.checkAndSetDirection(Direction::right, Direction::left);
						
						break;

					case SDL_SCANCODE_ESCAPE:
						m_paused = true;

					default:;
				}

				break;

			case SDL_EVENT_QUIT:
				m_running = false;
		}
	}

	if (!m_paused && m_snake.move()) {
		if (!m_snake.updateAndCheckCollisions()) {
			m_paused = true;
			m_deathPauseTimer = constants::deathPauseTime;

			return;
		}

		if (m_snake.ateFood(m_food))
			m_board.newFood(m_food);

		if (!m_board.updateAndCheckCollisions(m_snake)) {
			m_paused = true;
			m_deathPauseTimer = constants::deathPauseTime;

			return;
		}
	}

	if (m_deathPauseTimer > 0) {
		if (constants::deltaTime >= m_deathPauseTimer) {
			m_deathPauseTimer = 0;

			m_snake = Snake { };
			m_food = Food { };
			m_board.clear();
		} else m_deathPauseTimer -= constants::deltaTime;
	} else m_snake.placeTile(m_board);
	
	m_accumulator -= constants::deltaTime;
}

void Application::render() {
	m_food.placeTile(m_board);

	SDL_SetRenderDrawColor(
		m_renderer,
		constants::tileColors[0][0], 
		constants::tileColors[0][1], 
		constants::tileColors[0][2], 
		255);
	SDL_RenderClear(m_renderer);

	// Drawing acts as SDL_RenderClear since everything gets drawn over anyways
	bool verticalColorFlip = true;
	for (std::size_t y = 0; y < constants::boardHeight; y++) {
		bool horizontalColorFlip = verticalColorFlip;

		for (std::size_t x = 0; x < constants::boardWidth; x++) {
			switch (m_board[y * constants::boardWidth + x].mode) {
				case Board::TileMode::body:
					SDL_SetRenderDrawColor(
						m_renderer,
						constants::bodyColor[0], 
						constants::bodyColor[1], 
						constants::bodyColor[2], 
						255);

					break;

				case Board::TileMode::food:
					SDL_SetRenderDrawColor(
						m_renderer,
						constants::foodColor[0], 
						constants::foodColor[1], 
						constants::foodColor[2], 
						255);

					break;
				
				default:
					if (horizontalColorFlip) {
						horizontalColorFlip = !horizontalColorFlip;
						continue;
					} else SDL_SetRenderDrawColor(
						m_renderer,
						constants::tileColors[1][0], 
						constants::tileColors[1][1], 
						constants::tileColors[1][2], 
						255);
			}

			SDL_FRect rect {
				static_cast<float>(constants::tileSize * x),
				static_cast<float>(constants::tileSize * y),
				static_cast<float>(constants::tileSize),
				static_cast<float>(constants::tileSize)
			};

			SDL_RenderFillRect(m_renderer, &rect);

			horizontalColorFlip = !horizontalColorFlip;
		}

		verticalColorFlip = !verticalColorFlip;
	}

	SDL_RenderPresent(m_renderer);
}

} // namespace snake
