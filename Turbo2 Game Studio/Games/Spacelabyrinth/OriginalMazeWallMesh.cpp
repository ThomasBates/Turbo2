
#include "pch.h"

#include <CubicMaze.h>
#include <OriginalMazeWallMesh.h>
#include <TurboSceneMesh.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeWallMesh::OriginalMazeWallMesh()
{
	double left__ = -WALLHALF;	//	-X
	double right_ = 0.0f;		//	+X
	double top___ = +(CELLHALF - WALLHALF);	//	+Y
	double bottom = -(CELLHALF - WALLHALF);	//	-Y
	double back__ = +(CELLHALF - WALLHALF);	//	+Z
	double front_ = -(CELLHALF - WALLHALF);	//	-Z


	Vector3D normal = Vector3D(-1.0f, 0.0f, 0.0f);
	AddVertex(Vector3D(left__, bottom, front_), normal, Vector2D(1.0f, 0.0f));
	AddVertex(Vector3D(left__, top___, front_), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(left__, top___, back__), normal, Vector2D(0.0f, 1.0f));
	AddVertex(Vector3D(left__, bottom, back__), normal, Vector2D(0.0f, 0.0f));

	normal = Vector3D(1.0f, 0.0f, 0.0f);
	AddVertex(Vector3D(right_, bottom, front_), normal, Vector2D(0.0f, 0.0f));
	AddVertex(Vector3D(right_, bottom, back__), normal, Vector2D(1.0f, 0.0f));
	AddVertex(Vector3D(right_, top___, back__), normal, Vector2D(1.0f, 1.0f));
	AddVertex(Vector3D(right_, top___, front_), normal, Vector2D(0.0f, 1.0f));


	AddTriangle(0, 1, 2);
	AddTriangle(2, 3, 0);

	AddTriangle(4, 5, 6);
	AddTriangle(6, 7, 4);
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
