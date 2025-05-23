#pragma once
#include "Player.h" // Player must be fully defined first
#include <queue>
#include <set>
#include "Constants.h" // For BOARD_SIZE_CONST

// BoardPos struct should be fine here or in NetworkCommon.h if used by client too
struct BoardPos {
    int r, c;
    bool operator<(const BoardPos& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
};

class ComputerPlayer : public Player {
private:
    std::queue<BoardPos> smartTargetQueue;
    std::set<BoardPos> attemptedMoves;

public:
    ComputerPlayer(const std::string& name = "Computer");
    void strategizeAfterHit(int r, int c, const Player& opponent);
    bool makeStrategicMove(Player& opponent, int& outRow, int& outCol) override; // <<< Added override
    void resetComputerLogic();
};