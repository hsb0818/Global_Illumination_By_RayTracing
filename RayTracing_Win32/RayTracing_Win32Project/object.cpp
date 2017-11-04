#include "object.h"
#include "ray.h"
#include <assert.h>

#define GETMINMAX_T(DEST_TMIN, DEST_TMAX, XMIN, XMAX, START_POS, RAY_DIR, AXIS) \
{ \
	if (RAY_DIR.AXIS > 0)	\
	{ \
		DEST_TMIN = (XMIN - START_POS.AXIS) / RAY_DIR.AXIS; \
		DEST_TMAX = (XMAX - START_POS.AXIS) / RAY_DIR.AXIS; \
	} \
	else \
	{ \
		DEST_TMIN = (XMAX - START_POS.AXIS) / RAY_DIR.AXIS; \
		DEST_TMAX = (XMIN - START_POS.AXIS) / RAY_DIR.AXIS; \
	} \
} \

ObjectManager* ObjectManager::m_pInstance = nullptr;

void ObjectManager::Free()
{
	LIST::iterator itr = list.begin();
	for (; itr!=list.end(); ++itr)
	{
		if ((*itr) == nullptr)
			continue;

		delete (*itr);
	}
	list.clear();

	LIST(list).swap(list); // for reset the memory space. this is a simple skill. :)
}

///////////////
Sphere::Sphere(const char* pFileName) { this->LoadObjectFromFile(pFileName); }
Cube::Cube(const char* pFileName) { this->LoadObjectFromFile(pFileName); }
Plane::Plane(const char* pFileName) { this->LoadObjectFromFile(pFileName); }
ModelObj::ModelObj(const char* pFileName) { this->LoadObjectFromFile(pFileName); }

bool Triangle::Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal)
{
	assert(pRay != nullptr);

	Vector3 eye = pRay->position.ToVector3();
	Vector3 dir = pRay->direction.Normalize();

	Vector3 nv1 = (point[1] - point[0]);
	Vector3 nv2 = (point[2] - point[0]);
	Vector3 n = nv1.Cross(nv2); // normal.Normalize();

	Plane plane(BLACK, BLACK, 0.f, 
		0.f, 0.f, 0.f, 
		point[0], n);

	Point3 temp_intersect_p;
	Vector3 normal_intersect;
	
	if (plane.Intersection(pRay, &temp_intersect_p, &normal_intersect))
	{
		bool temp1 = false;
		bool temp2 = false;
		bool temp3 = false;

		//for edge 1
		Vector3 poly_edge = point[1] - point[0];
		Vector3 point_edge = temp_intersect_p - point[0];
		Vector3 cross_vec = poly_edge.Cross(point_edge);
		if (n.Dot(cross_vec) > 0.f)
			temp1 = true;

		//for edge 2
		poly_edge = point[2] - point[1];
		point_edge = temp_intersect_p - point[1];
		cross_vec = poly_edge.Cross(point_edge);
		if (n.Dot(cross_vec) > 0.f)
			temp2 = true;		

		//for edge 3
		poly_edge = point[0] - point[2];
		point_edge = temp_intersect_p - point[2];
		cross_vec = poly_edge.Cross(point_edge);
		if (n.Dot(cross_vec) > 0.f)
			temp3 = true;		

		if( ((temp1 == true) && (temp2 == true) && (temp3 == true)) 
			|| ((temp1 == false) && (temp2 == false) && (temp3 ==false)) )
		{
			*pOut_Intersect = temp_intersect_p;

			if (n.Dot(dir) > 0.f)
				n = n * (-1.f);

			normal = n;
			*pOut_Normal = normal;

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void Triangle::Set(const Point3& p1, const Point3& p2, const Point3& p3)
{
	Vector3 v1 = p2 - p1;
	Vector3 v2 = p3 - p1;
	point[0] = p1;
	point[1] = p2;
	point[2] = p3;
	normal = v1.Cross(v2);

	Vector3 n = normal.Normalize();
	D = -(n.Dot(p1.ToVector3()));
}

bool Sphere::LoadObjectFromFile(const char* pFileName)
{
	return true;
}

bool Sphere::Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal)
{
	assert(pRay != nullptr);

	Point3 eye = pRay->position;
	Vector3 dir = pRay->direction;
	Vector3 ray_to_center = center - eye;
	float len_rtc = ray_to_center.Length();
	float len_tc = ray_to_center.Dot(dir);

	if(len_tc <= 0.f) 
	{
		return false;
	}

	float b_square = len_rtc * len_rtc - len_tc * len_tc;

	if (b_square > (radius * radius))
	{
		return false;
	}

	float tu_square = radius * radius - b_square;
	float tu = 0.f;
	if (tu_square > 0.f)
		tu = sqrt(tu_square);

	float dist = ray_to_center.Length();
	// ray's start position is inside sphere or outside sphere.
	// If dist > radius, it's meaning is outside sphere.
	if (dist >= radius)
		(*pOut_Intersect) = eye + (dir * (len_tc - tu));
	else
		(*pOut_Intersect) = eye + (dir * (len_tc + tu));

	(*pOut_Normal) = (*pOut_Intersect - center).Normalize();

	return true;
}

bool Cube::LoadObjectFromFile(const char* pFileName)
{   
	return true;
}

bool Cube::Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal)
{
	assert(pRay != nullptr);
	
	float tx_min = 0.f, ty_min = 0.f, tz_min = 0.f;
	float tx_max = 0.f, ty_max = 0.f, tz_max = 0.f;

	GETMINMAX_T(tx_min, tx_max, xmin, xmax, pRay->position, pRay->direction, x);
	GETMINMAX_T(ty_min, ty_max, ymin, ymax, pRay->position, pRay->direction, y);
	GETMINMAX_T(tz_min, tz_max, zmin, zmax, pRay->position, pRay->direction, z);

	float t_min = 0.f;
	float t_max = 0.f;

	// Calc the range in here.
	// min range
	t_min = (tx_min > ty_min ) ? tx_min : ty_min;
	t_min = (t_min > tz_min) ? t_min : tz_min;

	// max range
	t_max = (tx_max < ty_max ) ? tx_max : ty_max;
	t_max = (t_max < tz_max) ? t_max : tz_max;

	if (t_min <= t_max)
	{
		if (t_min == tx_min)
		{
			Vector3 _normal(1.f, 0.f, 0.f);
			if (pRay->direction.Dot(_normal) > 0.f)
				_normal = _normal * (-1.f);
			normal = _normal;
		}
		else if (t_min == ty_min)
		{
			Vector3 _normal(0.f, 1.f, 0.f);
			if (pRay->direction.Dot(_normal) > 0.f)
				_normal = _normal * (-1.f);
			normal = _normal;
		}
		else if (t_min == tz_min)
		{
			Vector3 _normal(0.f, 0.f, 1.f);
			if (pRay->direction.Dot(_normal) > 0.f)
				_normal = _normal * (-1.f);
			normal = _normal;
		}

		// need to calc if the cube parallel from axis.
		isParallel = true;

		(*pOut_Intersect) = pRay->position + (pRay->direction * t_min);
		(*pOut_Normal) = normal;
		return true;
	}

	return false;
}

bool Plane::LoadObjectFromFile(const char* pFileName)
{   
	return true;
}

bool Plane::Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal)
{
	assert(pRay != nullptr);
	
	Vector3 eye = pRay->position.ToVector3();
	Vector3 dir = pRay->direction.Normalize();
	float t_n = normal.Dot(dir);

	if (t_n == 0.f)
	{
		return false;
	}
	else
	{
		float nde = normal.x * eye.x + normal.y * eye.y + normal.z * eye.z;
		float t = - (nde + D) / t_n;

		if (t <= 0.f)
		{
			return false;
		}

		(*pOut_Intersect) = pRay->position + dir * t;
		(*pOut_Normal) = normal;
		return true;
	}
}

bool ModelObj::LoadObjectFromFile(const char* pFileName)
{
	obj.Load(pFileName);
	if (obj.isLoaded == false)
		return false;

	vector<Face>& faces = obj.GetFaces();
	vector<Point3>& vertices = obj.GetVertices();
	vector<Vector3>& normals = obj.GetNormals();

	polygons.reserve(obj.GetNumberOfFaces());
	for(int i = 0; i < obj.GetNumberOfFaces(); i++) 
	{			
		Face& face = faces[i];
		polygons.push_back(Triangle(vertices[face.vertex_idx[0]], vertices[face.vertex_idx[1]], vertices[face.vertex_idx[2]],
			normals[face.normal_idx[0]], normals[face.normal_idx[1]], normals[face.normal_idx[2]]));
	}

	return true;
}

bool ModelObj::Intersection(const Ray* pRay, Point3* pOut_Intersect, Vector3* pOut_Normal)
{
	if (polygons.empty())
		return false;

	bool isIntersected = false;
	float dist = numeric_limits<float>::max();
	float temp_dist = 0.f;
	Vector3 normal;
	Point3 intersected_point;

	for (int i=0; i<(int)polygons.size(); ++i)
	{
		bool isTrue = polygons[i].Intersection(pRay, &intersected_point, &normal);
		if (isTrue == false)
			continue;

		temp_dist = (pRay->position - intersected_point).Length();
		if (temp_dist < dist)
		{	
			isIntersected = true;

			Vector3 n1 = this->obj.GetNormals()[this->obj.GetFaces()[i].normal_idx[0]];
			Vector3 n2 = this->obj.GetNormals()[this->obj.GetFaces()[i].normal_idx[1]];
			Vector3 n3 = this->obj.GetNormals()[this->obj.GetFaces()[i].normal_idx[2]];

			normal = ((n1 + n2 + n3) / 3.f).Normalize();
			if (normal.Dot(pRay->direction) > 0.f)
				normal = normal * (-1.f);

			dist = temp_dist;
			*pOut_Intersect = intersected_point;
			*pOut_Normal = normal;
		}
	}

	return isIntersected;
}

bool ModelObj::GetNormal(Vector3* pOut) const
{
	return false;
}