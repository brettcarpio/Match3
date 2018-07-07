#include "MainMenuScene.h"

MainMenuScene::MainMenuScene() : Scene("main"),
	mBackgroundSpritePosition(mEngine.GetWidth() / 2, mEngine.GetHeight() / 2),
	mLogoPosition(Position(mEngine.GetWidth() / 2, 200.f)),
	mState(State::IDLE),
	mEaseTimer(0) {
	float halfTexWidth = King::Engine::GetInstance().GetTextureWidth(King::Engine::TEXTURE_PLAY_BUTTON) / 2;
	float halfTexHeight = King::Engine::GetInstance().GetTextureWidth(King::Engine::TEXTURE_PLAY_BUTTON) / 2;
 	mPlayBtn = Button(Position(mEngine.GetWidth()/2, 500.f), halfTexWidth, halfTexHeight);
}

MainMenuScene::~MainMenuScene() {

}

void MainMenuScene::Update() {
	mMousePosition = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
	switch (mState) {
	case State::IDLE:
		if (mEngine.GetMouseButtonDown() && mPlayBtn.MouseDetected(mMousePosition)) {
			mState = State::TRANSITION;
		}
		break;
	case State::TRANSITION:
		if (mEaseTimer < 1.f) {
			mEaseTimer += mEngine.GetLastFrameSeconds();
			if (mEaseTimer > DURATION)
				mEaseTimer = 1;
			mPlayBtn.mPos.set_y(BackEaseIn(mEaseTimer, 500.f, 1000.f, DURATION));
			mLogoPosition.set_y(BackEaseIn(mEaseTimer, 200.f, 700.f, DURATION));
		}
		if (mLogoPosition.get_y() > 1200.f) {
			mSwitchScene = SwitchScene(true, "game");
		}
		break;
	}
}

void MainMenuScene::Render() const {
	mEngine.Render(King::Engine::TEXTURE_BACKGROUND, mBackgroundSpritePosition.get_x(), mBackgroundSpritePosition.get_y(), NULL);
	mEngine.Render(King::Engine::TEXTURE_BACKGROUND_FLAIR, mBackgroundSpritePosition.get_x(), mBackgroundSpritePosition.get_y(), NULL);

	mEngine.Render(King::Engine::TEXTURE_PLAY_BUTTON, mPlayBtn.mPos.get_x(), mPlayBtn.mPos.get_y(), NULL);
	mEngine.Render(King::Engine::TEXTURE_LOGO, mLogoPosition.get_x(), mLogoPosition.get_y(), NULL);
}

void MainMenuScene::Start() {
	mState = State::IDLE;
	mRunning = true;
}

void MainMenuScene::Stop() {
	mRunning = false;
	float halfTexWidth = King::Engine::GetInstance().GetTextureWidth(King::Engine::TEXTURE_PLAY_BUTTON) / 2;
	float halfTexHeight = King::Engine::GetInstance().GetTextureWidth(King::Engine::TEXTURE_PLAY_BUTTON) / 2;
	mPlayBtn = Button(Position(mEngine.GetWidth() / 2, 500.f), halfTexWidth, halfTexHeight);
	mLogoPosition = Position(mEngine.GetWidth() / 2, 200.f);
	mEaseTimer = 0;
	mSwitchScene = SwitchScene(false, "");
}
