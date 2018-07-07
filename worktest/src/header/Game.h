#pragma once

#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "SceneManager.h"

class Game : public King::Updater{
public:
	Game();
	~Game();

	void Start();
	void Update() override;

private:
	std::unique_ptr<SceneManager> mSceneMgr;
};