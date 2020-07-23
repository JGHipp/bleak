#include "Entity.hpp"

Entity::Entity(double x, double y, double dx, double dy, double width, double height): x(x), y(y), dx(dx), dy(dy), width(width), height(height)
{}

Entity::Entity(double x, double y, double width, double height): x(x), y(y), dx(0), dy(0), width(width), height(height)
{}

Entity::Entity(double x, double y): x(x), y(y), dx(0), dy(0), width(0), height(0)
{}

Entity::Entity(): x(0), y(0), dx(0), dy(0)
{}

void Entity::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Entity::updatePosition()
{
	x += dx;
	y += dy;
}

Rectangle Entity::getRectangle() { return Rectangle(x, y, width, height); }
double Entity::getX() { return x; }
double Entity::getY() { return y; }
double Entity::getDx() { return dx; }
double Entity::getDy() { return dy; }
double Entity::getWidth() { return width; }
double Entity::getHeight() { return height; }
void Entity::setX(double x) { this->x = x; }
void Entity::setY(double y) { this->y = y; }
void Entity::addX(double dx) { x += dx; }
void Entity::addY(double dy) { y += dy; }
void Entity::updateX() { x += dx; }
void Entity::updateY() { y += dy; }
void Entity::setDx(double dx) { this->dx = dx; }
void Entity::setDy(double dy) { this->dy = dy; }
void Entity::addDx(double ddx) { dx += ddx; }
void Entity::addDy(double ddy) { dy += ddy; }
void Entity::setWidth(double width) { this->width = width; }
void Entity::setHeight(double height) { this->height = height; }
