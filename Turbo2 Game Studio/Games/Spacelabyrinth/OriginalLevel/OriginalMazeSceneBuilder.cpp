
#include "pch.h"

#include <time.h>

#include <CubicMaze.h>
#include <OriginalCubicMazeFactory.h>
#include <OriginalMazeCornerMesh.h>
#include <OriginalMazeEdgeMesh.h>
#include <OriginalMazeWallMesh.h>
#include <OriginalMazeSceneBuilder.h>
#include <OriginalMazeSceneObject.h>
#include <TurboScene.h>
#include <TurboSceneObject.h>
#include <TurboSceneMesh.h>
#include <TurboSceneMaterial.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

OriginalMazeSceneBuilder::OriginalMazeSceneBuilder()
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneBuilder Methods -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> OriginalMazeSceneBuilder::BuildScene()
{
	std::unique_ptr<ICubicMazeFactory> factory = std::unique_ptr<ICubicMazeFactory>(new OriginalCubicMazeFactory());
	std::shared_ptr<CubicMaze> maze = factory->MakeMaze(3, 3, 3);

	std::shared_ptr<ITurboScene> scene = std::shared_ptr<ITurboScene>(new TurboScene());
	BuildSceneObjects(scene, maze);
	factory->FreeMaze(maze);

	return scene;
}

//  ITurboSceneBuilder Methods -----------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

void OriginalMazeSceneBuilder::BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> maze)
{
	location size = maze->GetSize();
	Array3D mazeArray = maze->GetMazeArray();

	std::shared_ptr<ITurboSceneMesh> cornerMesh = std::shared_ptr<ITurboSceneMesh>(new OriginalMazeCornerMesh());
	std::shared_ptr<ITurboSceneMesh> edgeMesh = std::shared_ptr<ITurboSceneMesh>(new OriginalMazeEdgeMesh());
	std::shared_ptr<ITurboSceneMesh> wallMesh = std::shared_ptr<ITurboSceneMesh>(new OriginalMazeWallMesh());

	std::shared_ptr<ITurboSceneMaterial> cornerMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalCorner",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> edgeMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalEdge",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> wallMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalWall",
			"StandardVertexShader",
			"StandardPixelShader"));

	std::shared_ptr<ITurboSceneMaterial> leftWallMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalLeftWall",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> rightWallMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalRightWall",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> backWallMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalBackWall",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> frontWallMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalFrontWall",
			"StandardVertexShader",
			"StandardPixelShader"));

	leftWallMaterial = wallMaterial;
	rightWallMaterial = wallMaterial;
	backWallMaterial = wallMaterial;
	frontWallMaterial = wallMaterial;

	std::shared_ptr<ITurboSceneMaterial> floorMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"OriginalFloor",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> ceilingMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
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
		BuildCorner(scene, w, h, d, cornerMesh, cornerMaterial);

		if (w>0)	BuildWEdge(scene, w, h, d, edgeMesh, edgeMaterial);
		if (h>0)	BuildHEdge(scene, w, h, d, edgeMesh, edgeMaterial);
		if (d>0)	BuildDEdge(scene, w, h, d, edgeMesh, edgeMaterial);

		MazeWalls walls = mazeArray[w][h][d];
		if (walls.right)	BuildRightWall (scene,	w, h, d,	wallMesh,	leftWallMaterial,	rightWallMaterial);
		if (walls.bottom)	BuildBottomWall(scene,	w, h, d,	wallMesh,	floorMaterial,		ceilingMaterial);
		if (walls.front)	BuildFrontWall (scene,	w, h, d,	wallMesh,	backWallMaterial,	frontWallMaterial);
	}
}

void OriginalMazeSceneBuilder::BuildCorner(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, material));

	sceneObject->Placement()->Move(
		+w * CELLSIZE + CELLHALF,
		-h * CELLSIZE - CELLHALF,
		-d * CELLSIZE - CELLHALF);

	scene->AddSceneObject(sceneObject);
}

void OriginalMazeSceneBuilder::BuildWEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE - CELLHALF,
		-d * CELLSIZE - CELLHALF);

	scene->AddSceneObject(sceneObject);

	//MazeObject mazeObject;

	//mazeObject.Active = 1;
	//mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	left
	//mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	right
	//mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	//mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	//mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	//mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildHEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, 90.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE + CELLHALF,
		-h * CELLSIZE,
		-d * CELLSIZE - CELLHALF);

	scene->AddSceneObject(sceneObject);

	//MazeObject mazeObject;

	//mazeObject.Active = 1;
	//mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	//mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	//mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	top
	//mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	bottom
	//mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	back
	//mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	front

	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildDEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 90.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE + CELLHALF,
		-h * CELLSIZE - CELLHALF,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);

	//MazeObject mazeObject;

	//mazeObject.Active = 1;
	//mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	left
	//mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	top
	//mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	//	back
	//mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	right
	//mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	//	bottom
	//mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	//	front

	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeEdgeObject(mazeObject, material)));
}

void OriginalMazeSceneBuilder::BuildRightWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> leftMaterial, std::shared_ptr<ITurboSceneMaterial> rightMaterial)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, leftMaterial));

	sceneObject->Placement()->Rotate(0.0, 0.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE + CELLHALF,
		-h * CELLSIZE,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);

	
	sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, rightMaterial));

	sceneObject->Placement()->Rotate(0.0, 180.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE + CELLHALF,
		-h * CELLSIZE,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);


	//MazeObject mazeObject;

	//mazeObject.Active = 1;
	//mazeObject.Left   = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	1 * 2.0 - 0.0 + (1.0 - 0.1) =  2.9
	//mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF + WALLHALF);	//	1 * 2.0 - 0.0 + (1.0 + 0.1) =  3.1
	//mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 + (1.0 - 0.1) = -1.1
	//mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 - 0.1) = -2.9
	//mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 + (1.0 - 0.1) = -1.1
	//mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 - 0.1) = -2.9

	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, leftMaterial, 1)));
	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, rightMaterial, 2)));
}

void OriginalMazeSceneBuilder::BuildBottomWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> topMaterial, std::shared_ptr<ITurboSceneMaterial> bottomMaterial)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, topMaterial));

	sceneObject->Placement()->Rotate(0.0, 0.0, -90.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE - CELLHALF,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);


	sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, bottomMaterial));

	sceneObject->Placement()->Rotate(0.0, 0.0, 90.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE - CELLHALF,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);

	//MazeObject mazeObject;

	//mazeObject.Active = 1;
	//mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	1 * 2.0 - 0.0 - (1.0 - 0.1) =  1.1
	//mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	1 * 2.0 - 0.0 + (1.0 - 0.1) =  2.9
	//mazeObject.Top    = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 - 0.1) = -2.9
	//mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 + 0.1) = -3.1
	//mazeObject.Back   = d * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 + (1.0 - 0.1) = -1.1
	//mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 - 0.1) = -2.9

	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, topMaterial, 1)));
	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, bottomMaterial, 2)));
}

void OriginalMazeSceneBuilder::BuildFrontWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> backMaterial, std::shared_ptr<ITurboSceneMaterial> frontMaterial)
{
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, backMaterial));

	sceneObject->Placement()->Rotate(0.0, 90.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE,
		-d * CELLSIZE - CELLHALF);

	scene->AddSceneObject(sceneObject);


	sceneObject = std::shared_ptr<ITurboSceneObject>(new OriginalMazeSceneObject(mesh, frontMaterial));

	sceneObject->Placement()->Rotate(0.0, -90.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE,
		-d * CELLSIZE - CELLHALF);

	scene->AddSceneObject(sceneObject);

	//MazeObject mazeObject;

	//mazeObject.Active = 1;
	//mazeObject.Left   = w *  CELLSIZE - OFFSET - (CELLHALF - WALLHALF);	//	1 * 2.0 - 0.0 - (1.0 - 0.1) =  1.1
	//mazeObject.Right  = w *  CELLSIZE - OFFSET + (CELLHALF - WALLHALF);	//	1 * 2.0 - 0.0 + (1.0 - 0.1) =  2.9
	//mazeObject.Top    = h * -CELLSIZE + OFFSET + (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 + (1.0 - 0.1) = -1.1
	//mazeObject.Bottom = h * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 - 0.1) = -2.9
	//mazeObject.Back   = d * -CELLSIZE + OFFSET - (CELLHALF - WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 - 0.1) = -2.9
	//mazeObject.Front  = d * -CELLSIZE + OFFSET - (CELLHALF + WALLHALF);	// -1 * 2.0 + 0.0 - (1.0 + 0.1) = -3.1
	//																			  
	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, backMaterial, 1)));
	//scene->AddSceneObject(std::shared_ptr<ITurboSceneObject>(new OriginalMazeWallObject(mazeObject, frontMaterial, 2)));
}																				 
																				 
//  Local Support Methods ----------------------------------------------------------------------------------------------

