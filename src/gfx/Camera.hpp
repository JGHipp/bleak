#ifndef CAMERA_H
#define CAMERA_H

namespace Camera
{
		extern double x, y, dx, dy, focusX, focusY, lerp;
		extern int centerX, centerY;
		enum mode_t { CAM_STATIC, CAM_DELTA, CAM_FOCUSPOINT };
		extern mode_t mode;
		extern void update();
		extern int transformX(double x);
		extern int transformY(double y);
		extern void setFocusPoint(double focusX, double focusY);
		extern double getCenterX();
		extern double getCenterY();
		extern void setCenter(int centerX, int centerY);
};

#endif
