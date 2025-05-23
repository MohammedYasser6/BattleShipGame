// Player.h
#pragma once

// --- GAME CONSTANTS ---
const int BOARD_SIZE_CONST = 10;
const char WATER_CHAR = '~';
const char SHIP_CHAR = 'S';
const char HIT_CHAR = 'X';
const char MISS_CHAR = 'O';
const char HIDDEN_CHAR = '?';
// --- END GAME CONSTANTS ---

#include <string>
#include <vector>
#include "Ship.h" // Ship.h is included after constants are defined

class Player {
protected:
    std::string playerName;
    char ownBoard[BOARD_SIZE_CONST][BOARD_SIZE_CONST];
    char trackingBoard[BOARD_SIZE_CONST][BOARD_SIZE_CONST]; // Used by Host to track attacks on Client
    // Client does not use its own trackingBoard logic;
    // its tracking display is built from Host's ownBoard data.
    std::vector<Ship> ships;

public:
    Player(const std::string& name = "Player");
    virtual ~Player() = default;

    const std::string& getName() const;
    void setName(const std::string& name);

    void initializeBoards();
    void addShipDefinition(const std::string& name, int size);
    bool placeShip(int shipIndex, int r, int c, bool isHorizontal);
    void placeShipsRandomly();

    char getOwnBoardCell(int r, int c) const;
    char getTrackingBoardCell(int r, int c) const; // Primarily for Host
    const char (*getOwnBoardCells() const)[BOARD_SIZE_CONST] { return ownBoard; }

    const std::vector<Ship>& getAllShips() const;

    char receiveAttack(int r, int c); // Updates ownBoard based on attack
    bool processAttackResult(int r, int c, char result, Player& opponent); // Updates trackingBoard
    bool isDefeated() const;
    void resetPlayer(); // Resets boards and ships (re-creates ship objects for new placement)

    // Serialization/Deserialization methods
    std::string getOwnBoardAsString() const;
    std::string getTrackingBoardAsString() const; // Host might send this if client needed to reconstruct it
    void setOwnBoardFromString(const std::string& boardStr); // Client uses this to reflect server state
    void setTrackingBoardCell(int r, int c, char val); // Potentially for client to update its view, but direct redraw from string is simpler
};