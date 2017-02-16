
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
#include <TurboSceneObjectMaterial.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeSceneBuilder::OriginalMazeSceneBuilder()
{
}

//  ITurboSceneBuilder Methods  ----------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> OriginalMazeSceneBuilder::BuildScene()
{
	std::unique_ptr<ICubicMazeFactory> factory = std::unique_ptr<ICubicMazeFactory>(new OriginalCubicMazeFactory());
	std::shared_ptr<CubicMaze> maze = factory->MakeMaze(3, 3, 3);

	std::shared_ptr<ITurboScene> scene = std::shared_ptr<ITurboScene>(new SpaceLabyrinthOriginalScene());
	BuildSceneObjects(scene, maze);
	factory->FreeMaze(maze);

	return scene;
}

//  ITurboSceneBuilder Methods  ----------------------------------------------------------------------------------------
//  Local Support Methods  ---------------------------------------------------------------------------------------------

void OriginalMazeSceneBuilder::BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> maze)
{
	location size = maze->GetSize();
	Array3D mazeArray = maze->GetMazeArray();

	std::shared_ptr<ITurboSceneObjectMaterial> cornerMaterial = std::shared_ptr<ITurboSceneObjectMaterial>(new TurboSceneObjectMaterial(
		TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
		TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
		TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
		1.0f,
		"OriginalCorner",
		"StandardVertexShader",
		"StandardPixelShader"));
	std::shared_ptr<ITurboSceneObjectMaterial> edgeMaterial = std::shared_ptr<ITurboSceneObjectMaterial>(new TurboSceneObjectMaterial(
		TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
		TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
		TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
		1.0f,
		"OriginalEdge",
		"StandardVertexShader",
		"StandardPixelShader"));
	std::shared_ptr<ITurboSceneObjectMaterial> wallMaterial = std::shared_ptr<ITurboSceneObjectMaterial>(new TurboSceneObjectMaterial(
		TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
		TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
		TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
		1.0f,
		"OriginalWall",
		"StandardVertexShader",
		"StandardPixelShader"));
	std::shared_ptr<ITurboSceneObjectMaterial> floorMaterial = std::shared_ptr<ITurboSceneObjectMaterial>(new TurboSceneObjectMaterial(
		TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
		TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
		TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
		1.0f,
		"OriginalFloor",
		"StandardVertexShader",
		"StandardPixelShader"));
	std::shared_ptr<ITurboSceneObjectMaterial> ceilingMaterial = std::shared_ptr<ITurboSceneObjectMaterial>(new TurboSceneObjectMaterial(
		TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
		TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
		TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
		1.0f,
		"OriginalCeiling",
		"StandardVertexShader",
		"StandardPixelShader"));

	for (int w = 0; w <= size.w; w++)
	for (int h = 0; h <= size.h; h++)
	for (int d = 0; d <= size.d; d++)
	{
		BuildCorner(scene, w, h, d, cornerMaterial);

		if (w>0)	BuildWEdge(scene, w, h, d, edgeMaterial);
		if (h>0)	BuildHEdge(scene, w, h, d, edgeMaterial);
		if (d>0)	BuildDEdge(scene, w, h, d, edgeMaterial);
		
		char walls = mazeArray[w][h][d];
		if (GETBIT(walls, RIGHTBIT ) > 0)	BuildRightWall (scene, w, h, d, wallMaterial,  wallMaterial);
		if (GETBIT(walls, BOTTOMBIT) > 0)	BuildBottomWall(scene, w, h, d, floorMaterial, ceilingMaterial);
		if (GETBIT(walls, FRONTBIT ) > 0)	BuildFrontWall (scene, w, h, d, wallMaterial,  wallMaterial);
	}
}

void OriginalMazeSceneBuilder::BuildCorner(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeCornerObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildWEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildHEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildDEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildRightWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> leftMaterial, std::shared_ptr<ITurboSceneObjectMaterial> rightMaterial)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, leftMaterial, 1)));
	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, rightMaterial, 2)));
}

void OriginalMazeSceneBuilder::BuildBottomWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> topMaterial, std::shared_ptr<ITurboSceneObjectMaterial> bottomMaterial)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, topMaterial, 1)));
	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, bottomMaterial, 2)));
}

void OriginalMazeSceneBuilder::BuildFrontWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> backMaterial, std::shared_ptr<ITurboSceneObjectMaterial> frontMaterial)
{
	MazeObject mazeObject;

	mazeObject.Active = 1;
	mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, backMaterial, 1)));
	scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, frontMaterial, 2)));
}

//  Local Support Methods  ---------------------------------------------------------------------------------------------

