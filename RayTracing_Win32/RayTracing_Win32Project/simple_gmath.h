#ifndef _SIMPLE_GMATH_H_
#define _SIMPLE_GMATH_H_

#include <math.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <iomanip>
#include <vector>
#include "define.h"

#define GETAXISMIN(DEST, POINTS, COUNT, AXIS) \
{ \
	assert(COUNT > 0); \
	DEST = POINTS[0].AXIS;	\
	for (int i=0; i<COUNT; ++i)	\
	{ \
		if (POINTS[i].AXIS < DEST)	\
			DEST = POINTS[i].AXIS;	\
	}	\
} \

#define GETAXISMAX(DEST, POINTS, COUNT, AXIS) \
{ \
	assert(COUNT > 0); \
	DEST = POINTS[0].AXIS;	\
	for (int i=0; i<COUNT; ++i)	\
	{ \
		if (POINTS[i].AXIS > DEST)	\
			DEST = POINTS[i].AXIS;	\
	}	\
} \

using namespace std;

namespace Simple_GMath
{
	float ToRadian(float angle);
	class Vector3;

class Point3
{
public:
    Point3(float _x = 0.f, float _y = 0.f, float _z = 0.f)
    : x(_x), y(_y), z(_z)
    {}
    
public:
    Point3 operator +(const Point3& _point) const;
	Point3 operator +(const Vector3& _vector) const;
    Point3 operator *(const float& val) const;
    Point3 operator /(const float& val) const;
    Point3& operator +=(const Point3& _point);
    Point3& operator -=(const Point3& _point);
    Point3& operator *=(const float& val);
    Point3& operator /=(const float& val);
 
    Vector3 operator -(const Point3& _point) const;

	Vector3 ToVector3() const;
	bool Is_Equal(const Point3& _point) const;

public:
    float x;
    float y;
    float z;
};

    
class Vector3
{
public:
    Vector3(float _x = 0.f, float _y = 0.f, float _z = 0.f)
    : x(_x), y(_y), z(_z)
    {}

public:
    Vector3 operator +(const Vector3& _vector) const;
    Vector3 operator -(const Vector3& _vector) const;
    Vector3 operator *(const float& val) const;
    Vector3 operator /(const float& val) const;
    Vector3& operator +=(const Vector3& _vector);
    Vector3& operator -=(const Vector3& _vector);
    float Dot(const Vector3& _vector) const;
    Vector3 Cross(const Vector3& _vector) const;
    Vector3 Normalize() const;
    float Length() const;
    
public:
    float x;
    float y;
    float z;
};

    
class Color4
{
public:
    Color4(float _r = 0.f, float _g = 0.f, float _b = 0.f, float _a = 1.f)
    : r(_r), g(_g), b(_b), a(_a)
    {}

public:
    Color4 operator +(const Color4& _color) const;
    Color4 operator -(const Color4& _color) const;
    Color4 operator *(const Color4& _color) const;
    Color4 operator *(const float& val) const;
    Color4 operator /(const Color4& _color) const;
    Color4 operator /(const float& val) const;
    Color4& operator +=(const Color4& _color);
    Color4& operator -=(const Color4& _color);
    Color4& operator *=(const float& val);
    Color4& operator /=(const float& val);

	void Min(float x = 0.f);
    void Max(float x = 1.f);
	Color4 Clamp() 
	{
		Min(); 
		Max();  

		return *this;
	}

public:
    float r;
    float g;
    float b;
    float a;
};
   
const Color4 NONE;
const Color4 BLACK(0.f, 0.f, 0.f);
const Color4 WHITE(1.f, 1.f, 1.f);
const Color4 RED(1.f, 0.f, 0.f);
const Color4 GREEN(0.f, 1.f, 0.f);
const Color4 BLUE(0.f, 0.f, 1.f);
const Color4 UNKNOWN(300.f, 300.f, 300.f);

}

#endif
