#include "Vector.h"
#include <iostream>

Vector2D::Vector2D(float xvar, float yvar)
{
	x = xvar;
	y = yvar;
	vx = 0;
	vy = 0;
}
Vector2D::Vector2D()
{
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
}

void Vector2D::print()
{
	std::cout <<"Postion = ("<< x << "," << y <<") & Velocity= ("<<vx<<","<<vy<<")";
}

Vector2D::Vector2D(float xvar, float yvar, float _vx, float _vy)
{
	x = xvar;
	y = yvar;
	vx = _vx;
	vy = _vy;
}

void Vector2D::reset()
{
	x = y = vx = vy = 0;
}