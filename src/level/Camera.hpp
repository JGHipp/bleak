#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.hpp"

class Camera: public Entity
{
	private:
		double focusX = 0, focusY = 0, lerp;
		int centerX = 0, centerY = 0;
	public:
		Camera(int centerX, int centerY);
		enum mode_t { CAM_STATIC, CAM_DELTA, CAM_FOCUSPOINT } mode;
		void setMode(mode_t mode);
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
