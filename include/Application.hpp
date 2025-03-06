/********************
 * @file Application.hpp
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Application class
 * 
 * @date 2025-02-20
 * @copyright Copyright (c) 2025
 ********************/

#pragma once

#include <SDL3/SDL.h>

#include <Constants.hpp>
#include <GameObjects.hpp>

namespace snake {

class Application {
public:
	Application();
	~Application();

	void run();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	std::uint64_t m_beginTime { };
	std::uint64_t m_accumulator { };

	bool m_running = true;
	bool m_paused = true;

	Board m_board { };
	Snake m_snake { };
	Food m_food { };

	std::uint64_t m_deathPauseTimer { };

	void update();
	void render();

	void updateTimeStep();
};

} // namespace snake
