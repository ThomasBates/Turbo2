
#include "pch.h"

#include <CubicMaze.h>
#include <IntroMazeCornerMesh.h>
#include <TurboSceneMesh.h>

using namespace Turbo::Math;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

IntroMazeCornerMesh::IntroMazeCornerMesh()
{
	double left__ = -WALLHALF;	//	-X
	double right_ = +WALLHALF;	//	+X
	double top___ = +WALLHALF;	//	+Y
	double bottom = -WALLHALF;	//	-Y
	double back__ = +WALLHALF;	//	+Z
	double front_ = -WALLHALF;	//	-Z


	TurboVector3D normal = TurboVector3D(0.0f, 0.0f, 1.0f);
	AddVertex(TurboVector3D(left__, bottom, front_), normal, TurboVector2D(0.0f, 0.0f));
	AddVertex(TurboVector3D(right_, bottom, front_), normal, TurboVector2D(1.0f, 0.0f));
	AddVertex(TurboVector3D(right_, top___, front_), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(left__, top___, front_), normal, TurboVector2D(0.0f, 1.0f));

	normal = TurboVector3D(0.0f, 0.0f, -1.0f);
	AddVertex(TurboVector3D(left__, bottom, back__), normal, TurboVector2D(1.0f, 0.0f));
	AddVertex(TurboVector3D(left__, top___, back__), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(right_, top___, back__), normal, TurboVector2D(0.0f, 1.0f));
	AddVertex(TurboVector3D(right_, bottom, back__), normal, TurboVector2D(0.0f, 0.0f));


	normal = TurboVector3D(0.0f, 1.0f, 0.0f);
	AddVertex(TurboVector3D(left__, bottom, front_), normal, TurboVector2D(0.0f, 1.0f));
	AddVertex(TurboVector3D(left__, bottom, back__), normal, TurboVector2D(0.0f, 0.0f));
	AddVertex(TurboVector3D(right_, bottom, back__), normal, TurboVector2D(1.0f, 0.0f));
	AddVertex(TurboVector3D(right_, bottom, front_), normal, TurboVector2D(1.0f, 1.0f));

	normal = TurboVector3D(0.0f, -1.0f, 0.0f);
	AddVertex(TurboVector3D(left__, top___, front_), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(right_, top___, front_), normal, TurboVector2D(0.0f, 1.0f));
	AddVertex(TurboVector3D(right_, top___, back__), normal, TurboVector2D(0.0f, 0.0f));
	AddVertex(TurboVector3D(left__, top___, back__), normal, TurboVector2D(1.0f, 0.0f));


	normal = TurboVector3D(1.0f, 0.0f, 0.0f);
	AddVertex(TurboVector3D(left__, bottom, front_), normal, TurboVector2D(1.0f, 0.0f));
	AddVertex(TurboVector3D(left__, top___, front_), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(left__, top___, back__), normal, TurboVector2D(0.0f, 1.0f));
	AddVertex(TurboVector3D(left__, bottom, back__), normal, TurboVector2D(0.0f, 0.0f));

	normal = TurboVector3D(-1.0f, 0.0f, 0.0f);
	AddVertex(TurboVector3D(right_, bottom, front_), normal, TurboVector2D(0.0f, 0.0f));
	AddVertex(TurboVector3D(right_, bottom, back__), normal, TurboVector2D(1.0f, 0.0f));
	AddVertex(TurboVector3D(right_, top___, back__), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(right_, top___, front_), normal, TurboVector2D(0.0f, 1.0f));

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

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneMesh Methods --------------------------------------------------------------------------------------------

//  ITurboSceneMesh Methods --------------------------------------------------------------------------------------------
