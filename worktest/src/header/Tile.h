#pragma once

#include "king/Engine.h"
#include "Object.h"
#include "Position.h"
#include <iostream>

struct Bounds {
	Bounds() {}
	Bounds(Position topLeft, float sizeX, float sizeY) :
		mPos(topLeft),
		mSizeX(sizeX),
		mSizeY(sizeY) {
	}
	Position mPos;
	float mSizeX;
	float mSizeY;
};

struct Mark {
	const King::Engine::Texture mTex = King::Engine::TEXTURE_MARK;
	Position mPos = Position(0.f, 0.f);
	void Render() const { King::Engine::GetInstance().Render(mTex, mPos.get_x(), mPos.get_y(), NULL); }
};

class Tile {
public:
	Tile();
	Tile(const Tile &t);
	~Tile();

	void Init(const Position pos, Tile *above, Tile *below, Tile *left, Tile *right);
	void Render() const;

	void InsertObj(Object object);
	void RemoveObject();
	std::shared_ptr<Object> GetObj() const;
	void SwapObj(std::shared_ptr<Object> object);

	Position GetPosition() const;
	void SetPosition(const Position pos);

	bool MouseDetected(const Position &pos) const;
	void SetMark(bool value);
	bool Marked() const;

private:
	Mark mMark;
	bool mMarked;
	Bounds mBounds;
	Position mPosition;
	std::shared_ptr<Object> mObject;
	Tile* mTopNeighbour;
	Tile* mBottomNeighbour;
	Tile* mRightNeighbour;
	Tile* mLeftNeighbour;
	const King::Engine::Texture mTextureID = King::Engine::TEXTURE_TILE;
};