
#pragma once

#include <CubicMaze.h>
#include <ICubicMazeSceneBuilder.h>

class CubicMazeSceneBuilder_Random : public ICubicMazeSceneBuilder
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    CubicMazeSceneBuilder_Random();
    virtual ~CubicMazeSceneBuilder_Random(){}

    //  ICubicMazeSceneBuilder Methods ---------------------------------------------------------------------------------
    virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);

private:
    std::shared_ptr<ICubicMazeSceneBuilder> _baseSceneBuilder;

    std::shared_ptr<ITurboSceneMaterial> RandomMaterial();
};
