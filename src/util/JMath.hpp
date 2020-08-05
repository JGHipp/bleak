#ifndef JMATH_H
#define JMATH_H

#include <cmath>
#include <limits>

// Trig functions in degrees
#define dtor(x) ((x) * (M_PI / 180.0))
#define dtan(x) (tan(dtor(x)))
#define dcos(x) (cos(dtor(x)))
#define dsin(x) (sin(dtor(x)))

template<class T>
T clamp(T number, T min, T max)
{
	if(number < min) number = min;
	if(number > max) number = max;
	return number;
}

template<class T>
T absv(T number) { return (number >= 0 ? number : -number); }

// Native double precision is trash
template<class T>
bool lessThan(T a, T b)
{
	const T epsilon = std::numeric_limits<T>::epsilon();
    return (b - a) > ( (absv(a) < absv(b) ? absv(b) : absv(a)) * epsilon);
}

#endif
