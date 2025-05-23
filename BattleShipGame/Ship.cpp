// Ship.cpp
#include "Ship.h" 

Ship::Ship(const std::string& shipName, int s) : name(shipName), size(s), hitsTaken(0) {
    // Cells vector is initially empty.
}

const std::string& Ship::getName() const {
    return this->name;
}

int Ship::getSize() const {
    return this->size;
}

const std::vector<CellCoordinate>& Ship::getCells() const {
    return this->cells;
}

void Ship::addCellPos(int row, int col) {
    // Called during initial ship placement by Player::placeShip
    if (static_cast<int>(cells.size()) < this->size) {
        cells.emplace_back(row, col); // New CellCoordinate will have isHit = false
    }
}

bool Ship::attemptHit(int r, int c) {
    if (this->isSunk()) {
        return false;
    }
    for (auto& cell : cells) {
        if (cell.row == r && cell.col == c) {
            if (!cell.isHit) {
                cell.isHit = true;
                this->hitsTaken++;
                return true;
            }
            return false; // Already hit this part
        }
    }
    return false; // Not part of this ship
}

bool Ship::isSunk() const {
    if (this->size <= 0) return true;
    return (this->hitsTaken >= this->size);
}

void Ship::clearCells() {
    cells.clear();
    hitsTaken = 0;
}

// reset() now makes the ship "unplaced" and "unhit", ready for Player::placeShip.
void Ship::reset() {
    clearCells();
}