#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <TurboSceneObject.h>

class OriginalMazeCornerObject : public TurboSceneObject
{
public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeCornerObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectMaterial> material);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------

};

