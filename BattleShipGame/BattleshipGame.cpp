// BattleshipGame.cpp
#include "BattleshipGame.h"
#include "Player.h" 
#include <cstdlib>   
#include <ctime>     
#include <sstream>   
#include <stdexcept> 

BattleshipGameLogic::BattleshipGameLogic() {
    currentTurnState = GameTurn::SETUP; activeMode = GameMode::PLAYER_VS_PLAYER;
    lastActionMessage = "Game not started. Waiting for PvP setup.";
}
void BattleshipGameLogic::StartNewGame(const std::string& p1Name, const std::string& p2Name, GameMode mode) {
    activeMode = GameMode::PLAYER_VS_PLAYER;
    player1 = std::make_unique<Player>(p1Name.empty() ? "Player 1" : p1Name);
    player2 = std::make_unique<Player>(p2Name.empty() ? "Player 2" : p2Name);
    for (const auto& conf : defaultShipConfig) {
        if (player1) player1->addShipDefinition(conf.first, conf.second);
        if (player2) player2->addShipDefinition(conf.first, conf.second);
    }
    if (player1) { player1->resetPlayer(); player1->placeShipsRandomly(); }
    if (player2) { player2->resetPlayer(); player2->placeShipsRandomly(); }
    currentTurnState = GameTurn::PLAYER1;
    if (player1) lastActionMessage = player1->getName() + "'s turn to attack.";
    else lastActionMessage = "Error: Player 1 not initialized.";
}
bool BattleshipGameLogic::MakeAttack(int r, int c) {
    if (IsGameOver()) { lastActionMessage = "Game is over. " + GetWinnerString(); return false; }
    Player* attacker = nullptr; Player* defender = nullptr; GameTurn nextTurnStateAfterAttack = GameTurn::SETUP;
    if (currentTurnState == GameTurn::PLAYER1) {
        attacker = player1.get(); defender = player2.get(); nextTurnStateAfterAttack = GameTurn::PLAYER2;
    }
    else if (currentTurnState == GameTurn::PLAYER2) {
        attacker = player2.get(); defender = player1.get(); nextTurnStateAfterAttack = GameTurn::PLAYER1;
    }
    else { lastActionMessage = "Invalid game state or not a player's turn for attack."; return false; }
    if (!attacker || !defender) { lastActionMessage = "Attacker or defender is missing."; return false; }
    if (r < 0 || r >= BOARD_SIZE_CONST || c < 0 || c >= BOARD_SIZE_CONST || attacker->getTrackingBoardCell(r, c) != HIDDEN_CHAR) {
        lastActionMessage = attacker->getName() + " made an invalid move at (" + std::to_string(r) + "," + std::to_string(c) + "). Cell already targeted or out of bounds. Try again."; return false;
    }
    char resultChar = defender->receiveAttack(r, c); attacker->processAttackResult(r, c, resultChar, *defender);
    std::string outcomeStr = ""; std::string sunkMsgDetail = "";
    if (resultChar == HIT_CHAR) {
        outcomeStr = "HIT";
        for (const auto& ship : defender->getAllShips()) {
            bool cellInThisShip = false;
            for (const auto& cell : ship.getCells()) if (cell.row == r && cell.col == c) { cellInThisShip = true; break; }
            if (cellInThisShip && ship.isSunk()) { outcomeStr = "SUNK"; sunkMsgDetail = (defender == player1.get()) ? " Sunk your " : " Sunk their "; sunkMsgDetail += ship.getName() + "!"; break; }
        }
    }
    else if (resultChar == MISS_CHAR) {
        outcomeStr = "MISS";
    }
    else { lastActionMessage = attacker->getName() + " made an invalid move at (" + std::to_string(r) + "," + std::to_string(c) + "). Cell already targeted or invalid state. Try again."; return false; }
    lastActionMessage = attacker->getName() + " attacked (" + std::to_string(r) + "," + std::to_string(c) + "): " + outcomeStr + "!" + sunkMsgDetail;
    if (player1->isDefeated()) {
        currentTurnState = GameTurn::GAME_OVER_P2_WINS; lastActionMessage += " " + GetWinnerString();
    }
    else if (player2->isDefeated()) {
        currentTurnState = GameTurn::GAME_OVER_P1_WINS; lastActionMessage += " " + GetWinnerString();
    }
    else { currentTurnState = nextTurnStateAfterAttack; Player* nextPlayer = (currentTurnState == GameTurn::PLAYER1) ? player1.get() : player2.get(); if (nextPlayer) lastActionMessage += " Now " + nextPlayer->getName() + "'s turn."; }
    return true;
}
bool BattleshipGameLogic::IsGameOver() const { if (!player1 || !player2) return true; return currentTurnState == GameTurn::GAME_OVER_P1_WINS || currentTurnState == GameTurn::GAME_OVER_P2_WINS || player1->isDefeated() || player2->isDefeated(); }
std::string BattleshipGameLogic::GetWinnerString() const {
    if (currentTurnState == GameTurn::GAME_OVER_P1_WINS && player1) return player1->getName() + " wins!"; if (currentTurnState == GameTurn::GAME_OVER_P2_WINS && player2) return player2->getName() + " wins!";
    if (player1 && player1->isDefeated() && player2) return player2->getName() + " wins!"; if (player2 && player2->isDefeated() && player1) return player1->getName() + " wins!"; return "Game Over!";
}
const Player* BattleshipGameLogic::GetPlayerById(int playerId) const { if (playerId == 1) return player1.get(); if (playerId == 2) return player2.get(); return nullptr; }
Player* BattleshipGameLogic::GetPlayerByIdForUpdate(int playerId) { if (playerId == 1) return player1.get(); if (playerId == 2) return player2.get(); return nullptr; }