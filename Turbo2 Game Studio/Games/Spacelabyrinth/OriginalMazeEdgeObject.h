#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <TurboSceneObject.h>

class OriginalMazeEdgeObject : public TurboSceneObject
{
public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeEdgeObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectMaterial> material);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------

};

