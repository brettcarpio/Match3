#pragma once

#include "king/Engine.h"
#include "king/Updater.h"
#include "king/Sprite.h"

typedef std::pair<bool, std::string> SwitchScene;
class Scene {
public:
	~Scene() {}

	virtual void Update() = 0;
	virtual void Render() const = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;

	const bool &Running() { return mRunning; }
	const std::string &GetName() const { return mName; }
	const SwitchScene &SwitchingScene() { return mSwitchScene; }

protected:
	Scene() {}
	Scene(std::string name) : mName(name), mRunning(false) {}
	std::string mName;
	bool mRunning;
	SwitchScene mSwitchScene;
	King::Engine& mEngine = King::Engine::GetInstance();
};