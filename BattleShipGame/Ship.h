// Ship.h
#pragma once
#include <string>
#include <vector>

// Constants are visible because Player.h (which defines them) includes Ship.h

struct CellCoordinate {
    int row;
    int col;
    bool isHit;
    CellCoordinate(int r, int c) : row(r), col(c), isHit(false) {}
};

class Ship {
private:
    std::string name;
    int size;
    std::vector<CellCoordinate> cells;
    int hitsTaken;

public:
    Ship(const std::string& name, int s);
    const std::string& getName() const;
    int getSize() const;
    const std::vector<CellCoordinate>& getCells() const; // Useful for checking ship location

    void addCellPos(int row, int col);
    bool attemptHit(int r, int c); // Returns true if it's a new hit on this ship part
    bool isSunk() const;
    void reset();      // Resets hitsTaken and clears cell positions.
    void clearCells(); // Specifically clears cell positions and hitsTaken.
};