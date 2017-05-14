
#include "pch.h"

#include <CubicMaze.h>
#include <OriginalMazeEdgeMesh.h>
#include <TurboSceneMaterial.h>
#include <TurboSceneMesh.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeEdgeMesh::OriginalMazeEdgeMesh()
{
	double left__ = -(CELLHALF - WALLHALF);	//	-X
	double right_ = +(CELLHALF - WALLHALF);	//	+X
	double top___ = +WALLHALF;	//	+Y
	double bottom = -WALLHALF;	//	-Y
	double back__ = +WALLHALF;	//	+Z
	double front_ = -WALLHALF;	//	-Z


	Vector3D normal = Vector3D(0.0f, 0.0f, 1.0f);
	AddVertex(Vector3D(left__, bottom, back__), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(left__, top___, back__), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(right_, top___, back__), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(right_, bottom, back__), normal, Vector2D(1.0f, 0.0f));

	normal = Vector3D(0.0f, 0.0f, -1.0f);
	AddVertex(Vector3D(left__, top___, front_), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(left__, bottom, front_), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(right_, bottom, front_), normal, Vector2D(1.0f, 0.0f));
	AddVertex(Vector3D(right_, top___, front_), normal, Vector2D(1.0f, 1.0f));


	normal = Vector3D(0.0f, 1.0f, 0.0f);
	AddVertex(Vector3D(left__, top___, back__), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(left__, top___, front_), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(right_, top___, front_), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(right_, top___, back__), normal, Vector2D(1.0f, 0.0f));

	normal = Vector3D(0.0f, -1.0f, 0.0f);
	AddVertex(Vector3D(left__, bottom, front_), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(left__, bottom, back__), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(right_, bottom, back__), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(right_, bottom, front_), normal, Vector2D(1.0f, 0.0f));


	normal = Vector3D(-1.0f, 0.0f, 0.0f);
	AddVertex(Vector3D(left__, top___, front_), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(left__, top___, back__), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(left__, bottom, back__), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(left__, bottom, front_), normal, Vector2D(1.0f, 0.0f));

	normal = Vector3D(1.0f, 0.0f, 0.0f);
	AddVertex(Vector3D(right_, top___, back__), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(right_, top___, front_), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(right_, bottom, front_), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(right_, bottom, back__), normal, Vector2D(1.0f, 0.0f));


	AddTriangle(0, 1, 2);
	AddTriangle(2, 3, 0);

	AddTriangle(4, 5, 6);
	AddTriangle(6, 7, 4);

	AddTriangle(8, 9, 10);
	AddTriangle(10, 11, 8);

	AddTriangle(12, 13, 14);
	AddTriangle(14, 15, 12);

	AddTriangle(16, 17, 18);
	AddTriangle(18, 19, 16);

	AddTriangle(20, 21, 22);
	AddTriangle(22, 23, 20);
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboSceneMesh Methods  -------------------------------------------------------------------------------------------

//  ITurboSceneMesh Methods  -------------------------------------------------------------------------------------------
