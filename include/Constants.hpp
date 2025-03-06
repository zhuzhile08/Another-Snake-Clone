/********************
 * @file Constants.hpp
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Constants
 * 
 * @date 2025-02-20
 * @copyright Copyright (c) 2025
 ********************/

#pragma once

#include <cstdint>
#include <array>

namespace snake {

namespace constants {

inline constexpr int boardWidth = 20;
inline constexpr int boardHeight = 15;

inline constexpr int tileSize = 32;

inline constexpr std::uint64_t deltaTime = 11;

inline constexpr std::uint64_t deathPauseTime = 2000;

inline constexpr std::array<std::array<std::uint8_t, 3>, 2> tileColors = {{
	{{ 0x60, 0xd3, 0x94 }},
	{{ 0xaa, 0xf6, 0x83 }}
}};
inline constexpr std::array<std::uint8_t, 3> bodyColor = {{ 0xff, 0xd9, 0x7d }};
inline constexpr std::array<std::uint8_t, 3> foodColor = {{ 0xee, 0x60, 0x55 }};

inline constexpr std::size_t startingSpeed = 24;
inline constexpr std::size_t startingLength = 3;
inline constexpr float snakeSpeedFactor = 0.3f;

} // namespace constants

} // namespace snake
