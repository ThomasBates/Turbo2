
#include <pch.h>

#include <TurboSceneViewModel.h>

using namespace Turbo::Game;
using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TurboSceneViewModel::TurboSceneViewModel(std::shared_ptr<ITurboGameLevel> level) :
    _level(level)
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  Public Properties --------------------------------------------------------------------------------------------------

std::shared_ptr<ITurboScene> TurboSceneViewModel::Scene()
{
    return _level->Scene();
}

//  Public Properties --------------------------------------------------------------------------------------------------
