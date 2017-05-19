#pragma once

#include <TurboVector2D.h>
#include <TurboVector3D.h>
#include <ITurboSceneMaterial.h>

struct TurboSceneVertex
{
	TurboVector3D Position;
	TurboVector3D Normal;
	TurboVector2D TextureUV;
};

struct TurboSceneTriangle
{
	unsigned short Vertex1;
	unsigned short Vertex2;
	unsigned short Vertex3;
};

class ITurboSceneMesh
{
public:
	//  ITurboSceneMesh Methods ----------------------------------------------------------------------------------------
	virtual void AddVertex(TurboVector3D position, TurboVector3D normal, TurboVector2D textureUV) = 0;
	virtual void AddTriangle(int vertex1, int vertex2, int vertex3) = 0;

	//  ITurboSceneMesh Properties -------------------------------------------------------------------------------------
	virtual std::vector<TurboSceneVertex> Vertices() = 0;
	virtual std::vector<TurboSceneTriangle> Triangles() = 0;

	virtual TurboVector3D MinExtent() = 0;
	virtual TurboVector3D MaxExtent() = 0;
};