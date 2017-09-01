
#include <pch.h>

#include <time.h>

#include <TurboSceneMaterial.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneBuilder_Metal.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneBuilder_Metal::CubicMazeSceneBuilder_Metal()
{
	_baseSceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalCorner")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalEdge")),

		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalFloor")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalCeiling")),

		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalEntrance")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalLocked")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalWall")),

		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalExit")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalLocked")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("MetalWall"))
		));
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> CubicMazeSceneBuilder_Metal::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
	return _baseSceneBuilder->BuildScene(cubicMaze);
}

//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------

