#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <TurboSceneObject.h>

class OriginalMazeCornerObject : public TurboSceneObject
{
public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeCornerObject(std::shared_ptr<ITurboSceneObjectMesh> mesh, MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------

};

