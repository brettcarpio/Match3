#pragma once

#include "Scene.h"
#include "Position.h"

struct Button {
	Button() {}
	Button(Position position, float sizeX, float sizeY) :
		mPos(position){
		mTopLeft = Position(mPos.get_x() - sizeX, mPos.get_y() - sizeY);
		mSizeX = mPos.get_x() + (sizeX);
		mSizeY = mPos.get_y() + (sizeY);
	}
	bool MouseDetected(const Position & pos) const {
		if (pos.get_x() > mTopLeft.get_x() &&
			pos.get_x() < mSizeX &&
			pos.get_y() > mTopLeft.get_y() &&
			pos.get_y() < mSizeY)
			return true;
		else
			return false;
	}
	Position mPos;
	Position mTopLeft;
	float mSizeX;
	float mSizeY;
};

class MainMenuScene : public Scene {
public:
	MainMenuScene();
	~MainMenuScene();

	void Update();
	void Render() const;
	void Start();
	void Stop();

private:
	enum State {
		IDLE,
		TRANSITION
	};

	State mState;
	Position mMousePosition;
	Button mPlayBtn;
	Position mLogoPosition;
	Position mBackgroundSpritePosition;
	float mEaseTimer;
	const float DURATION = 2.f;
	float BackEaseIn(float t, float b, float c, float d) {
		return c * (t /= d)*t*t*t + b;
	}
};