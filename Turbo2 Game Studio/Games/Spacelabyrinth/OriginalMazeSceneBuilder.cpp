
#include "pch.h"

#include <time.h>

#include <CubicMaze.h>
#include <OriginalCubicMazeFactory.h>
#include <OriginalMazeCornerObject.h>
#include <OriginalMazeEdgeObject.h>
#include <OriginalMazeWallObject.h>
#include <OriginalMazeSceneBuilder.h>
#include <SpaceLabyrinthOriginalScene.h>
#include <TurboSceneObject.h>
#include <TurboSceneObjectMesh.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeSceneBuilder::OriginalMazeSceneBuilder(std::shared_ptr<ITurboApplicationPlatform> platform)
{
	_platform = platform;
}

//  ITurboSceneBuilder Methods  ----------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> OriginalMazeSceneBuilder::BuildScene()
{
	std::unique_ptr<ICubicMazeFactory> factory = std::unique_ptr<ICubicMazeFactory>(new OriginalCubicMazeFactory());
	std::shared_ptr<CubicMaze> maze = factory->MakeMaze(3, 3, 3);

	std::shared_ptr<ITurboScene> scene = std::shared_ptr<ITurboScene>(new SpaceLabyrinthOriginalScene(_platform));
	BuildSceneObjects(scene, maze);
	factory->FreeMaze(maze);

	return scene;
}

void OriginalMazeSceneBuilder::FreeScene(std::shared_ptr<ITurboScene> scene)
{
}

//  ITurboSceneBuilder Methods  ----------------------------------------------------------------------------------------
//  Local Support Methods  ---------------------------------------------------------------------------------------------

void OriginalMazeSceneBuilder::BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> maze)
{
	location size = maze->GetSize();
	Array3D mazeArray = maze->GetMazeArray();

	std::shared_ptr<ITurboSceneObjectTexture> cornerTexture = _platform->LoadTexture("OriginalCorner");
	std::shared_ptr<ITurboSceneObjectTexture> edgeTexture = _platform->LoadTexture("OriginalEdge");
	std::shared_ptr<ITurboSceneObjectTexture> wallTexture = _platform->LoadTexture("OriginalWall");
	std::shared_ptr<ITurboSceneObjectTexture> floorTexture = _platform->LoadTexture("OriginalFloor");
	std::shared_ptr<ITurboSceneObjectTexture> ceilingTexture = _platform->LoadTexture("OriginalCeiling");

	for (int w = 0; w <= size.w; w++)
	for (int h = 0; h <= size.h; h++)
	for (int d = 0; d <= size.d; d++)
	{
		scene->AddSceneObject(BuildCorner(w, h, d, cornerTexture));

		if (w>0)	scene->AddSceneObject(BuildWEdge(w, h, d, edgeTexture));
		if (h>0)	scene->AddSceneObject(BuildHEdge(w, h, d, edgeTexture));
		if (d>0)	scene->AddSceneObject(BuildDEdge(w, h, d, edgeTexture));
		
		char walls = mazeArray[w][h][d];
		if (GETBIT(walls, RIGHTBIT ) > 0)	scene->AddSceneObject(BuildRightWall (w, h, d, wallTexture, wallTexture));
		if (GETBIT(walls, BOTTOMBIT) > 0)	scene->AddSceneObject(BuildBottomWall(w, h, d, floorTexture, ceilingTexture));
		if (GETBIT(walls, FRONTBIT ) > 0)	scene->AddSceneObject(BuildFrontWall (w, h, d, wallTexture, wallTexture));
	}
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildCorner(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	return CreateCornerObject(mazeObject, texture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildWEdge(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	return CreateEdgeObject(mazeObject, texture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildHEdge(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	return CreateEdgeObject(mazeObject, texture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildDEdge(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	return CreateEdgeObject(mazeObject, texture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildRightWall(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> leftTexture, std::shared_ptr<ITurboSceneObjectTexture> rightTexture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	return CreateWallObject(mazeObject, leftTexture, rightTexture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildBottomWall(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> topTexture, std::shared_ptr<ITurboSceneObjectTexture> bottomTexture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	return CreateWallObject(mazeObject, topTexture, bottomTexture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::BuildFrontWall(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> backTexture, std::shared_ptr<ITurboSceneObjectTexture> frontTexture)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	return CreateWallObject(mazeObject, backTexture, frontTexture);
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::CreateCornerObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = _platform->CreateMesh();

	return std::shared_ptr<ITurboSceneObject>(new OriginalMazeCornerObject(mesh, mazeObject, texture));
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::CreateEdgeObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = _platform->CreateMesh();

	return std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mesh, mazeObject, texture));
}

std::shared_ptr<ITurboSceneObject> OriginalMazeSceneBuilder::CreateWallObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture1, std::shared_ptr<ITurboSceneObjectTexture> texture2)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = _platform->CreateMesh();

	return std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mesh, mazeObject, texture1, texture2));
}

//  Local Support Methods  ---------------------------------------------------------------------------------------------

