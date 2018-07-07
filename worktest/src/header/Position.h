#pragma once
#include <iostream>
#include <math.h>

class Position {
public:
	Position();
	Position(float x, float y);
	~Position();
	Position(const Position &p);
	Position &operator=(const Position &p);
	Position &operator+=(const Position &p);
	Position &operator-=(const Position &p);
	Position operator+(const Position &p);
	Position operator-(const Position &p);
	Position operator*(const float scalar);
	friend std::ostream &operator<<(std::ostream& os, const Position &p); 
	float Distance(Position p);
	Position Lerp(Position start, Position end, float percent);

	inline float get_x() const { return m_x; }
	inline float get_y() const { return m_y; }
	inline void set_x(const float x) { m_x = x; }
	inline void set_y(const float y) { m_y = y; }
	inline void set(const float x, const float y) { m_x = x; m_y = y; }

private:
	float Length(const Position p);
	float m_x;
	float m_y;
};