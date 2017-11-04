#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "class_header.h"
#include "simple_gmath.h"
#include "obj_parser.h"
#include <list>
#include <vector>
#include <assert.h>
#include <string.h> // for memcpy

using namespace Simple_GMath;
using namespace std;

const Color4 DEFAULT_AMBIENT(0.4f, 0.4f, 0.4f);
const int CUBEPOINT_MAX = 8;

class Object;
class ObjectManager
{
public:
	typedef list<Object*> LIST;
public:
	~ObjectManager() { Free(); }

public:
	void Insert(Object* pObject) { list.push_back(pObject); }
	void Remove(Object* pObject) { list.remove(pObject); }
	void Free(); // this is cannot definition at here. but possible at cpp.

	LIST& GetList() { return list; }

private:
	LIST list;

public:
	static ObjectManager* GetInstance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new ObjectManager();

		return m_pInstance;
	}

	static void FreeInstance()
	{
		if (m_pInstance == nullptr)
			return;

		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static ObjectManager* m_pInstance;
};

///////////////////
// Objects defenition is started from here.
// intersection is closest.
///////////////////

class Object
{
public:
	enum Type
	{
		OBJMODEL,
		POLYGON,
		SPHERE,
		CUBE,
		PLANE
	};

public :
	Object() 
		: diffuse(RED), specular(WHITE), shininess(10.f)
		, reflected_coef(0.5f) , transmitted_coef(0.5f), refracted_idx(0.5f)
	{}
    Object(const Color4& _diffuse, const Color4& _specular, float _shininess, 
		float _reflected_coef, float _transmitted_coef, float _refracted_idx) 
		: diffuse(_diffuse), specular(_specular), shininess(_shininess)
		, reflected_coef(_reflected_coef), transmitted_coef(_transmitted_coef)
		, refracted_idx(_refracted_idx)
		, fAngle(0.f)
	{ }
    virtual ~Object() {}

public:
	virtual bool LoadObjectFromFile(const char* pFileName) = 0;
	virtual bool Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal) = 0;
	virtual bool GetNormal(Vector3* pOut) const = 0;
	virtual Type GetType() = 0;

public:
    void Set(const Color4& _diffuse, const Color4& _specular, float _shininess, 
		float _reflected_coef, float _transmitted_coef, float _refracted_idx) 
	{
		diffuse = _diffuse;
		specular = _specular;
		shininess = _shininess;
		reflected_coef = _reflected_coef;
		transmitted_coef = _transmitted_coef;
		refracted_idx = _refracted_idx;
	}

public:
    Color4 diffuse;
    Color4 specular;
    float shininess;
    float reflected_coef;
    float transmitted_coef;
    float refracted_idx;
	float fAngle;
};

class Triangle : public Object
{
public:
	Triangle() {}
	Triangle(const Point3& p1, const Point3& p2, const Point3& p3, const Color4& _diffuse, const Color4& _specular, float _shininess = 10.f, 
		float _reflected_coef = 0.5f, float _transmitted_coef = 0.5f, float _refracted_idx = 0.5f)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
    {
        Set(p1, p2, p3);
    }
	Triangle(const Point3& p1, const Point3& p2, const Point3& p3, 
		const Vector3& n1, const Vector3& n2, const Vector3& n3,
		const Color4 _diffuse = GREEN, const Color4 _specular = WHITE, float _shininess = 10.f, 
		float _reflected_coef = 0.5f, float _transmitted_coef = 0.5f, float _refracted_idx = 0.5f)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
    {
        Set(p1, p2, p3);
    }

public:
	virtual bool LoadObjectFromFile(const char* pFileName) { return false; }
	virtual bool Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal);
	virtual Type GetType() { return Type::POLYGON; }
	virtual bool GetNormal(Vector3* pOut) const 
	{
		assert(pOut != nullptr);
		(*pOut) = normal;
		return true;
	}

public:
    void Set(const Point3& p1, const Point3& p2, const Point3& p3);
    
public:
	float D;
	Vector3 normal;
    Point3 point[3];
};


class Sphere : public Object
{
public:
	Sphere(const char* pFileName);
    Sphere(const Color4& _diffuse, const Color4& _specular, float _shininess, 
		float _reflected_coef, float _transmitted_coef, float _refracted_idx,
		const Point3& _center, float _radius)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
    {
        Set(_center, _radius);
    }

public:
	virtual bool LoadObjectFromFile(const char* pFileName);
	virtual bool Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal);
	virtual Type GetType() { return Type::SPHERE; }
	virtual bool GetNormal(Vector3* pOut) const 
	{
		assert(pOut != nullptr);
		return false; // need to converting the 'Object' class to 'Sphere' class. then use the 'CalcNormal()' function.
	}

public:
    void Set(const Point3& _center, float _radius)
    {
        center = _center;
        radius = _radius;
    }

	Vector3 CalcNormal(const Point3& outside_point)
	{
		return (outside_point - center).Normalize();
	}
    
public:
    Point3 center;
    float radius;
};

class Cube : public Object
{
public:
	Cube(const char* pFileName);
    Cube(const Color4& _diffuse, const Color4& _specular, float _shininess, 
		float _reflected_coef, float _transmitted_coef, float _refracted_idx,
		const Point3 _points[], int count)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
		, isParallel(false)
	{
		Set(_points, count);
	}

public:
	virtual bool LoadObjectFromFile(const char* pFileName);
	virtual bool Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal);
	virtual Type GetType() { return Type::CUBE; }
	virtual bool GetNormal(Vector3* pOut) const 
	{
		if (isParallel)
		{
			*pOut = normal;
			return true;
		}

		return false;
	}

public:
    void Set(const Point3 _points[], int count)
    {
		assert(count == 8);
		memcpy_s(points, sizeof(Point3) * 8, _points, sizeof(Point3) * 8);
		
		GETAXISMIN(xmin, points, CUBEPOINT_MAX, x);
		GETAXISMIN(ymin, points, CUBEPOINT_MAX, y);
		GETAXISMIN(zmin, points, CUBEPOINT_MAX, z);

		GETAXISMAX(xmax, points, CUBEPOINT_MAX, x);
		GETAXISMAX(ymax, points, CUBEPOINT_MAX, y);
		GETAXISMAX(zmax, points, CUBEPOINT_MAX, z);
    }
/*
	Vector3 CalcNormal(const Point3& outside_point, const Vector3& ray_dir)
	{
	}
*/  
public:
	bool isParallel;
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;
    Point3 points[8];
	Vector3 normal;
};

class Plane : public Object
{
public:
	Plane(const char* pFileName);
    Plane(const Color4& _diffuse, const Color4& _specular, float _shininess, 
		float _reflected_coef, float _transmitted_coef, float _refracted_idx,
		const Point3& _point, const Vector3& _normal)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
	{
		Set(_point, _normal);
	}

public:
	virtual bool LoadObjectFromFile(const char* pFileName);
	virtual bool Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal);
	virtual Type GetType() { return Type::PLANE; }
	virtual bool GetNormal(Vector3* pOut) const 
	{
		assert(pOut != nullptr);
		(*pOut) = normal;
		return true;
	}

public:
    void Set(const Point3& _point, const Vector3& _normal)
    {
		point = _point;
		normal = _normal;
		D = -(_normal.Dot(_point.ToVector3()));
    }
    
public:
	float D;
	Point3 point;
	Vector3 normal;
};

class ModelObj : public Object
{
public:
	ModelObj(const char* pFileName);

public:
    void Set(const Color4& _diffuse, const Color4& _specular, float _shininess, 
		float _reflected_coef, float _transmitted_coef, float _refracted_idx)
    {
		Object::Set(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx);
    }

	virtual bool LoadObjectFromFile(const char* pFileName);
	virtual bool Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal);
	virtual Type GetType() { return Type::OBJMODEL; }
	virtual bool GetNormal(Vector3* pOut) const;

private:
	vector<Triangle> polygons;
	ObjData obj;
};

#endif