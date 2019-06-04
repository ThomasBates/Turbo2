
#pragma once

#include <CubicMaze.h>
#include <ICubicMazeSceneBuilder.h>

class CubicMazeSceneBuilder_Flat : public ICubicMazeSceneBuilder
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    CubicMazeSceneBuilder_Flat();
    virtual ~CubicMazeSceneBuilder_Flat(){}

    //  ICubicMazeSceneBuilder Methods ---------------------------------------------------------------------------------
    virtual std::shared_ptr<ITurboScene> BuildScene(std::shared_ptr<CubicMaze> cubicMaze);

private:
    std::shared_ptr<ICubicMazeSceneBuilder> _baseSceneBuilder;

    std::shared_ptr<ITurboSceneMaterial> ColorMaterial(float r, float g, float b);
};
