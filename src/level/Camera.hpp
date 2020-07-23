#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.hpp"

class Camera: public Entity
{
	private:
		double focusX, focusY, lerp;
		int centerX, centerY;
		int mode; // 0 for static mode, 1 for delta mode, 2 for focus point mode
	public:
		Camera(int centerX, int centerY);
		void setMode(int mode);
		void update();
		int transformX(double x);
		int transformY(double y);
		// Focus point mode
		void setFocusPoint(double focusX, double focusY);
		void setCenter(int centerX, int centerY);
		double getFocusX();
		double getFocusY();
		double getLerp();
		void setLerp(double lerp);
};

#endif
