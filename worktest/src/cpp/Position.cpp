#include "Position.h"

Position::Position()
	: m_x(0.f),
	m_y(0.f){
}

Position::Position(float x, float y)
	: m_x(x),
	m_y(y){
}

Position::~Position(){
}

Position::Position(const Position &p) {
	set(p.get_x(), p.get_y());
}

Position &Position::operator=(const Position &p) {
	set(p.get_x(), p.get_y());
	return *this;
}

Position &Position::operator+=(const Position &p) {
	set(m_x + p.get_x(), m_y + p.get_y());
	return *this;
}

Position &Position::operator-=(const Position &p) {
	set(m_x - p.get_x(), m_y - p.get_y());
	return *this;
}

Position Position::operator+(const Position &p) {
	Position pos(m_x + p.get_x(), m_y + p.get_y());
	return pos;
}

Position Position::operator-(const Position &p) {
	Position pos(m_x - p.get_x(), m_y - p.get_y());
	return pos;
}

Position Position::operator*(const float scalar) {
	set(m_x * scalar, m_y * scalar);
	return *this;
}

std::ostream & operator<<(std::ostream & os, const Position & p) {
	os << "{ x: " << p.get_x() << ", y: " << p.get_y() << "}";
	return os;
}

float Position::Distance(Position p) {
	return Length(p - Position(m_x, m_y));
}

Position Position::Lerp(Position start, Position end, float percent) {
	return (start + ((end - start)*percent));
}

float Position::Length(const Position p) {
	return sqrt(p.get_x()*p.get_x() + p.get_y()*p.get_y());
}



