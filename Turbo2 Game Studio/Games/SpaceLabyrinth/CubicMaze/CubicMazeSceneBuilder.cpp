
#include <pch.h>

#include <time.h>

#include <CubicMaze.h>
#include <CubicMazeFactory.h>
#include <CubicMazeCornerMesh.h>
#include <CubicMazeEdgeMesh.h>
#include <CubicMazeWallMesh.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneObject.h>
#include <TurboScene.h>
#include <TurboSceneObject.h>
#include <TurboSceneMesh.h>
#include <TurboSceneMaterial.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneBuilder::CubicMazeSceneBuilder()
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneBuilder Methods -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> CubicMazeSceneBuilder::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
	std::shared_ptr<ITurboScene> scene = std::shared_ptr<ITurboScene>(new TurboScene());

	BuildSceneObjects(scene, cubicMaze);

	return scene;
}

//  ITurboSceneBuilder Methods -----------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

void CubicMazeSceneBuilder::BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> cubicMaze)
{
	CubicMazeLocation size = cubicMaze->Size();
	//CubicMazeCellArray3D mazeArray = cubicMaze->MazeArray();

	std::shared_ptr<ITurboSceneMesh> cornerMesh = std::shared_ptr<ITurboSceneMesh>(new CubicMazeCornerMesh());
	std::shared_ptr<ITurboSceneMesh> edgeMesh = std::shared_ptr<ITurboSceneMesh>(new CubicMazeEdgeMesh());
	std::shared_ptr<ITurboSceneMesh> wallMesh = std::shared_ptr<ITurboSceneMesh>(new CubicMazeWallMesh());

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
	std::shared_ptr<ITurboSceneMaterial> entranceMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"IntroEntrance",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> exitMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"IntroWall",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> lockedExitMaterial = 
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
			TurboColor(0.5f, 0.5f, 0.5f, 1.0f),
			TurboColor(0.8f, 0.8f, 0.8f, 1.0f),
			TurboColor(0.3f, 0.3f, 0.3f, 1.0f),
			1.0f,
			"IntroEdge",
			"StandardVertexShader",
			"StandardPixelShader"));
	std::shared_ptr<ITurboSceneMaterial> lockedEntranceMaterial = lockedExitMaterial;
	std::shared_ptr<ITurboSceneMaterial> entranceBackMaterial = lockedExitMaterial;
	std::shared_ptr<ITurboSceneMaterial> exitBackMaterial = lockedExitMaterial;

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

	std::shared_ptr<ITurboSceneMaterial> wallMaterials[CellWallTypeCount] =
	{
		nullptr,
		wallMaterial,
		nullptr,
		entranceMaterial,
		lockedEntranceMaterial,
		entranceBackMaterial,
		exitMaterial,
		lockedExitMaterial,
		exitBackMaterial
	};

	std::shared_ptr<ITurboSceneMaterial> floorMaterials[CellWallTypeCount] =
	{
		nullptr,
		floorMaterial,
		nullptr,
		entranceMaterial,
		lockedEntranceMaterial,
		entranceBackMaterial,
		exitMaterial,
		lockedExitMaterial,
		exitBackMaterial
	};

	std::shared_ptr<ITurboSceneMaterial> ceilingMaterials[CellWallTypeCount] =
	{
		nullptr,
		ceilingMaterial,
		nullptr,
		entranceMaterial,
		lockedEntranceMaterial,
		entranceBackMaterial,
		exitMaterial,
		lockedExitMaterial,
		exitBackMaterial
	};


	for (int w = 0; w <= size.W; w++)
	for (int h = 0; h <= size.H; h++)
	for (int d = 0; d <= size.D; d++)
	{
		BuildCorner(cubicMaze, scene, w, h, d, cornerMesh, cornerMaterial);

		BuildWEdge(cubicMaze, scene, w, h, d, edgeMesh, edgeMaterial);
		BuildHEdge(cubicMaze, scene, w, h, d, edgeMesh, edgeMaterial);
		BuildDEdge(cubicMaze, scene, w, h, d, edgeMesh, edgeMaterial);

		if ((w < size.W) && (h < size.H) && (d < size.D))
		{
			CubicMazeCell *cell = cubicMaze->Cell(w, h, d);

			BuildRightWall(	scene, w, h, d, wallMesh, wallMaterials[(int)cell->RightWall.Type]);
			BuildLeftWall(	scene, w, h, d, wallMesh, wallMaterials[(int)cell->LeftWall.Type]);
			BuildTopWall(	scene, w, h, d, wallMesh, ceilingMaterials[(int)cell->TopWall.Type]);
			BuildBottomWall(scene, w, h, d, wallMesh, floorMaterials[(int)cell->BottomWall.Type]);
			BuildBackWall(	scene, w, h, d, wallMesh, wallMaterials[(int)cell->BackWall.Type]);
			BuildFrontWall(	scene, w, h, d, wallMesh, wallMaterials[(int)cell->FrontWall.Type]);
		}
	}
}

void CubicMazeSceneBuilder::BuildCorner(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	//	Only put a corner where there is a wall that needs one.
	if (!(CellHasLeftWall(cubicMaze, w, h, d) ||
		CellHasLeftWall(cubicMaze, w, h - 1, d) ||
		CellHasLeftWall(cubicMaze, w, h, d - 1) ||
		CellHasLeftWall(cubicMaze, w, h - 1, d - 1) ||
		CellHasRightWall(cubicMaze, w - 1, h, d) ||
		CellHasRightWall(cubicMaze, w - 1, h - 1, d) ||
		CellHasRightWall(cubicMaze, w - 1, h, d - 1) ||
		CellHasRightWall(cubicMaze, w - 1, h - 1, d - 1) ||
		CellHasTopWall(cubicMaze, w, h, d) ||
		CellHasTopWall(cubicMaze, w - 1, h, d) ||
		CellHasTopWall(cubicMaze, w, h, d - 1) ||
		CellHasTopWall(cubicMaze, w - 1, h, d - 1) ||
		CellHasBottomWall(cubicMaze, w, h - 1, d) ||
		CellHasBottomWall(cubicMaze, w - 1, h - 1, d) ||
		CellHasBottomWall(cubicMaze, w, h - 1, d - 1) ||
		CellHasBottomWall(cubicMaze, w - 1, h - 1, d - 1) ||
		CellHasBackWall(cubicMaze, w, h, d) ||
		CellHasBackWall(cubicMaze, w - 1, h, d) ||
		CellHasBackWall(cubicMaze, w, h - 1, d) ||
		CellHasBackWall(cubicMaze, w - 1, h - 1, d) ||
		CellHasFrontWall(cubicMaze, w, h, d - 1) ||
		CellHasFrontWall(cubicMaze, w - 1, h, d - 1) ||
		CellHasFrontWall(cubicMaze, w, h - 1, d - 1) ||
		CellHasFrontWall(cubicMaze, w - 1, h - 1, d - 1)))
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Move(
		+w * CELLSIZE - CELLHALF,
		-h * CELLSIZE + CELLHALF,
		-d * CELLSIZE + CELLHALF);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildWEdge(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	//	Only put an edge where there is a wall that needs one.
	if (!(CellHasTopWall(cubicMaze, w, h, d) ||
		CellHasTopWall(cubicMaze, w, h, d - 1) ||
		CellHasBottomWall(cubicMaze, w, h - 1, d) ||
		CellHasBottomWall(cubicMaze, w, h - 1, d - 1) ||
		CellHasBackWall(cubicMaze, w, h, d) ||
		CellHasBackWall(cubicMaze, w, h - 1, d) ||
		CellHasFrontWall(cubicMaze, w, h, d - 1) ||
		CellHasFrontWall(cubicMaze, w, h - 1, d - 1)))
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE + CELLHALF,
		-d * CELLSIZE + CELLHALF);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildHEdge(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	//	Only put an edge where there is a wall that needs one.
	if (!(CellHasLeftWall(cubicMaze, w, h, d) ||
		CellHasLeftWall(cubicMaze, w, h, d - 1) ||
		CellHasRightWall(cubicMaze, w - 1, h, d) ||
		CellHasRightWall(cubicMaze, w - 1, h, d - 1) ||
		CellHasBackWall(cubicMaze, w, h, d) ||
		CellHasBackWall(cubicMaze, w - 1, h, d) ||
		CellHasFrontWall(cubicMaze, w, h, d - 1) ||
		CellHasFrontWall(cubicMaze, w - 1, h, d - 1)))
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, 90.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE - CELLHALF,
		-h * CELLSIZE,
		-d * CELLSIZE + CELLHALF);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildDEdge(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	//	Only put an edge where there is a wall that needs one.
	if (!(CellHasLeftWall(cubicMaze, w, h, d) ||
		CellHasLeftWall(cubicMaze, w, h - 1, d) ||
		CellHasRightWall(cubicMaze, w - 1, h, d) ||
		CellHasRightWall(cubicMaze, w - 1, h - 1, d) ||
		CellHasTopWall(cubicMaze, w, h, d) ||
		CellHasTopWall(cubicMaze, w - 1, h, d) ||
		CellHasBottomWall(cubicMaze, w, h - 1, d) ||
		CellHasBottomWall(cubicMaze, w - 1, h - 1, d)))
	{
		return;
	}
	
	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 90.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE - CELLHALF,
		-h * CELLSIZE + CELLHALF,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildLeftWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 180.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE - CELLHALF,
		-h * CELLSIZE,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildRightWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE + CELLHALF,
		-h * CELLSIZE,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildTopWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, 90.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE + CELLHALF,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildBottomWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 0.0, -90.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE - CELLHALF,
		-d * CELLSIZE);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildBackWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, -90.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE,
		-d * CELLSIZE + CELLHALF);

	scene->AddSceneObject(sceneObject);
}

void CubicMazeSceneBuilder::BuildFrontWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	if (material == nullptr)
	{
		return;
	}

	std::shared_ptr<ITurboSceneObject> sceneObject = std::shared_ptr<ITurboSceneObject>(new CubicMazeSceneObject(mesh, material));

	sceneObject->Placement()->Rotate(0.0, 90.0, 0.0);

	sceneObject->Placement()->Move(
		+w * CELLSIZE,
		-h * CELLSIZE,
		-d * CELLSIZE - CELLHALF);

	scene->AddSceneObject(sceneObject);
}

bool CubicMazeSceneBuilder::CellHasLeftWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d)
{
	CubicMazeLocation size = cubicMaze->Size();

	if ((w < 0) || (w >= size.W) ||
		(h < 0) || (h >= size.H) ||
		(d < 0) || (d >= size.D))
	{
		return false;
	}

	CubicMazeCell *pCell = cubicMaze->Cell(w, h, d);

	if ((pCell->LeftWall.Type == None) ||
		(pCell->LeftWall.Type == InvisibleWall))
	{
		return false;
	}

	return true;
}

bool CubicMazeSceneBuilder::CellHasRightWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d)
{
	CubicMazeLocation size = cubicMaze->Size();

	if ((w < 0) || (w >= size.W) ||
		(h < 0) || (h >= size.H) ||
		(d < 0) || (d >= size.D))
	{
		return false;
	}

	CubicMazeCell *pCell = cubicMaze->Cell(w, h, d);

	if ((pCell->RightWall.Type == None) ||
		(pCell->RightWall.Type == InvisibleWall))
	{
		return false;
	}

	return true;
}

bool CubicMazeSceneBuilder::CellHasTopWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d)
{
	CubicMazeLocation size = cubicMaze->Size();

	if ((w < 0) || (w >= size.W) ||
		(h < 0) || (h >= size.H) ||
		(d < 0) || (d >= size.D))
	{
		return false;
	}

	CubicMazeCell *pCell = cubicMaze->Cell(w, h, d);

	if ((pCell->TopWall.Type == None) ||
		(pCell->TopWall.Type == InvisibleWall))
	{
		return false;
	}

	return true;
}

bool CubicMazeSceneBuilder::CellHasBottomWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d)
{
	CubicMazeLocation size = cubicMaze->Size();

	if ((w < 0) || (w >= size.W) ||
		(h < 0) || (h >= size.H) ||
		(d < 0) || (d >= size.D))
	{
		return false;
	}

	CubicMazeCell *pCell = cubicMaze->Cell(w, h, d);

	if ((pCell->BottomWall.Type == None) ||
		(pCell->BottomWall.Type == InvisibleWall))
	{
		return false;
	}

	return true;
}

bool CubicMazeSceneBuilder::CellHasFrontWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d)
{
	CubicMazeLocation size = cubicMaze->Size();

	if ((w < 0) || (w >= size.W) ||
		(h < 0) || (h >= size.H) ||
		(d < 0) || (d >= size.D))
	{
		return false;
	}

	CubicMazeCell *pCell = cubicMaze->Cell(w, h, d);

	if ((pCell->FrontWall.Type == None) ||
		(pCell->FrontWall.Type == InvisibleWall))
	{
		return false;
	}

	return true;
}

bool CubicMazeSceneBuilder::CellHasBackWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d)
{
	CubicMazeLocation size = cubicMaze->Size();

	if ((w < 0) || (w >= size.W) ||
		(h < 0) || (h >= size.H) ||
		(d < 0) || (d >= size.D))
	{
		return false;
	}

	CubicMazeCell *pCell = cubicMaze->Cell(w, h, d);

	if ((pCell->BackWall.Type == None) ||
		(pCell->BackWall.Type == InvisibleWall))
	{
		return false;
	}

	return true;
}

//  Local Support Methods ----------------------------------------------------------------------------------------------

