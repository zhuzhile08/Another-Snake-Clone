cmake_minimum_required(VERSION 3.24.0)
project(Snake)

set(CMAKE_CXX_STANDARD 23)

set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build a static version of the library" FORCE)
add_subdirectory("sdl")

include_directories(Snake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Snake 
	"src/main.cpp"
	"src/Application.cpp"
	"src/GameObjects.cpp"
)

target_compile_options(Snake PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(Snake
LINK_PRIVATE
	SDL3::Headers
	SDL3::SDL3-static
)
