#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "ray.h"
#include "light.h"
#include "object.h"
#include "Camera.h"
#include "define_manager.h"
#include "make_ppm.h"

using namespace Simple_GMath;

void RayTrace(const char* pFileName, int width, int height, Camera* pCamera)
{
	FILE* pFile = CreatePPM(pFileName, width, height);
    if (pFile == nullptr)
        return;
    
    if (pCamera == nullptr)
        pCamera = MainCamera::GetInstance();

	assert(pCamera != nullptr);

	pCamera->UpdateUV(width, height);

	Point3 eye = pCamera->eye;
	Vector3 o = pCamera->o;
	Vector3 u = pCamera->u;
	Vector3 v = pCamera->v;

	printf("Start RayTracing\n");
	for (float y=0; y<height; y+=1.f)
	{
		for (float x=0; x<width; x+=1.f)
        {
            // calc the pixel color.
			Ray ray(eye, o + u*x + v*y);
			Color4 pixel = ray.Tracing(10);

			DrawPPM(pFile, pixel);
		}

		printf("\r");
		printf("%d%%", (int)((y / height) * 100.f));
    }
	printf("\r");
	printf("100%%\n");
    
    fclose(pFile);
}

Color4 Ray::Tracing(int recursive_count)
{
	if (recursive_count <= 0)
		return NONE;
	Point3 intersection_point;
	Vector3 normal;
	Object* pObject = nullptr;

	if (Closest_Intersect(&pObject, &intersection_point, &normal))
		return Shade(pObject, intersection_point, normal, recursive_count);
	else
		return BLACK;
}

bool Ray::Closest_Intersect(Object** ppOut_Object, Point3* pOut_Intersected, Vector3* pOut_Normal)
{
	assert(pOut_Intersected != nullptr);

	float length_min = numeric_limits<float>::max();
	Point3 closest_point;
	Vector3 closest_normal;
	Object* pObject = nullptr;

	ObjectManager::LIST& obj_list = ObjectMng().GetList();
	ObjectManager::LIST::iterator itr = obj_list.begin();
	for (; itr != obj_list.end(); ++itr)
	{
		if ((*itr) == this->pHost)
			continue;

		Vector3 normal;
		Point3 intersected_point;
		if ((*itr)->Intersection(this, &intersected_point, &normal))
		{
			float length = (intersected_point - this->position).Length();
			if (length < length_min)
			{
				length_min = length;
				pObject = (*itr);
				closest_point = intersected_point;
				closest_normal = normal;
			}
		}
	}

	if (pObject != nullptr)
	{
		(*pOut_Intersected) = closest_point;
		(*pOut_Normal) = closest_normal;
		(*ppOut_Object) = pObject;

		return true;
	}

	return false;
}

Color4 Ray::Shade(const Object* pObject, const Point3& intersected_point, const Vector3& normal, int recursive_count)
{
	assert(pObject != nullptr);
	if (recursive_count <= 0)
		return NONE;

	Color4 total;

	total += Local_Illumination(pObject, intersected_point, normal);	/* DEFAULT_AMBIENT + */ 
	if (recursive_count > 0)
		total += Reflection(pObject, intersected_point, normal, pObject->reflected_coef, recursive_count -1);
	if (pObject->transmitted_coef > 0)
		total += Transmission(pObject, intersected_point, normal, recursive_count -1);

	return total.Clamp();
}

Color4 Ray::Local_Illumination(const Object* pObject, const Point3& intersected_point, const Vector3& normal)
{
	assert(pObject != nullptr);

	Color4 total;
	Vector3 dir_ray = this->direction.Normalize();
	Vector3 N = normal.Normalize();
	Object* pColObj = nullptr;
	Point3 colPoint;
	Vector3 colNormal;

	LightManager::LIST& light_list = LightMng().GetList();
	LightManager::LIST::iterator itr = light_list.begin();
	for (; itr != light_list.end(); ++itr)
	{
		if ((*itr)->GetType() != Light::Type::Point)
			continue;
		
		PointLight* pPL = static_cast<PointLight*>((*itr));
		assert(pPL != nullptr);

		Ray ray(intersected_point, (pPL->position - intersected_point).Normalize(), pObject);
		if (ray.Closest_Intersect(&pColObj, &colPoint, &colNormal))
			 continue;

		Vector3 L = (pPL->position - intersected_point).Normalize();
		Vector3 H = (L + (dir_ray * (-1.f))) * 0.5f;
		float diffuse = N.Dot(L);
		float specular = pow(N.Dot(H), pObject->shininess);

		if (diffuse < 0.f)
			diffuse = 0.f;
		if (specular < 0.f)
			specular = 0.f;
		
		Color4 color = pPL->color * ((pObject->diffuse * diffuse) + (pObject->specular * specular));
		total += color;
	}

	return total;
}

Color4 Ray::Reflection(const Object* pObject, const Point3& intersected_point, const Vector3& normal, float reflection_coef, int recursive_count)
{
	if (reflection_coef <= 0.f)
		return NONE;

	Vector3 I = this->direction;
	Vector3 reverse_I = I * (-1.f);
	Vector3 R = normal * (normal.Dot(reverse_I) * 2.f) + I;

	return (Ray(intersected_point, R, pObject).Tracing(recursive_count) * reflection_coef);
}

Color4 Ray::Transmission(const Object* pObject, const Point3& intersected_point, const Vector3& normal, int recursive_count)
{
	assert(pObject != nullptr);

	Vector3 N = normal.Normalize();
	Vector3 I = this->direction;

	float n = 0.f;
	if (N.Dot(I) >= 0.f)
	{
		N = N * (-1.f);
		n = m_refraction_env / pObject->refracted_idx;
	}
	else
		n = pObject->refracted_idx / m_refraction_env;
	n = 1.f/n;

	float dotIN = I.Dot(N);
	Vector3 T = N * (-1.f) * sqrt(1.f - n*n * (1.f - pow(dotIN, 2))) + (I - N*dotIN) * n;

	return (Ray(intersected_point, T, pObject).Tracing(recursive_count).Clamp() * pObject->transmitted_coef);
}

Vector3 Ray::CalcNormal(Object* pObject, Point3 outside_point)
{
	assert(pObject != nullptr);

	switch(pObject->GetType())
	{
	case Object::Type::SPHERE:
		{
			Sphere* pSphere = static_cast<Sphere*>(pObject);
			return pSphere->CalcNormal(outside_point);
		}
	case Object::Type::CUBE:
		{
			break;
		}
	}

	return Vector3();
}