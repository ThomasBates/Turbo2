
#include "pch.h"

#include "SpaceLabyrinthOriginalScene.h"
#include "SpaceLabyrinthPlayer.h"
#include "OriginalMazeSceneBuilder.h"

//  Constructors and Destructors  --------------------------------------------------------------------------------------

SpaceLabyrinthOriginalScene::SpaceLabyrinthOriginalScene(std::shared_ptr<ITurboApplicationPlatform> platform)
{
	_platform = platform;
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

int SpaceLabyrinthOriginalScene::Render()
{
	for (int i = 0; i < _sceneObjects.size(); i++)
	{
		std::shared_ptr<ITurboSceneObject> sceneObject = _sceneObjects[i];
		_platform->RenderSceneObject(sceneObject);
		//sceneObject->Render();
	}
	return 1;
}

//  ITurboScene Methods  -----------------------------------------------------------------------------------------------
//  Local Support Methods  ---------------------------------------------------------------------------------------------

//  Local Support Methods  ---------------------------------------------------------------------------------------------
