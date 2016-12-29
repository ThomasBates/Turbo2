#pragma once

#include <pch.h>

#include <ITurboSceneObjectMesh.h>

class TurboSceneObjectMesh : public ITurboSceneObjectMesh
{
private:
	std::vector<TurboSceneObjectVertex>			_vertices;
	std::vector<TurboSceneObjectTriangle>		_triangles;
	Vector3D _minExtent = Vector3D( 10000.0,  10000.0,  10000.0);
	Vector3D _maxExtent = Vector3D(-10000.0, -10000.0, -10000.0);

	TurboSceneObjectTriangle CreateTriangle(int vertex1, int vertex2, int vertex3);

public:
	//  ITurboSceneObjectMesh Methods  ---------------------------------------------------------------------------------
	virtual void AddVertex(Vector3D position, Vector3D color);
	virtual void AddVertex(Vector3D position, Vector3D normal, Vector2D textureCoordinate);
	virtual void AddTriangle(int vertex1, int vertex2, int vertex3);
	virtual void AddTriangle(int vertex1, int vertex2, int vertex3, std::shared_ptr<ITurboSceneObjectTexture> texture);

	//  ITurboSceneObjectMesh Properties  ------------------------------------------------------------------------------
	virtual std::vector<TurboSceneObjectVertex> Vertices() { return _vertices; };
	virtual std::vector<TurboSceneObjectTriangle> Triangles() { return _triangles; };
	virtual Vector3D MinExtent() { return _minExtent; };
	virtual Vector3D MaxExtent() { return _maxExtent; };
};