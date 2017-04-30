#pragma once

#include <Vector2D.h>
#include <Vector3D.h>

struct TurboSceneObjectVertex
{
	Vector3D Position;
	Vector3D Normal;
	Vector2D TextureUV;
};

struct TurboSceneObjectTriangle
{
	unsigned short Vertex1;
	unsigned short Vertex2;
	unsigned short Vertex3;
};

class ITurboSceneObjectMesh
{
public:
	//  ITurboSceneObjectMesh Methods  ---------------------------------------------------------------------------------
	virtual void AddVertex(Vector3D position, Vector3D normal, Vector2D textureUV) = 0;
	virtual void AddTriangle(int vertex1, int vertex2, int vertex3) = 0;

	//  ITurboSceneObjectMesh Properties  ------------------------------------------------------------------------------
	virtual std::vector<TurboSceneObjectVertex> Vertices() = 0;
	virtual std::vector<TurboSceneObjectTriangle> Triangles() = 0;
	virtual Vector3D MinExtent() = 0;
	virtual Vector3D MaxExtent() = 0;

};