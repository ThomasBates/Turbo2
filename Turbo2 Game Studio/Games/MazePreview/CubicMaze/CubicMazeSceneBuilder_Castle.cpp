
#include <pch.h>

#include <time.h>

#include <TurboSceneMaterial.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneBuilder_Castle.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneBuilder_Castle::CubicMazeSceneBuilder_Castle()
{
	_baseSceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleCorner")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleEdge")),

		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleWall")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleFloor")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleCeiling")),

		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleEntrance")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleLocked")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleWall")),

		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleExit")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleLocked")),
		std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("CastleWall"))
		));
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> CubicMazeSceneBuilder_Castle::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
	return _baseSceneBuilder->BuildScene(cubicMaze);
}

//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

//  Local Support Methods ----------------------------------------------------------------------------------------------

