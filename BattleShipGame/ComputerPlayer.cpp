#include "ComputerPlayer.h"
#include <cstdlib> // For rand

ComputerPlayer::ComputerPlayer(const std::string& name) : Player(name) {}

void ComputerPlayer::strategizeAfterHit(int r, int c, const Player& opponent) {
    (void)opponent;
    BoardPos hitPos = { r, c };
    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        BoardPos nextPos = { hitPos.r + dr[i], hitPos.c + dc[i] };
        if (nextPos.r >= 0 && nextPos.r < BOARD_SIZE_CONST &&
            nextPos.c >= 0 && nextPos.c < BOARD_SIZE_CONST &&
            this->getTrackingBoardCell(nextPos.r, nextPos.c) == HIDDEN_CHAR &&
            attemptedMoves.find(nextPos) == attemptedMoves.end()) {
            smartTargetQueue.push(nextPos);
        }
    }
}

bool ComputerPlayer::makeStrategicMove(Player& opponent, int& outRow, int& outCol) {
    BoardPos target;
    while (!smartTargetQueue.empty()) {
        target = smartTargetQueue.front();
        smartTargetQueue.pop();
        if (opponent.getOwnBoardCell(target.r, target.c) != HIT_CHAR &&
            opponent.getOwnBoardCell(target.r, target.c) != MISS_CHAR &&
            attemptedMoves.find(target) == attemptedMoves.end()) {

            outRow = target.r;
            outCol = target.c;
            attemptedMoves.insert(target);
            return true;
        }
    }

    int current_attempts = 0; // Renamed from 'attempts' to avoid conflict if there's a member var
    const int maxAttempts = BOARD_SIZE_CONST * BOARD_SIZE_CONST * 2;
    while (current_attempts < maxAttempts) {
        target.r = rand() % BOARD_SIZE_CONST;
        target.c = rand() % BOARD_SIZE_CONST;
        if (attemptedMoves.find(target) == attemptedMoves.end() &&
            opponent.getOwnBoardCell(target.r, target.c) != HIT_CHAR &&
            opponent.getOwnBoardCell(target.r, target.c) != MISS_CHAR) {
            outRow = target.r;
            outCol = target.c;
            attemptedMoves.insert(target);
            return true;
        }
        current_attempts++;
    }
    return false;
}

void ComputerPlayer::resetComputerLogic() {
    // Player::resetPlayer(); // Base class resetPlayer should be called by its own logic if needed
                           // Or if ComputerPlayer has specific needs beyond Player's reset.
                           // For now, just reset ComputerPlayer specific state.
    while (!smartTargetQueue.empty()) smartTargetQueue.pop();
    attemptedMoves.clear();
}