
#include <pch.h>

#include <TurboSceneText.h>
#include <TurboScenePlacement.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

Turbo::Scene::TurboSceneText::TurboSceneText()
{
}

Turbo::Scene::TurboSceneText::TurboSceneText(std::shared_ptr<ITurboSceneFont> font, std::string text)
{
	_font = font;
	_text = text;
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneText Methods ------------------------------------------------------------------------------------------

//  ITurboSceneText Methods ------------------------------------------------------------------------------------------
