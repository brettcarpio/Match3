#include "SceneManager.h"

SceneManager::SceneManager() {
	mScenes.push_back(new GameScene());
	mScenes.push_back(new MainMenuScene());
	mCurrentScene = mScenes.front();
}

SceneManager::~SceneManager() {
	for (Scene* scene : mScenes) {
		delete scene;
	}
}

SceneManager::SceneManager(const SceneManager & sm) {
	
}

void SceneManager::Update() {
	mCurrentScene->Update();
	if (mCurrentScene->SwitchingScene().first == true)
		SwitchScene(mCurrentScene->SwitchingScene().second);
}

void SceneManager::Render() const {
	mCurrentScene->Render();
}

void SceneManager::SwitchScene(std::string name) {
	std::cout << "Switching scene from " << mCurrentScene->GetName() << " to " << name << std::endl;
	for (int i = 0; i < mScenes.size(); ++i) {
		if (mScenes.at(i)->GetName() == name) {
			mCurrentScene->Stop();
			mCurrentScene = mScenes.at(i);
			mCurrentScene->Start();
			std::cout << "Current scene is " << mCurrentScene->GetName() << std::endl;
			return;
		}
	}
	std::cout << "Scene not found" << std::endl; 
}
