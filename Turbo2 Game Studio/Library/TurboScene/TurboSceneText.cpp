
#include <pch.h>

#include <TurboSceneText.h>

using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TurboSceneText::TurboSceneText()
{
}

TurboSceneText::TurboSceneText(std::shared_ptr<ITurboSceneFont> font, std::string text) :
	_font(font),
	_text(text)
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneText Methods --------------------------------------------------------------------------------------------

//  ITurboSceneText Methods --------------------------------------------------------------------------------------------
