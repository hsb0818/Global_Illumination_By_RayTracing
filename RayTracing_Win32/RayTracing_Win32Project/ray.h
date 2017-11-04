#ifndef RayTracing_ray_h
#define RayTracing_ray_h

#include "class_header.h"
#include "simple_gmath.h"
#include "Camera.h"
#include "object.h"

#define REFRACTION_AIR 1.0003f
#define REFRACTION_WATER 1.33f
#define REFRACTION_GLASS 1.52f
#define REFRACTION_DIAMOND 2.42f

using namespace Simple_GMath;

class Ray
{
public:
	Ray(const Point3& _position, const Vector3& _direction, const Object* _pHost = nullptr, float _refraction_env = REFRACTION_AIR)
    : position(_position)
	, direction(_direction.Normalize())
	, pHost(_pHost)
	, m_refraction_env(_refraction_env)
    { }

public:
    Color4 Tracing(int recursive_count = 4);	
	bool Closest_Intersect(Object** ppOut_Object, Point3* pOut_Intersected, Vector3* pOut_Normal);
	Color4 Shade(const Object* pObject, const Point3& intersected_point, const Vector3& normal, int recursive_count);

	// This is for adding light color. shadow or light.
	Vector3 CalcNormal(Object* pObject, Point3 outside_point);
	Color4 Local_Illumination(const Object* pObject, const Point3& intersected_point, const Vector3& normal);
	Color4 Reflection(const Object* pObject, const Point3& intersected_point, const Vector3& normal, float reflection_coef, int recursive_count);
	Color4 Transmission(const Object* pObject, const Point3& intersected_point, const Vector3& normal, int recursive_count);

	void SetRefractionEnv(float _refraction_env) { m_refraction_env = _refraction_env; }

public:
    Point3 position;
    Vector3 direction;
	const Object* pHost;

private:
	float m_refraction_env;
};

void RayTrace(const char* pFileName, int width, int height, Camera* pCamera = nullptr);

#endif
