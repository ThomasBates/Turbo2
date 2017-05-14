#pragma once

#include <pch.h>

#include <ITurboSceneMesh.h>

class TurboSceneMesh : public ITurboSceneMesh
{
private:
	std::vector<TurboSceneVertex>	_vertices;
	std::vector<TurboSceneTriangle>	_triangles;

	Vector3D _minExtent = Vector3D( 10000.0,  10000.0,  10000.0);
	Vector3D _maxExtent = Vector3D(-10000.0, -10000.0, -10000.0);

public:
	//  ITurboSceneMesh Methods  ---------------------------------------------------------------------------------
	virtual void AddVertex(Vector3D position, Vector3D normal, Vector2D textureUV);
	virtual void AddTriangle(int vertex1, int vertex2, int vertex3);

	//  ITurboSceneMesh Properties  ------------------------------------------------------------------------------
	virtual std::vector<TurboSceneVertex> Vertices() { return _vertices; };
	virtual std::vector<TurboSceneTriangle> Triangles() { return _triangles; };

	virtual Vector3D MinExtent() { return _minExtent; };
	virtual Vector3D MaxExtent() { return _maxExtent; };
};