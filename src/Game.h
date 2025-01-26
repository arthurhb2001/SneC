#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _HAS_EXCEPTIONS 1

#include <SDL3/SDL.h>
#include <vector>
#include <random>
#include "Score.h"

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Position 
{
    int x;
    int y;
    bool operator==(const Position& other) const noexcept 
    {
        return (x == other.x) && (y == other.y);
    }
};

class Game 
{
private:
    static constexpr int kScreenWidth{ 800 };
    static constexpr int kScreenHeight{ 600 };
    static constexpr int kGridSize{ 20 };
    static constexpr int kInitialSnakeLength{ 4 };
    
    SDL_Window* m_window{ nullptr };
    SDL_Surface* m_screenSurface{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
    
    std::vector<Position> m_snake;
    Position m_apple;
    Direction m_direction{ Direction::RIGHT };
    bool m_gameOver{ false };
    std::random_device m_rd;
    std::mt19937 m_gen;
    Score m_scoreManager;
    int m_currentScore{ 0 };

    void SpawnApple();
    void UpdateSnake();
    void HandleInput(SDL_Event& e);
    void Render();

public:
    Game();
    ~Game();

    bool Initialize();
    bool LoadMedia();
    void Close();
    void Run();
};
