#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Score {
private:
    static constexpr int kMaxHighScores = 10;
    static constexpr const char* kHighScoreFile = "bin/highscores.txt";
    std::vector<int> m_highScores;

public:
    Score();
    void LoadHighScores();
    void SaveHighScores();
    bool IsHighScore(int score) const;
    void AddScore(int score);
    const std::vector<int>& GetHighScores() const { return m_highScores; }
};
