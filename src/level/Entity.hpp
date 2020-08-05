#ifndef ENTITY_H
#define ENTITY_H

#include "../util/Rectangle.hpp"

class Entity
{
	private:
		double x = 0, y = 0, dx = 0, dy = 0, width = 0, height = 0;
	public: 
		Entity(double x, double y, double dx, double dy, double width, double height);
		Entity(double x, double y, double width, double height);
		Entity(double x, double y);
		Entity();
		void updatePosition();
		void updateX();
		void updateY();
		void setPosition(double x, double y);
		void setX(double x);
		void setY(double y);
		void addX(double dx);
		void addY(double dy);
		void setDx(double dx);
		void setDy(double dy);
		void addDx(double ddx);
		void addDy(double ddy);
		void setWidth(double width);
		void setHeight(double height);
		double getX();
		double getY();
		double getDx();
		double getDy();
		double getWidth();
		double getHeight();
		Rectangle getRectangle();
};

#endif
