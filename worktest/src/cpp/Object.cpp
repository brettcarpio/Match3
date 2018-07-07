#include "Object.h"

Object::Object() : mSpeed(1.25f) {
}

Object::Object(King::Engine::Texture textureID) : 
	mSpeed(1.25f), 
	mTextureID(textureID) {

}

Object::Object(const Object & obj) {
	mTextureID = obj.GetTexID();
	mPosition = obj.GetPosition();;
	mSpeed = obj.GetSpeed();
}

Object::~Object() {

}

Object &Object::operator=(const Object & obj) {
	mTextureID = obj.GetTexID();
	mPosition = obj.GetPosition();;
	mSpeed = obj.GetSpeed();
	return *this;
}

void Object::Render() const {
	King::Engine::GetInstance().Render(mTextureID, mPosition.get_x(), mPosition.get_y(), NULL);
}

void Object::SetPosition(const Position pos) {
	mPosition = pos;
}

void Object::SetTexID(const King::Engine::Texture tex) {
	mTextureID = tex;
}

Position Object::GetPosition() const {
	return mPosition;
}

King::Engine::Texture Object::GetTexID() const {
	return mTextureID;
}

float Object::GetSpeed() const {
	return mSpeed;
}
