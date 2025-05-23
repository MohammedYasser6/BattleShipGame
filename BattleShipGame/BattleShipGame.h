// BattleshipGame.h
#pragma once
#include "Player.h"       
#include <string>
#include <vector>
#include <memory>

enum class GameMode { PLAYER_VS_PLAYER };
enum class GameTurn { PLAYER1, PLAYER2, GAME_OVER_P1_WINS, GAME_OVER_P2_WINS, SETUP };

class BattleshipGameLogic {
private:
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    GameMode activeMode;
    GameTurn currentTurnState;
    std::vector<std::pair<std::string, int>> defaultShipConfig = {
        {"Carrier", 5}, {"Battleship", 4}, {"Cruiser", 3},
        {"Submarine", 3}, {"Destroyer", 2}
    };
    std::string lastActionMessage;
public:
    BattleshipGameLogic();
    void StartNewGame(const std::string& p1Name, const std::string& p2Name, GameMode mode = GameMode::PLAYER_VS_PLAYER);
    bool MakeAttack(int r, int c);
    GameTurn GetCurrentTurnState() const { return currentTurnState; }
    GameMode GetActiveMode() const { return activeMode; }
    const std::string& GetLastActionMessage() const { return lastActionMessage; }
    const Player* GetPlayer1() const { return player1.get(); }
    const Player* GetPlayer2() const { return player2.get(); }
    Player* GetPlayer1ForUpdate() { return player1.get(); }
    Player* GetPlayer2ForUpdate() { return player2.get(); }
    const Player* GetPlayerById(int playerId) const;
    Player* GetPlayerByIdForUpdate(int playerId);
    bool IsGameOver() const;
    std::string GetWinnerString() const;
};