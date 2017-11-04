#ifndef RayTracing_MacOS_Camera_h
#define RayTracing_MacOS_Camera_h

#include <math.h>
#include "simple_gmath.h"

using namespace Simple_GMath;

class Camera
{
public:
    Camera(const Point3& _eye, const Point3& _look, Vector3& _up,
           float _fov, int width, int height)
    : eye(_eye), up(_up), fov(_fov)
    {
        Setting(_eye, _look, _up, _fov, width, height);
    }

	virtual ~Camera() {}
    
public:
    void Setting (const Point3& _eye, const Point3& _look, Vector3& _up,
                  float _fov, int width, int height)
    {
		look = _look.ToVector3();
		//look = (_look - eye).Normalize();
        UpdateUV(width, height);
    }
    
    void UpdateUV(int width, int height)
    {
		float f_width = (float)width;
		float f_height = (float)height;
		
        u = look.Cross(up).Normalize();
        v = look.Cross(u).Normalize();
//        ToRadian(

		look = look.Normalize();
		o = look * (f_width / (2.0f * tan(fov * 0.5f))) - (u * (f_width * 0.5f)) - (v * (f_height * 0.5f));
    }
    
public:
    float fov;
    Point3 eye;
    Vector3 look;
    Vector3 up;
    
    Vector3 o;
    Vector3 u;
    Vector3 v;

private:
    bool isInitialized;
};

class MainCamera : public Camera
{
public:
    MainCamera(const Point3& _eye, const Point3& _look, Vector3& _up,
           float _fov, int width, int height)
    : Camera(_eye, _look, _up, _fov, width, height)
    { }
    
public:
    static void CreateInstance(const Point3& _eye, const Point3& _look, Vector3& _up,
                               float _fov, int width, int height)
    {
        if (m_pInstance == nullptr)
            m_pInstance = new MainCamera(_eye, _look, _up, _fov, width, height);
    }
    
    static MainCamera* GetInstance()
    {
        if (m_pInstance == nullptr)
            return nullptr;

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
    static MainCamera* m_pInstance;
};

#endif
