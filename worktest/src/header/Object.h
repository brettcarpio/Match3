#pragma once
#include "king/Engine.h"
#include "Position.h"

class Object {
public:
	Object();
	Object(King::Engine::Texture textureID);
	Object(const Object &obj);
	~Object();
	Object &operator=(const Object &obj);

	void Render() const;
	void SetPosition(const Position pos);
	void SetTexID(const King::Engine::Texture tex);
	Position GetPosition() const;
	King::Engine::Texture GetTexID() const;
	float GetSpeed() const;

private:
	Position mPosition;
	float mSpeed;
	King::Engine::Texture mTextureID;
};