#include "GameScene.h"

GameScene::GameScene() : Scene("game"), 
	mGameBoard(Gameboard(Position(100.f, 100.f), 5.f)),
	mBackgroundSpritePosition(mEngine.GetWidth() / 2, mEngine.GetHeight() / 2),
	mState(State::SELECT_MATCH),
	mScore(0),
	mTimer(mCountdown),
	mNextSceneTimer(mCountdownToNextScene){
}

GameScene::~GameScene() {
}

void GameScene::Update() {
	if (mState == State::DISPLAY_SCORE) {
		if (mNextSceneTimer <= 0)
			mSwitchScene = SwitchScene(true, "main");
		if (UpdateTimer())
			--mNextSceneTimer;
	}
	else {
		if (mTimer <= 0) 
			mState = State::DISPLAY_SCORE;
		if (UpdateTimer())
			--mTimer;
		UpdateGameBoard();
	}
}

void GameScene::Render() const {
	mEngine.Render(King::Engine::TEXTURE_BACKGROUND, mBackgroundSpritePosition.get_x(), mBackgroundSpritePosition.get_y(), NULL);
	mEngine.Render(King::Engine::TEXTURE_BACKGROUND_FLAIR, mBackgroundSpritePosition.get_x(), mBackgroundSpritePosition.get_y(), NULL);
	mGameBoard.Render();
	UpdateTexts();
}

void GameScene::UpdateGameBoard() {
	mMousePosition = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
	switch (mState) {
	case State::SELECT_MATCH:
		if (mEngine.GetMouseButtonDown() && !mGameBoard.GetMarkedTiles().hasFirst)
			mGameBoard.MarkTile(mMousePosition);
		else if (mEngine.GetMouseButtonDown() && !mGameBoard.GetMarkedTiles().hasSecond)
			mGameBoard.MarkSwapTarget(mMousePosition);
		else if (mGameBoard.GetMarkedTiles().hasFirst && mGameBoard.GetMarkedTiles().hasSecond && mGameBoard.SwapObjects())
			mState = State::CHECK_MATCHES;
		break;
	case State::CHECK_MATCHES:
		if (mGameBoard.CheckMatch(mScore))
			mState = State::FILL_BOARD;
		else
			mState = State::REVERSE_MATCH;
		break;
	case State::REVERSE_MATCH:
		if (mGameBoard.SwapObjects()) {
			mGameBoard.ResetMarks();
			mState = State::SELECT_MATCH;
		}
		break;
	case State::FILL_BOARD:
		if (mGameBoard.RefillBoard())
			mState = State::DROP_OBJECTS;
		break;
	case State::DROP_OBJECTS:
		if (mGameBoard.DropObjects())
			mState = State::REMOVE_OBJECTS;
		break;
	case State::REMOVE_OBJECTS:
		if (mGameBoard.RemoveObjects(mScore))
			mState = State::FILL_BOARD;
		else
			mState = State::SELECT_MATCH;
		break;
	}
}

bool GameScene::UpdateTimer() {
	std::chrono::duration<float> duration;
	auto now = std::chrono::high_resolution_clock::now();
	duration = now - mClock;
	if (duration.count() > 1) {
		mClock = now;
		return true;
	}
	return false;
}

void GameScene::UpdateTexts() const {
	if (mState == State::DISPLAY_SCORE) {
		char string[100];
		strcpy(string, "Your final Score is: ");
		mEngine.Render(King::Engine::TEXT_ITEM_00, strcat(string, std::to_string(mScore).c_str()), mBackgroundSpritePosition.get_x(), mBackgroundSpritePosition.get_y());
	}
	char string[100];
	strcpy(string, "Score: ");
	mEngine.Render(King::Engine::TEXTURE_BASE, 850.f, 180.0f, NULL);
	mEngine.Render(King::Engine::TEXT_ITEM_00, strcat(string, std::to_string(mScore).c_str()), 850.f, 180.0f);

	char string2[10];
	strcpy(string2, "Timer: ");
	mEngine.Render(King::Engine::TEXTURE_BASE, 850.f, 540.0F, NULL);
	mEngine.Render(King::Engine::TEXT_ITEM_01, strcat(string2, std::to_string(mTimer).c_str()), 850.f, 540.0f);
}

void GameScene::Start() {
	mGameBoard.RestartBoard();
	mRunning = true;
	mScore = 0;
	mTimer = mCountdown;
	mNextSceneTimer = mCountdownToNextScene;
	mClock = std::chrono::high_resolution_clock::now();
	mState = State::SELECT_MATCH;
}

void GameScene::Stop() {
	mRunning = false;
	mSwitchScene = SwitchScene(false, "");
	
}

