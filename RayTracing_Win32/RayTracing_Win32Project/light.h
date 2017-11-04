#ifndef _RayTracing_light_h_
#define _RayTracing_light_h_

#include "simple_gmath.h"
#include <list>

using namespace std;
using namespace Simple_GMath;

class Light;
class LightManager
{
public:
	typedef list<Light*> LIST;
public:
	~LightManager() { Free(); }

public:
    void Insert(Light* pLight) { list.push_back(pLight); }
	void Remove(Light* pLight) { list.remove(pLight); }
	void Free();

	LIST& GetList() { return list; }
    
public:
	LIST list;

private:
    static LightManager* m_pInstance;
    
public:
    static LightManager* GetInstance()
    {
        if (m_pInstance == nullptr)
            m_pInstance = new LightManager();
        
        return m_pInstance;
    }    

	static void FreeInstance()
	{
		if (m_pInstance == nullptr)
			return;

		delete m_pInstance;
		m_pInstance = nullptr;
	}
};

///////////////////
class Light
{
public:
	Light(const Color4& _color)
		: color(_color)
	{}

	virtual ~Light()
	{ }

public:
	enum Type
	{
		Directional,
		Point,
		Spot,
	};	

public:
	virtual Type GetType() const = 0;

public:
    Color4 color;
};

class PointLight : public Light
{
public:
	PointLight(const Point3& _position, const Color4& _color)
		: position(_position), Light(_color)
	{}

public:
	virtual Type GetType() const { return Type::Point; }

public:
    Point3 position;
};

class SpotLight : public Light
{
public:
	SpotLight(const Point3& _position, const Vector3& _direction, Color4 _color)
		: position(_position), direction(_direction), Light(_color)
	{}

public:
	virtual Type GetType() const { return Type::Spot; }

public:
    Vector3 direction;
    Point3 position;
};

class DirectLight : public Light
{
public:
	DirectLight(const Vector3& _direction, const Color4& _color)
		: direction(_direction), Light(_color)
	{}

public:
	virtual Type GetType() const { return Type::Directional; }

public:
    Vector3 direction;
};

#endif
