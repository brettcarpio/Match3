#include "Game.h"

Game::Game()
	: mSceneMgr(new SceneManager()){
}

Game::~Game(){
}

void Game::Start() {
	mSceneMgr->mCurrentScene->Start();
	King::Engine::GetInstance().Start(*this);
}

void Game::Update() {
	mSceneMgr->Update();
	mSceneMgr->Render();
}

