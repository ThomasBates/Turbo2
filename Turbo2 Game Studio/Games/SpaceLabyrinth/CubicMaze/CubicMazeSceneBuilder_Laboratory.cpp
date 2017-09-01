
#include <pch.h>

#include <time.h>

#include <TurboSceneMaterial.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneBuilder_Laboratory.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneBuilder_Laboratory::CubicMazeSceneBuilder_Laboratory()
{
	_baseSceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryCorner")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryEdge")),
		
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryFloor")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryCeiling")),
		
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryEntrance")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryLocked")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
	
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryExit")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryLocked")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall"))
	));
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> CubicMazeSceneBuilder_Laboratory::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
	return _baseSceneBuilder->BuildScene(cubicMaze);
}

//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------

