
#include <pch.h>

#include <TurboSceneSprite.h>
#include <TurboScenePlacement.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

Turbo::Scene::TurboSceneSprite::TurboSceneSprite()
{
}

Turbo::Scene::TurboSceneSprite::TurboSceneSprite(std::shared_ptr<ITurboSceneTexture> texture)
{
	_texture = texture;
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneSprite Methods ------------------------------------------------------------------------------------------

//  ITurboSceneSprite Methods ------------------------------------------------------------------------------------------
