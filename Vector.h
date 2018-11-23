#ifndef _VECTOR_H
#define _VECTOR_H

class Vector2D
{
public:
	void print();
	void reset();
	Vector2D();
	Vector2D(float xvar, float yvar);
	Vector2D(float xvar, float yvar, float _vx, float _vy);
	~Vector2D()= default;
	float x;
	float y;
	float vx;
	float vy;
};

#endif