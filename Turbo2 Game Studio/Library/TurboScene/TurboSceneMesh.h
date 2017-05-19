#pragma once

#include <pch.h>

#include <ITurboSceneMesh.h>

class TurboSceneMesh : public ITurboSceneMesh
{
private:
	std::vector<TurboSceneVertex>	_vertices;
	std::vector<TurboSceneTriangle>	_triangles;

	TurboVector3D _minExtent = TurboVector3D( 10000.0,  10000.0,  10000.0);
	TurboVector3D _maxExtent = TurboVector3D(-10000.0, -10000.0, -10000.0);

public:
	//  ITurboSceneMesh Methods ----------------------------------------------------------------------------------------
	virtual void AddVertex(TurboVector3D position, TurboVector3D normal, TurboVector2D textureUV);
	virtual void AddTriangle(int vertex1, int vertex2, int vertex3);

	//  ITurboSceneMesh Properties -------------------------------------------------------------------------------------
	virtual std::vector<TurboSceneVertex> Vertices() { return _vertices; };
	virtual std::vector<TurboSceneTriangle> Triangles() { return _triangles; };

	virtual TurboVector3D MinExtent() { return _minExtent; };
	virtual TurboVector3D MaxExtent() { return _maxExtent; };
};