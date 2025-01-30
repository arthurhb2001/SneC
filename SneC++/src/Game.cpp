#include "Game.h"
#include <iostream>
#include <random>
#include <algorithm>
 
Game::Game() : m_gen(m_rd()), m_currentScore{0} {}

Game::~Game() 
{ 
    Close(); 
}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow("SneC++", kScreenWidth, kScreenHeight, 0);
    if (!m_window)
    {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer)
    {
        SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
        return false;
    }

    int startX = (kScreenWidth / kGridSize) / 2;
    int startY = (kScreenHeight / kGridSize) / 2;
    
    for (int i = 0; i < kInitialSnakeLength; ++i)
    {
        m_snake.push_back({ startX - i, startY });
    }
    
    SpawnApple();
    return true;
}

void Game::SpawnApple()
{
    std::uniform_int_distribution<> disX(0, (kScreenWidth / kGridSize) - 1);
    std::uniform_int_distribution<> disY(0, (kScreenHeight / kGridSize) - 1);
    
    do
    {
        m_apple = { disX(m_gen), disY(m_gen) };
    }
    while (std::find(m_snake.begin(), m_snake.end(), m_apple) != m_snake.end());
}

void Game::UpdateSnake()
{
    Position newHead = m_snake.front();
    
    switch (m_direction)
    {
        case Direction::UP: newHead.y--; break;
        case Direction::DOWN: newHead.y++; break;
        case Direction::LEFT: newHead.x--; break;
        case Direction::RIGHT: newHead.x++; break;
    }
    
    if (newHead.x < 0)
        newHead.x = (kScreenWidth / kGridSize) - 1;
    else if (newHead.x >= kScreenWidth / kGridSize)
        newHead.x = 0;
    
    if (newHead.y < 0)
        newHead.y = (kScreenHeight / kGridSize) - 1;
    else if (newHead.y >= kScreenHeight / kGridSize)
        newHead.y = 0;
    
    if (std::find(m_snake.begin(), m_snake.end(), newHead) != m_snake.end())
    {
        m_gameOver = true;
        return;
    }
    
    m_snake.insert(m_snake.begin(), newHead);
    
    if (newHead == m_apple)
    {
        SpawnApple();
        m_currentScore += 100;
    }
    else
    {
        m_snake.pop_back();
    }
}

void Game::HandleInput(SDL_Event& e)
{
    if (e.type == SDL_EVENT_KEY_DOWN)
    {
        switch (e.key.key)
        {
            case SDLK_UP:
                if (m_direction != Direction::DOWN) m_direction = Direction::UP;
                break;
            case SDLK_DOWN:
                if (m_direction != Direction::UP) m_direction = Direction::DOWN;
                break;
            case SDLK_LEFT:
                if (m_direction != Direction::RIGHT) m_direction = Direction::LEFT;
                break;
            case SDLK_RIGHT:
                if (m_direction != Direction::LEFT) m_direction = Direction::RIGHT;
                break;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
    
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect rect{ 0, 0, kGridSize, kGridSize };
    for (const auto& pos : m_snake)
    {
        rect.x = pos.x * kGridSize;
        rect.y = pos.y * kGridSize;
        SDL_RenderFillRect(m_renderer, &rect);
    }
    
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    rect.x = m_apple.x * kGridSize;
    rect.y = m_apple.y * kGridSize;
    SDL_RenderFillRect(m_renderer, &rect);
    
    SDL_RenderPresent(m_renderer);
}

void Game::Close()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

void Game::Run()
{
    bool quit = false;
    SDL_Event e;
    Uint32 lastUpdateTime = SDL_GetTicks();
    const Uint32 updateInterval = 100;

    while (!quit && !m_gameOver)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            HandleInput(e);
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdateTime >= updateInterval)
        {
            UpdateSnake();
            lastUpdateTime = currentTime;
        }

        Render();
    }

    std::cout << "\nGame Over!\n";
    std::cout << "Final Score: " << m_currentScore << "\n\n";
    
    const auto& highScores = m_scoreManager.GetHighScores();
    if (m_currentScore > 0) {
        if (!highScores.empty() && m_currentScore > highScores[0]) {
            std::cout << "********************************\n";
            std::cout << "* CONGRATULATIONS! TOP SCORE!! *\n";
            std::cout << "********************************\n\n";
        }
        else if (m_scoreManager.IsHighScore(m_currentScore)) {
            std::cout << "NEW HIGH SCORE!\n\n";
        }
        m_scoreManager.AddScore(m_currentScore);
    }
    
    std::cout << "Top 10 High Scores:\n";
    for (size_t i = 0; i < highScores.size(); ++i) {
        if (highScores[i] > 0) {
            std::cout << (i + 1) << ". " << highScores[i] << "\n";
        }
    }
}
