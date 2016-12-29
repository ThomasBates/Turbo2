#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <TurboSceneObject.h>

class OriginalMazeEdgeObject : public TurboSceneObject
{
public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeEdgeObject(std::shared_ptr<ITurboSceneObjectMesh> mesh, MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------

};

