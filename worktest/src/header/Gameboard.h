#pragma once

#include "king/Engine.h"
#include "Position.h"
#include "Tile.h"
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> GridID;

struct MarkedTiles {
	bool hasFirst = false, hasSecond = false;
	GridID mMarkedTiles[2];
	void SetFront(const GridID id) { mMarkedTiles[0] = id; }
	void SetBack(const GridID id) { mMarkedTiles[1] = id; }
	GridID Front() const { return mMarkedTiles[0]; }
	GridID Back() const { return mMarkedTiles[1]; }
	void Clear() { mMarkedTiles[0] = GridID(NULL, NULL); mMarkedTiles[1] = GridID(NULL, NULL); hasFirst = false; hasSecond = false; }
};

class Gameboard {
public:
	Gameboard();
	Gameboard(Position pos, float offset);
	Gameboard(const Gameboard &gb);
	~Gameboard();

	void Render() const;

	void RestartBoard();
	void MarkTile(Position &mousePos);
	bool MarkSwapTarget(Position &mousePos);
	bool SwapObjects();
	bool CheckMatch(int &score);
	bool RefillBoard();
	bool DropObjects();
	bool RemoveObjects(int &score);
	void ResetMarks();

	const MarkedTiles& GetMarkedTiles() const;

private:
	void InitialiseBoard();
	void Populate();
	void Shuffle();
	
	void FindMatch(GridID tile, std::vector<GridID> &matches);
	void FindLowestEmpty(std::vector<GridID> matches);

	static const int Rows = 8;
	static const int Columns = 8;
	Tile mGrid[Rows][Columns]; 
	Position mPos;
	float mOffset;

	MarkedTiles mMarkedTiles;
	std::vector<GridID> mNeighbourTiles;
	std::vector<GridID> mLowestEmpty;
	Position Lerp(Position start, Position end, float percent) {
		return (start + ((end - start)*percent));
	}
};