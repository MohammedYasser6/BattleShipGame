// Player.cpp
#include "Player.h" 
#include <cstdlib>   // For rand()
#include <vector>    // For std::vector

Player::Player(const std::string& name) : playerName(name) {
    initializeBoards();
}

const std::string& Player::getName() const {
    return playerName;
}

void Player::setName(const std::string& name) {
    this->playerName = name;
}

void Player::initializeBoards() {
    for (int i = 0; i < BOARD_SIZE_CONST; ++i) {
        for (int j = 0; j < BOARD_SIZE_CONST; ++j) {
            ownBoard[i][j] = WATER_CHAR;
            trackingBoard[i][j] = HIDDEN_CHAR; // Initialize tracking board too
        }
    }
}

void Player::addShipDefinition(const std::string& name, int size) {
    ships.emplace_back(name, size); // Creates a new Ship object with given name and size
}

// Places a ship from the 'ships' vector at the given index
bool Player::placeShip(int shipIndex, int r, int c, bool isHorizontal) {
    if (shipIndex < 0 || static_cast<size_t>(shipIndex) >= ships.size()) return false;
    Ship& currentShip = ships[shipIndex];

    // Ensure the ship object is clean for placement (no previous cell data)
    // This should be handled by how Ship objects are managed, esp. after Player::resetPlayer
    // or before calling placeShip in a loop like in placeShipsRandomly.
    // Calling clearCells here makes sure.
    currentShip.clearCells();

    std::vector<CellCoordinate> tempCellPositions;
    int shipSize = currentShip.getSize();

    if (isHorizontal) {
        if (c + shipSize > BOARD_SIZE_CONST) return false;
        for (int j = 0; j < shipSize; ++j) {
            if (ownBoard[r][c + j] != WATER_CHAR) return false;
            tempCellPositions.emplace_back(r, c + j);
        }
    }
    else {
        if (r + shipSize > BOARD_SIZE_CONST) return false;
        for (int i = 0; i < shipSize; ++i) {
            if (ownBoard[r + i][c] != WATER_CHAR) return false;
            tempCellPositions.emplace_back(r + i, c);
        }
    }

    for (const auto& cellPos : tempCellPositions) {
        ownBoard[cellPos.row][cellPos.col] = SHIP_CHAR;
        currentShip.addCellPos(cellPos.row, cellPos.col);
    }
    return true;
}

void Player::placeShipsRandomly() {
    // Assumes ships vector contains Ship objects (definitions) ready to be placed.
    // Player::resetPlayer ensures these are fresh objects.
    for (size_t i = 0; i < ships.size(); ++i) {
        ships[i].clearCells(); // Good practice to ensure it's unplaced
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 200) {
            int r_coord = rand() % BOARD_SIZE_CONST;
            int c_coord = rand() % BOARD_SIZE_CONST;
            bool isHorizontal = (rand() % 2 == 0);

            if (placeShip(static_cast<int>(i), r_coord, c_coord, isHorizontal)) {
                placed = true;
            }
            attempts++;
        }
        // if (!placed) { /* Optionally handle ships that couldn't be placed */ }
    }
}

char Player::getOwnBoardCell(int r, int c) const {
    if (r >= 0 && r < BOARD_SIZE_CONST && c >= 0 && c < BOARD_SIZE_CONST) {
        return ownBoard[r][c];
    }
    return ' ';
}

char Player::getTrackingBoardCell(int r, int c) const {
    if (r >= 0 && r < BOARD_SIZE_CONST && c >= 0 && c < BOARD_SIZE_CONST) {
        return trackingBoard[r][c];
    }
    return ' ';
}

const std::vector<Ship>& Player::getAllShips() const {
    return ships;
}

// This player is being attacked at (r,c)
char Player::receiveAttack(int r, int c) {
    if (r < 0 || r >= BOARD_SIZE_CONST || c < 0 || c >= BOARD_SIZE_CONST) {
        return ' '; // Invalid coordinate
    }
    if (ownBoard[r][c] == SHIP_CHAR) {
        ownBoard[r][c] = HIT_CHAR; // Update board display
        for (auto& ship : ships) { // Update ship object state
            if (ship.attemptHit(r, c)) {
                break;
            }
        }
        return HIT_CHAR;
    }
    else if (ownBoard[r][c] == WATER_CHAR) {
        ownBoard[r][c] = MISS_CHAR;
        return MISS_CHAR;
    }
    return ownBoard[r][c]; // Cell was already hit or missed
}

// This player made an attack, and this is the result on the opponent
bool Player::processAttackResult(int r, int c, char result, Player& opponent) {
    (void)opponent; // Opponent object not directly used here.

    if (r < 0 || r >= BOARD_SIZE_CONST || c < 0 || c >= BOARD_SIZE_CONST) {
        return false;
    }
    if (trackingBoard[r][c] != HIDDEN_CHAR) {
        return false; // Already targeted this cell
    }

    if (result == HIT_CHAR) {
        trackingBoard[r][c] = HIT_CHAR;
    }
    else if (result == MISS_CHAR) {
        trackingBoard[r][c] = MISS_CHAR;
    }
    else if (result == 'S') { // 'S' for SUNK (from game logic)
        trackingBoard[r][c] = HIT_CHAR; // Mark as hit, UI might color differently for SUNK
    }
    else {
        return false; // Unknown result
    }
    return true;
}

bool Player::isDefeated() const {
    if (ships.empty()) return true; // No ships = defeated by default in a game context
    for (const auto& ship : ships) {
        if (!ship.isSunk()) {
            return false;
        }
    }
    return true;
}

void Player::resetPlayer() {
    initializeBoards();
    // Create new Ship objects from the name/size data stored in the current Ship objects.
    // This effectively resets them to unplaced state.
    std::vector<Ship> freshShips;
    freshShips.reserve(ships.size());
    for (const auto& existingShipDefinition : ships) {
        freshShips.emplace_back(existingShipDefinition.getName(), existingShipDefinition.getSize());
    }
    ships = freshShips;
}

std::string Player::getOwnBoardAsString() const {
    std::string s = "";
    s.reserve(BOARD_SIZE_CONST * BOARD_SIZE_CONST);
    for (int i = 0; i < BOARD_SIZE_CONST; ++i) {
        for (int j = 0; j < BOARD_SIZE_CONST; ++j) {
            s += ownBoard[i][j];
        }
    }
    return s;
}

std::string Player::getTrackingBoardAsString() const {
    std::string s = "";
    s.reserve(BOARD_SIZE_CONST * BOARD_SIZE_CONST);
    for (int i = 0; i < BOARD_SIZE_CONST; ++i) {
        for (int j = 0; j < BOARD_SIZE_CONST; ++j) {
            s += trackingBoard[i][j];
        }
    }
    return s;
}

// Used by Client to update its own board display based on data from Host
void Player::setOwnBoardFromString(const std::string& boardStr) {
    if (boardStr.length() == BOARD_SIZE_CONST * BOARD_SIZE_CONST) {
        int k = 0;
        for (int i = 0; i < BOARD_SIZE_CONST; ++i) {
            for (int j = 0; j < BOARD_SIZE_CONST; ++j) {
                char receivedCellState = boardStr[k++];
                // If the cell on server shows hit but was a ship part, update local ship objects.
                // This is a simplified update; a full sync would involve updating Ship objects' hit status.
                if (ownBoard[i][j] == SHIP_CHAR && receivedCellState == HIT_CHAR) {
                    for (auto& ship : ships) {
                        ship.attemptHit(i, j); // Try to mark this part of ship as hit
                    }
                }
                ownBoard[i][j] = receivedCellState;
            }
        }
    }
}

// May not be strictly needed if client redraws tracking board fully from host's ownBoardString
void Player::setTrackingBoardCell(int r, int c, char val) {
    if (r >= 0 && r < BOARD_SIZE_CONST && c >= 0 && c < BOARD_SIZE_CONST) {
        trackingBoard[r][c] = val;
    }
}