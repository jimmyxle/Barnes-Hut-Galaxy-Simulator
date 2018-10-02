#ifndef _VECTOR_H
#define _VECTOR_H

class Vector2D
{
public:
	void print();
	Vector2D();
	Vector2D(double xvar, double yvar);
	~Vector2D()= default;
	double x;
	double y;
};

#endif