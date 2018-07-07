#pragma once

#include "king/Engine.h"
#include "Scene.h"
#include "MainMenuScene.h"
#include "GameScene.h"

#include <vector>
#include <iostream>

class SceneManager {
public:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager & sm);

	void Update();
	void Render() const;
	void SwitchScene(std::string name);

	Scene * mCurrentScene;
private:
	std::vector<Scene*> mScenes;
};