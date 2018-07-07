#include "Gameboard.h"

Gameboard::Gameboard() : 
	mPos(Position(0.f, 0.f)), 
	mOffset(0.f){
	InitialiseBoard();
	Populate();
	mMarkedTiles.Clear();
	Shuffle();
	
}

Gameboard::Gameboard(Position pos, float offset) :
	mPos(pos),
	mOffset(offset){
	InitialiseBoard();
	Populate();
	mMarkedTiles.Clear();
	Shuffle();
}

Gameboard::Gameboard(const Gameboard & gb) {
}

Gameboard::~Gameboard() {
}

void Gameboard::Render() const {
	for (int i = 0; i < Rows; ++i)	{
		for (int j = 0; j < Columns; ++j) {
			mGrid[i][j].Render();
		}
	}
}

void Gameboard::Shuffle() {
	auto ChangeObj = [&](std::vector<GridID> &vec) {
		int element = rand() % vec.size();
		King::Engine::Texture randTex;
		do {
			randTex = (King::Engine::Texture)(rand() % King::Engine::Texture::TEXTURE_OBJ_MAX);
			mGrid[vec.at(element).first][vec.at(element).second].GetObj()->SetTexID(randTex);
		} while (randTex != mGrid[vec.at(element).first][vec.at(element).second].GetObj()->GetTexID());
	};

	std::vector<GridID> matches;
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Columns; ++j) {
			FindMatch(GridID(i, j), matches); 
			if (matches.size() >= 3) {
				ChangeObj(matches);
			}
			matches.clear();
		}
	}
}

void Gameboard::RestartBoard() {
	mNeighbourTiles.clear();
	mLowestEmpty.clear();
	mMarkedTiles.Clear();
	Populate();
	Shuffle();
}

void Gameboard::MarkTile(Position &mousePos) { 
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Columns; ++j) {
			if (mGrid[i][j].MouseDetected(mousePos)) {
				mGrid[i][j].SetMark(true);
				mMarkedTiles.SetFront(std::make_pair(i, j));
				mMarkedTiles.hasFirst = true;
				if (i - 1 >= 0) {
					mNeighbourTiles.push_back(std::make_pair(i - 1, j));
				}
				if (i + 1 < Rows) {
					mNeighbourTiles.push_back(std::make_pair(i + 1, j));
				}
				if (j - 1 >= 0) {
					mNeighbourTiles.push_back(std::make_pair(i, j - 1));
				}
				if (j + 1 < Columns) {
					mNeighbourTiles.push_back(std::make_pair(i, j + 1));
				}
			}
		}
	}
}

bool Gameboard::MarkSwapTarget(Position &mousePos) {
	if (mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].MouseDetected(mousePos)){
		return false;
	}
	for (GridID t : mNeighbourTiles) {
		if (mGrid[t.first][t.second].MouseDetected(mousePos)) {
			mMarkedTiles.SetBack(std::make_pair(t.first, t.second));
			mMarkedTiles.hasSecond = true;
			mNeighbourTiles.clear();
			mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].SetMark(false);
			std::shared_ptr<Object> firstObj = mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].GetObj();
			std::shared_ptr<Object> secondObj = mGrid[mMarkedTiles.Back().first][mMarkedTiles.Back().second].GetObj();
			std::swap(*firstObj, *secondObj);
			return true;
		}
	}
	ResetMarks();
	return false;
}

void Gameboard::ResetMarks(){
	mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].SetMark(false);
	mMarkedTiles.Clear();
	mNeighbourTiles.clear();
}

bool Gameboard::CheckMatch(int &score) {
	//check horizontal
	std::vector<GridID> matches;
	std::vector<GridID> matches2;
	FindMatch(mMarkedTiles.Front(), matches);
	FindMatch(mMarkedTiles.Back(), matches2);
	if (matches.size() >= 3) {
		for (int i = 0; i < matches.size(); ++i) {
			mGrid[matches.at(i).first][matches.at(i).second].RemoveObject();
		}
		score += 50 * matches.size();
		FindLowestEmpty(matches);
	}
	if (matches2.size() >= 3) {
		for (int i = 0; i < matches2.size(); ++i) {
			mGrid[matches2.at(i).first][matches2.at(i).second].RemoveObject();
		}
		score += 50 * matches2.size();
		FindLowestEmpty(matches2);
	}
	if (matches.size() >= 3 || matches2.size() >= 3) {
		ResetMarks();
		return true;
	}
	else {
		std::shared_ptr<Object> firstObj = mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].GetObj();
		std::shared_ptr<Object> secondObj = mGrid[mMarkedTiles.Back().first][mMarkedTiles.Back().second].GetObj();
		std::swap(*firstObj, *secondObj);
		return false;
	}
}

bool Gameboard::RefillBoard() {
	for (int i = 0; i < mLowestEmpty.size(); ++i) {
		GridID target;
		GridID current = mLowestEmpty.at(i);
		for (int j = 1; current.second - j >= 0; ++j) {
			if (mGrid[current.first][current.second - j].GetObj() != nullptr) {
				target = GridID(current.first, current.second - j);
				break;
			}
		}
		if (target != GridID(NULL, NULL)) {
			for (; target.second >= 0; --target.second, --current.second) {
				Object obj = Object(mGrid[target.first][target.second].GetObj()->GetTexID());
				obj.SetPosition(mGrid[target.first][target.second].GetPosition());
				mGrid[current.first][current.second].InsertObj(obj);
			}
		}
		if (target == GridID(NULL, NULL) || target.second <= 0) {
			for (int i = current.second, y = 1; i >= 0; --i, ++y) {
				Object obj = Object((King::Engine::Texture)(rand() % King::Engine::Texture::TEXTURE_OBJ_MAX));
				obj.SetPosition(Position(mGrid[current.first][0].GetPosition().get_x(), mGrid[current.first][0].GetPosition().get_y() -
					( 82*y)) );
				mGrid[current.first][i].InsertObj(obj);
			}
		}
	}
	return true;
}

bool Gameboard::DropObjects() {
	for (int i = 0; i < mLowestEmpty.size(); ++i) {
		GridID current = mLowestEmpty.at(i);
		for (int j = current.second; j >= 0; --j) {
			std::shared_ptr<Object> firstObj = mGrid[current.first][j].GetObj();
			const Position target = mGrid[current.first][j].GetPosition();
			firstObj->SetPosition(Lerp(firstObj->GetPosition(), target, 0.1f));
			if (firstObj->GetPosition().Distance(target) <= 0.5f) {
				for (int i = 0; i < mLowestEmpty.size(); ++i) {
					GridID current = mLowestEmpty.at(i);
					for (int j = current.second; j >= 0; --j) {
						mGrid[current.first][j].GetObj()->SetPosition(mGrid[current.first][j].GetPosition());
					}
				}
				mLowestEmpty.clear();
				return true;
			}
		}
	}
	return false;
}

bool Gameboard::RemoveObjects(int &score) {
	std::vector<GridID> matches;
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Columns; ++j) {
			FindMatch(GridID(i, j), matches);
			if (matches.size() >= 3) {
				for (int i = 0; i < matches.size(); ++i) {
					mGrid[matches.at(i).first][matches.at(i).second].RemoveObject();
				}
				score += 50 * matches.size();
				FindLowestEmpty(matches);
				return true;
			}
			matches.clear();
		}
	}
	return false;
}

void Gameboard::FindMatch(GridID tile, std::vector<GridID> &matches) {
	//check horizontal
	//can only check neighbours twice
	//ox.x.xx
	//xx.x.xo
	Tile* currentTile;
	for (int i = 1; i <= 3; ++i) {
		if (tile.first - i < 0) {
			break;
		}
		if (mGrid[tile.first - i][tile.second].GetObj()->GetTexID() == mGrid[tile.first][tile.second].GetObj()->GetTexID()) {
			matches.push_back(GridID(tile.first - i, tile.second));
			continue;
		}
		break;
	}
	for (int i = 1; i <= 3; ++i) {
		if (tile.first + i >= Rows) {
			break;
		}
		if (mGrid[tile.first + i][tile.second].GetObj()->GetTexID() == mGrid[tile.first][tile.second].GetObj()->GetTexID()) {
			matches.push_back(GridID(tile.first + i, tile.second));
			continue;
		}
		break;
	}
	//check vertical
	if (matches.size() < 2) {
		matches.clear();
		for (int i = 1; i <= 3; ++i) {
			if (tile.second - i < 0) {
				break;
			}
			if (mGrid[tile.first][tile.second - i].GetObj()->GetTexID() == mGrid[tile.first][tile.second].GetObj()->GetTexID()) {
				matches.push_back(GridID(tile.first, tile.second - i));
				continue;
			}
			break;
		}
		for (int i = 1; i <= 3; ++i) {
			if (tile.second + i >= Columns) {
				break;
			}
			if (mGrid[tile.first][tile.second + i].GetObj()->GetTexID() == mGrid[tile.first][tile.second].GetObj()->GetTexID()) {
				matches.push_back(GridID(tile.first, tile.second + i));
				continue;
			}
			break;
		}
	}
	matches.push_back(tile);
}

void Gameboard::FindLowestEmpty(std::vector<GridID> matches){
	for (int i = 0; i < matches.size(); i++) {
		if (mGrid[matches.at(i).first][matches.at(i).second + 1].GetObj() != nullptr) {
			mLowestEmpty.push_back(matches.at(i));
		}
	}
}

void Gameboard::InitialiseBoard() {
	float xPos = mPos.get_x();
	for (int i = 0; i < Rows; ++i) {
		float yPos = mPos.get_y();
		for (int j = 0; j < Columns; ++j) {
			mGrid[i][j].Init(Position(xPos, yPos), 
				(j - 1 > 0) ? &mGrid[i][j - 1] : nullptr,
				(j + 1 < Columns) ? &mGrid[i][j + 1] : nullptr,
				(i - 1 > 0) ? &mGrid[i - 1][j] : nullptr,
				(i + 1 < Rows) ? &mGrid[i + 1][j] : nullptr);
			yPos += ((float)King::Engine::GetInstance().GetTextureHeight(King::Engine::TEXTURE_TILE) + mOffset);
		}
		xPos += ((float)King::Engine::GetInstance().GetTextureWidth(King::Engine::TEXTURE_TILE) + mOffset);
	}
}

void Gameboard::Populate() {
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Columns; ++j) {
			Object obj = Object((King::Engine::Texture)(rand() % King::Engine::Texture::TEXTURE_OBJ_MAX));
			obj.SetPosition(mGrid[i][j].GetPosition());
			mGrid[i][j].InsertObj(obj);
		}
	}
}

bool Gameboard::SwapObjects() {
	std::shared_ptr<Object> firstObj = mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].GetObj();
	std::shared_ptr<Object> secondObj = mGrid[mMarkedTiles.Back().first][mMarkedTiles.Back().second].GetObj();
	const Position firstTarget = mGrid[mMarkedTiles.Front().first][mMarkedTiles.Front().second].GetPosition();
	const Position secondTarget = mGrid[mMarkedTiles.Back().first][mMarkedTiles.Back().second].GetPosition();
	firstObj->SetPosition(Lerp(firstObj->GetPosition(), firstTarget, 0.25f));
	secondObj->SetPosition(Lerp(secondObj->GetPosition(), secondTarget, 0.25f));
	if (firstObj->GetPosition().Distance(firstTarget) <= 2) {
		firstObj->SetPosition(firstTarget);
		secondObj->SetPosition(secondTarget);
		return true;
	}
	return false;
}

const MarkedTiles& Gameboard::GetMarkedTiles() const {
	return mMarkedTiles;
}


