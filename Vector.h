#ifndef _VECTOR_H
#define _VECTOR_H

class Vector2D
{
public:
	void print();
	void reset();
	Vector2D();
	Vector2D(double xvar, double yvar);
	Vector2D(double xvar, double yvar, double _vx, double _vy);
	~Vector2D()= default;
	double x;
	double y;
	double vx;
	double vy;
};

#endif