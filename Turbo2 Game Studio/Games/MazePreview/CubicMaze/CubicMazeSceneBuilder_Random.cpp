
#include <pch.h>

#include <time.h>

#include <TurboSceneMaterial.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneBuilder_Random.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneBuilder_Random::CubicMazeSceneBuilder_Random()
{
    _baseSceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryCorner")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryEdge")),

            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryFloor")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryCeiling")),

            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryEntrance")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryLocked")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),

            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryExit")),
            RandomMaterial(),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryLocked")),
            RandomMaterial()    //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall"))
    ));
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> CubicMazeSceneBuilder_Random::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
    return _baseSceneBuilder->BuildScene(cubicMaze);
}

//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

std::shared_ptr<ITurboSceneMaterial> CubicMazeSceneBuilder_Random::RandomMaterial()
{
	TurboColor diffuse = TurboColor(
		0.25f + rand() % 1000 / 2000.0f,
		0.25f + rand() % 1000 / 2000.0f,
		0.25f + rand() % 1000 / 2000.0f);

	std::shared_ptr<ITurboSceneMaterial> material = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
		TurboColor(0.3f, 0.3f, 0.3f),	//	Ambient
		diffuse,						//	Diffuse
		TurboColor(1.0f, 1.0f, 1.0f),	//	Specular
		10.0f							//	Specular Exponent
		));

    return material;
}

//  Local Support Methods ----------------------------------------------------------------------------------------------

