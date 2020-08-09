#include "Camera.hpp"
#include "../util/JMath.hpp"
#include <iostream>
#include <cmath>

namespace Camera
{
	const double MIN_DELTA = 0.5, FLOOR_EPSILON = 0.00005;
	double x = 0, y = 0, dx = 0, dy = 0, focusX = 0, focusY = 0, lerp = 1;
	int centerX = 0, centerY = 0;
	mode_t mode;

	void update()
	{
		if(mode == CAM_FOCUSPOINT) // Focus point mode
		{
			double moveX = (focusX - centerX) - x, moveY = (focusY - centerY) - y;
			x += moveX * (absv(moveX) < MIN_DELTA ? 1.0 : lerp);
			y +=  moveY * (absv(moveY) < MIN_DELTA ? 1.0 : lerp);
		}
		else if(mode == CAM_STATIC) // Static mode
		{
			x = focusX - centerX;
			y = focusY - centerY;
		}
		else if(mode == CAM_DELTA) // Delta mode
		{
			setFocusPoint(focusX + dx, focusY + dy);
			x = focusX - centerX;
			y = focusY - centerY;	
		}
	}

	// Focus point mode
	void setFocusPoint(double focusX, double focusY)
	{
		Camera::focusX = focusX;
		Camera::focusY = focusY;
	}

	void setCenter(int centerX, int centerY)
	{
		Camera::centerX = centerX;
		Camera::centerY = centerY;
	}
	
	int transformX(double x) { return (int) floor(x - Camera::x + FLOOR_EPSILON); }
	int transformY(double y) { return (int) floor(y - Camera::y + FLOOR_EPSILON); }
	double getCenterX() { return x + centerX; }
	double getCenterY() { return y + centerY; }
}
