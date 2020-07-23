#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
	public:
		double x, y, width, height;
		Rectangle(double x, double y, double width, double height);
		void updateDimensions(double x, double y, double width, double height);
		double getCenterX();
		double getCenterY();
		bool intersects(Rectangle rectangle);
};

#endif
