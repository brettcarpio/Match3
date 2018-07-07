#include "Tile.h"

Tile::Tile() : 
	mPosition(0.f, 0.f),
	mObject(nullptr),
	mMarked(false),
	mTopNeighbour(nullptr),
	mBottomNeighbour(nullptr),
	mRightNeighbour(nullptr),
	mLeftNeighbour(nullptr){
}

Tile::Tile(const Tile &t) {

}

Tile::~Tile() {
	mObject.reset();
}

void Tile::Init(const Position pos, Tile *above, Tile *below, Tile *left, Tile *right) {
	mPosition = pos;
	mMark.mPos = mPosition;
	float halfTexWidth = King::Engine::GetInstance().GetTextureWidth(mTextureID) / 2;
	float halfTexHeight = King::Engine::GetInstance().GetTextureWidth(mTextureID) / 2;
	mBounds = Bounds(Position(mPosition.get_x() - halfTexWidth, mPosition.get_y() - halfTexHeight), mPosition.get_x() + halfTexWidth, mPosition.get_y() + halfTexHeight);
	mTopNeighbour = above;
	mBottomNeighbour = below;
	mLeftNeighbour = left;
	mRightNeighbour = right;
}

void Tile::Render() const {
	King::Engine::GetInstance().Render(mTextureID, mPosition.get_x(), mPosition.get_y(), NULL);
	if (mObject != nullptr)
		mObject->Render();
	if (mMarked)
		mMark.Render();
}

void Tile::InsertObj(Object obj) {
	mObject.reset(new Object());
	*mObject = obj;
}

void Tile::RemoveObject() {
	mObject.reset();
}

std::shared_ptr<Object> Tile::GetObj() const {
	return mObject;
}

void Tile::SwapObj(std::shared_ptr<Object> object) {
	mObject.swap(object);
}

Position Tile::GetPosition() const {
	return mPosition;
}

void Tile::SetPosition(const Position pos) {
	mPosition = pos;
}

bool Tile::MouseDetected(const Position & pos) const {
	if (pos.get_x() > mBounds.mPos.get_x() &&
		pos.get_x() < mBounds.mSizeX &&
		pos.get_y() > mBounds.mPos.get_y() &&
		pos.get_y() < mBounds.mSizeY)
		return true;
	else
		return false;
}

void Tile::SetMark(bool value){
	mMarked = value;
}

bool Tile::Marked() const {
	return mMarked;
}
