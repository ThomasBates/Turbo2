
#include <pch.h>

#include <CubicMaze.h>
#include <CubicMazeSignMesh.h>
#include <TurboSceneMesh.h>

using namespace Turbo::Math;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSignMesh::CubicMazeSignMesh()
{
	double left__ = -WALLHALF - 0.01;				//	-X
	double right_ = 0.0 - 0.01;						//	+X
	double top___ = +(CELLHALF - WALLHALF) * 0.5;	//	+Y
	double bottom = -(CELLHALF - WALLHALF) * 0.5;	//	-Y
	double back__ = +(CELLHALF - WALLHALF) * 0.9;	//	+Z
	double front_ = -(CELLHALF - WALLHALF) * 0.9;	//	-Z


	TurboVector3D normal = TurboVector3D(-1.0f, 0.0f, 0.0f);
	AddVertex(TurboVector3D(left__, bottom, front_), normal, TurboVector2D(0.0f, 0.0f));
	AddVertex(TurboVector3D(left__, top___, front_), normal, TurboVector2D(0.0f, 1.0f));
	AddVertex(TurboVector3D(left__, top___, back__), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(left__, bottom, back__), normal, TurboVector2D(1.0f, 0.0f));

	normal = TurboVector3D(1.0f, 0.0f, 0.0f);
	AddVertex(TurboVector3D(right_, bottom, front_), normal, TurboVector2D(0.0f, 0.0f));
	AddVertex(TurboVector3D(right_, bottom, back__), normal, TurboVector2D(1.0f, 0.0f));
	AddVertex(TurboVector3D(right_, top___, back__), normal, TurboVector2D(1.0f, 1.0f));
	AddVertex(TurboVector3D(right_, top___, front_), normal, TurboVector2D(0.0f, 1.0f));


	AddTriangle(0, 1, 2);
	AddTriangle(2, 3, 0);

	AddTriangle(4, 5, 6);
	AddTriangle(6, 7, 4);
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
