#pragma once

#include "Scene.h"
#include "Gameboard.h"
#include <chrono>

class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void Update();
	void Render() const;
	void Start();
	void Stop();

private:
	enum State {
		SELECT_MATCH,
		CHECK_MATCHES,
		REVERSE_MATCH,
		FILL_BOARD,
		DROP_OBJECTS,
		REMOVE_OBJECTS,
		DISPLAY_SCORE,
	};

	void UpdateGameBoard();
	bool UpdateTimer();
	void UpdateTexts() const;

	State mState;
	Gameboard mGameBoard;
	Position mBackgroundSpritePosition;
	Position mMousePosition;
	int mScore;
	int mTimer;
	int mNextSceneTimer;
	const int mCountdownToNextScene = 5;
	const int mCountdown = 60;
	std::chrono::time_point<std::chrono::steady_clock> mClock;
};