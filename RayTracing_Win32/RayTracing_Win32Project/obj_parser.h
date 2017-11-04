#ifndef __OBJPARSER_H__
#define __OBJPARSER_H__

#include "simple_gmath.h"

using namespace std;
using namespace Simple_GMath;

enum OBJ_OPTION
{
	V,
	VT,
	VN,
	F,
};

class CStringTokenizer;
class Face
{
public:
	Face() 
	{
		memset(vertex_idx, 0, sizeof(vertex_idx));
		memset(texcrd_idx, 0, sizeof(texcrd_idx));
		memset(normal_idx, 0, sizeof(normal_idx));
	}
	Face(int _vertex_idx[3], int _texcrd_idx[3], int _normal_idx[3])
	{
		memcpy_s(vertex_idx, sizeof(int) * 3, _vertex_idx, sizeof(int) * 3);
		memcpy_s(texcrd_idx, sizeof(int) * 3, _texcrd_idx, sizeof(int) * 3);
		memcpy_s(normal_idx, sizeof(int) * 3, _normal_idx, sizeof(int) * 3);
	}

public:
	int vertex_idx[3];
	int texcrd_idx[3];
	int normal_idx[3];
};

class ObjData
{
public:
	ObjData() : isLoaded(false) 
	{}
	ObjData(const char* pFileName) { Load(pFileName); }

public:
	bool Load(const char* pFileName);
	int  GetNumberOfVertices() const { return m_vertices.size(); }
	int  GetNumberOfFaces()	   const { return m_faces.size(); }
	__int64 GetFileSize() const { return m_fileSize; }
	void Display();
	Vector3* GetNormal(const int idx);
	Point3* GetVertex(const int idx);

	vector<Point3>& GetVertices() { return m_vertices; }
	vector<Point3>& GetVerticesTexture() { return m_vertices_texture; }
	vector<Vector3>& GetNormals() { return m_normals; }
	vector<Face>& GetFaces() { return m_faces; }

private:
	void ProcessData(OBJ_OPTION option, string& line);

public:
	bool isLoaded;

private:
	vector<Point3> m_vertices;
	vector<Point3> m_vertices_texture;
	vector<Vector3> m_normals;
	vector<Face> m_faces;

	__int64 m_fileSize;
};

#endif