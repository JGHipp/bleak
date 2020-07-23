#include "Rectangle.hpp"
#include "JMath.hpp"
#include <iostream>

Rectangle::Rectangle(double x, double y, double width, double height): x(x), y(y), width(width), height(height)
{}

bool Rectangle::intersects(Rectangle rectangle)
{
	bool intersects = (lessThan(absv(getCenterX() - rectangle.getCenterX()), (width / 2 + rectangle.width / 2)) &&
			lessThan(absv(getCenterY() - rectangle.getCenterY()) , (height / 2 + rectangle.height / 2)));
		return intersects;
}

// -1 means keep the dimension the same
void Rectangle::updateDimensions(double x, double y, double width, double height)
{
	if(x != -1) this->x = x;
	if(y != -1) this->y = y;
	if(width != -1) this->width = width;
	if(height != -1) this->height = height;			
}

double Rectangle::getCenterX() { return x + width / 2; } 
double Rectangle::getCenterY() { return y + height / 2; }
