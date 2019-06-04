
#include <pch.h>

#include <time.h>

#include <TurboSceneMaterial.h>
#include <CubicMazeSceneBuilder.h>
#include <CubicMazeSceneBuilder_Flat.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneBuilder_Flat::CubicMazeSceneBuilder_Flat()
{
    _baseSceneBuilder = std::shared_ptr<ICubicMazeSceneBuilder>(new CubicMazeSceneBuilder(
            ColorMaterial(0.8, 0.2, 0.2),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryCorner")),
            ColorMaterial(0.9, 0.1, 0.1),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryEdge")),

            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),
            ColorMaterial(0.1, 0.9, 0.1),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryFloor")),
            ColorMaterial(0.1, 0.1, 0.9),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryCeiling")),

            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryEntrance")),
            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryLocked")),
            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall")),

            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryExit")),
            ColorMaterial(0.5, 0.5, 0.5),   //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryLocked")),
            ColorMaterial(0.5, 0.5, 0.5)    //  std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial("LaboratoryWall"))
    ));
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> CubicMazeSceneBuilder_Flat::BuildScene(std::shared_ptr<CubicMaze> cubicMaze)
{
    return _baseSceneBuilder->BuildScene(cubicMaze);
}

//  ICubicMazeSceneBuilder Methods -------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

std::shared_ptr<ITurboSceneMaterial> CubicMazeSceneBuilder_Flat::ColorMaterial(float r, float g, float b)
{
    std::shared_ptr<ITurboSceneMaterial> material = std::shared_ptr<ITurboSceneMaterial>(new TurboSceneMaterial(
		TurboColor(0.3f, 0.3f, 0.3f),	//	Ambient
		TurboColor(r, g, b),			//	Diffuse
		TurboColor(1.0f, 1.0f, 1.0f),	//	Specular
		10.0f							//	Specular Exponent
		));

    return material;
}

//  Local Support Methods ----------------------------------------------------------------------------------------------

