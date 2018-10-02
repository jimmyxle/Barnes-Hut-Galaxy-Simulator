#include "Vector.h"
#include <iostream>

Vector2D::Vector2D(double xvar, double yvar)
{
	x = xvar;
	y = yvar;

}
Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

void Vector2D::print()
{
	std::cout << x << "," << y << std::endl;
}