
#pragma once

#include <CubicMaze.h>
#include <ICubicMazeSceneBuilder.h>

class CubicMazeSceneBuilder: public ICubicMazeSceneBuilder
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	CubicMazeSceneBuilder(
		std::shared_ptr<ITurboSceneMaterial> cornerMaterial,
		std::shared_ptr<ITurboSceneMaterial> edgeMaterial,

		std::shared_ptr<ITurboSceneMaterial> leftWallMaterial,
		std::shared_ptr<ITurboSceneMaterial> rightWallMaterial,
		std::shared_ptr<ITurboSceneMaterial> frontWallMaterial,
		std::shared_ptr<ITurboSceneMaterial> backWallMaterial,
		std::shared_ptr<ITurboSceneMaterial> floorMaterial,
		std::shared_ptr<ITurboSceneMaterial> ceilingMaterial,

		std::shared_ptr<ITurboSceneMaterial> entranceMaterial,
		std::shared_ptr<ITurboSceneMaterial> entranceLockedMaterial,
		std::shared_ptr<ITurboSceneMaterial> entranceBackMaterial,
		std::shared_ptr<ITurboSceneMaterial> exitMaterial,
		std::shared_ptr<ITurboSceneMaterial> exitLockedMaterial,
		std::shared_ptr<ITurboSceneMaterial> exitBackMaterial);

	//  ITurboSceneBuilder Methods -------------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);

private:

	std::shared_ptr<ITurboSceneMaterial> _cornerMaterial;
	std::shared_ptr<ITurboSceneMaterial> _edgeMaterial;

	std::shared_ptr<ITurboSceneMaterial> _leftWallMaterial;
	std::shared_ptr<ITurboSceneMaterial> _rightWallMaterial;
	std::shared_ptr<ITurboSceneMaterial> _frontWallMaterial;
	std::shared_ptr<ITurboSceneMaterial> _backWallMaterial;
	std::shared_ptr<ITurboSceneMaterial> _floorMaterial;
	std::shared_ptr<ITurboSceneMaterial> _ceilingMaterial;

	std::shared_ptr<ITurboSceneMaterial> _entranceMaterial;
	std::shared_ptr<ITurboSceneMaterial> _entranceLockedaterial;
	std::shared_ptr<ITurboSceneMaterial> _entranceBackMaterial;
	std::shared_ptr<ITurboSceneMaterial> _exitMaterial;
	std::shared_ptr<ITurboSceneMaterial> _exitLockedMaterial;
	std::shared_ptr<ITurboSceneMaterial> _exitBackMaterial;

	//	Local Support Methods ------------------------------------------------------------------------------------------
	void BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> cubicMaze);

	void BuildCorner(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	
	void BuildWEdge(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildHEdge(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildDEdge(std::shared_ptr<CubicMaze> cubicMaze, std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	
	void BuildLeftWall(  std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildRightWall( std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildTopWall(   std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildBottomWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildBackWall(  std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void BuildFrontWall( std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);

	bool CellHasLeftWall(  std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d);
	bool CellHasRightWall( std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d);
	bool CellHasTopWall(   std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d);
	bool CellHasBottomWall(std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d);
	bool CellHasBackWall(  std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d);
	bool CellHasFrontWall( std::shared_ptr<CubicMaze> cubicMaze, int w, int h, int d);
};
