#include <iostream>
#include "simple_gmath.h"
#include "define.h"

namespace Simple_GMath
{
	float ToRadian(float angle)
	{
		return angle * (PI / 180.f);
	}

	// Point3
    Point3 Point3::operator +(const Point3& _point) const
    {
        return Point3(x + _point.x, y + _point.y, z + _point.z);
    }

	Point3 Point3::operator +(const Vector3& _vector) const
	{
		return Point3(x + _vector.x, y + _vector.y, z + _vector.z);
	}
     
    Point3 Point3::operator *(const float& val) const
    {
        return Point3(x * val, y * val, z * val);
    }
    
    Point3 Point3::operator /(const float& val) const
    {
		if (val == 0.f)
			return Point3(x, y, z);

        return Point3(x / val, y / val, z / val);
    }
    
    Point3& Point3::operator +=(const Point3& _point)
    {
        x += _point.x;
        y += _point.y;
        z += _point.z;
        
        return *this;
    }
    
    Point3& Point3::operator -=(const Point3& _point)
    {
        x -= _point.x;
        y -= _point.y;
        z -= _point.z;
        
        return *this;
    }
    
    Point3& Point3::operator *=(const float& val)
    {
        x *= val;
        y *= val;
        z *= val;
        
        return *this;
    }
    
    Point3& Point3::operator /=(const float& val)
    {
		if (val == 0.f)
			return *this;

        x /= val;
        y /= val;
        z /= val;
        
        return *this;
    }

	Vector3 Point3::operator -(const Point3& _point) const
    {
        return Vector3(x - _point.x, y - _point.y, z - _point.z);
    }

	Vector3 Point3::ToVector3() const { return Vector3(x, y, z); }
	bool Point3::Is_Equal(const Point3& _point) const
	{
		return (x == _point.x) && (y == _point.y) && (z == _point.z);
	}
    
	// Vector3
    Vector3 Vector3::operator +(const Vector3& _vector) const
    {
        return Vector3(x + _vector.x, y + _vector.y, z + _vector.z);
    }
    
    Vector3 Vector3::operator -(const Vector3& _vector) const
    {
        return Vector3(x - _vector.x, y - _vector.y, z - _vector.z);
    }
    
    Vector3 Vector3::operator *(const float& val) const
    {
        return Vector3(x * val, y * val, z * val);
    }
    
    Vector3 Vector3::operator /(const float& val) const
    {
		if (val == 0.f)
			return Vector3(x, y, z);

        return Vector3(x / val, y / val, z / val);
    }
    
    Vector3& Vector3::operator +=(const Vector3& val)
    {
        x += val.x;
        y += val.y;
        z += val.z;
        
        return *this;
    }
    
    Vector3& Vector3::operator -=(const Vector3& val)
    {
        x -= val.x;
        y -= val.y;
        z -= val.z;
        
        return *this;
    }
 
    float Vector3::Dot(const Vector3& _vector) const
    {
        return (x * _vector.x + y * _vector.y + z * _vector.z);
    }
    
    Vector3 Vector3::Cross(const Vector3& _vector) const
    {
        return Vector3 ((y * _vector.z - z * _vector.y),
                 -(x * _vector.z - z * _vector.x),
                 (x * _vector.y - y * _vector.x));
    }
    
    Vector3 Vector3::Normalize() const
    {
        float length = Length();
		if (length == 0.f)
			return Vector3();

        return Vector3(x/length, y/length, z/length);
    }
    
    float Vector3::Length() const
    {
        return sqrt(x*x + y*y + z*z);
    }

	// Color4
    Color4 Color4::operator +(const Color4& _color) const
    {
		return Color4(r + _color.r, g + _color.g, b + _color.b);
    }
 
    Color4 Color4::operator -(const Color4& _color) const
    {
        return Color4(r - _color.r, g - _color.g, b - _color.b);
    }
    
    Color4 Color4::operator *(const float& val) const
    {
        return Color4(r * val, g * val, b * val);
    }
    
	Color4 Color4::operator *(const Color4& _color) const
	{
		return Color4(r * _color.r, g * _color.g, b * _color.b);
	}

	Color4 Color4::operator /(const float& val) const
    {
        return Color4(r / val, g / val, b / val);
    }

	Color4 Color4::operator /(const Color4& _color) const
	{
		return Color4(r / _color.r, g / _color.g, b / _color.b);
	}

    
    Color4& Color4::operator +=(const Color4& _color)
    {
        r += _color.r;
        g += _color.g;
        b += _color.b;

        return *this;
    }
    
    Color4& Color4::operator -=(const Color4& _color)
    {
        r -= _color.r;
        g -= _color.g;
        b -= _color.b;
        
        return *this;
    }
    
    Color4& Color4::operator *=(const float& val)
    {
        r *= val;
        g *= val;
        b *= val;
        
        return *this;
    }
    
    Color4& Color4::operator /=(const float& val)
    {
		if (val == 0.f)
			return *this;

		r /= val;
        g /= val;
        b /= val;

        return *this;
    }

	void Color4::Min(float x)
	{
		r = (r < x ? x : r);
		g = (g < x ? x : g);
		b = (b < x ? x : b);
	}

	void Color4::Max(float x)
	{
		r = (r > x ? x : r);
		g = (g > x ? x : g);
		b = (b > x ? x : b);
	}
}