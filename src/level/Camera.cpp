#include "Camera.hpp"
#include <iostream>
#include <cmath>	// floor()

Camera::Camera(int centerX, int centerY): Entity(), lerp(1.0), centerX(centerX), centerY(centerY)
{}

void Camera::update()
{
	if(mode == CAM_FOCUSPOINT) // Focus point mode
	{
		setX(getX() + ((focusX - centerX) - getX()) * lerp);
		setY(getY() + ((focusY - centerY) - getY()) * lerp);
	}
	else if(mode == CAM_STATIC) // Static mode
	{
		setX(focusX - centerX);
		setY(focusY - centerY);
	}
	else if(mode == CAM_DELTA) // Delta mode
	{
		setFocusPoint(focusX + getDx(), focusY + getDy());
		setX(focusX - centerX);
		setY(focusY - centerY);	
	}
}

/*
	We always want the integer value to be 
	rounded to the smallest number, including
	negatives.
*/
const double floorEpsilon = 0.005; // Double precision is a nightmare!!
int Camera::transformX(double x)
{
	return (int) floor(x - getX() + floorEpsilon);
}

int Camera::transformY(double y)
{
	return (int) floor(y - getY() + floorEpsilon);
}

// Focus point mode
void Camera::setFocusPoint(double focusX, double focusY)
{
	this->focusX = focusX;
	this->focusY = focusY;
}

void Camera::setCenter(int centerX, int centerY)
{
	this->centerX = centerX;
	this->centerY = centerY;
}

double Camera::getFocusX() { return focusX; }
double Camera::getFocusY() { return focusY; }
double Camera::getLerp() { return lerp; }
void Camera::setMode(mode_t mode) { this->mode = mode; }
void Camera::setLerp(double lerp) { this->lerp = lerp; }
