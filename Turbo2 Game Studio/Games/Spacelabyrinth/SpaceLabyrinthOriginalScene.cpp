
#include "pch.h"

#include "SpaceLabyrinthOriginalScene.h"
#include "SpaceLabyrinthPlayer.h"
#include "OriginalMazeSceneBuilder.h"

//  Constructors and Destructors  --------------------------------------------------------------------------------------

SpaceLabyrinthOriginalScene::SpaceLabyrinthOriginalScene()
{
}

SpaceLabyrinthOriginalScene::~SpaceLabyrinthOriginalScene()
{
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboScene Properties  --------------------------------------------------------------------------------------------

//  ITurboScene Properties  --------------------------------------------------------------------------------------------
//  ITurboScene Methods  -----------------------------------------------------------------------------------------------

void SpaceLabyrinthOriginalScene::AddSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
	_sceneObjects.push_back(sceneObject);
}

//  ITurboScene Methods  -----------------------------------------------------------------------------------------------
//  Local Support Methods  ---------------------------------------------------------------------------------------------

//  Local Support Methods  ---------------------------------------------------------------------------------------------
