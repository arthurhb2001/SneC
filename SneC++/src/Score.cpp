#include "Score.h"

Score::Score() {
    LoadHighScores();
}

void Score::LoadHighScores() {
    std::ifstream file(kHighScoreFile);
    int score;
    while (file >> score) {
        m_highScores.push_back(score);
    }
    if (m_highScores.empty()) {
        m_highScores.resize(kMaxHighScores, 0);
    }
    std::sort(m_highScores.begin(), m_highScores.end(), std::greater<int>());
}

void Score::SaveHighScores() {
    std::ofstream file(kHighScoreFile);
    for (int score : m_highScores) {
        file << score << '\n';
    }
}

bool Score::IsHighScore(int score) const {
    if (m_highScores.size() < kMaxHighScores) {
        return true;
    }
    return score > m_highScores.back();
}

void Score::AddScore(int score) {
    m_highScores.push_back(score);
    std::sort(m_highScores.begin(), m_highScores.end(), std::greater<int>());
    if (m_highScores.size() > kMaxHighScores) {
        m_highScores.resize(kMaxHighScores);
    }
    SaveHighScores();
}
