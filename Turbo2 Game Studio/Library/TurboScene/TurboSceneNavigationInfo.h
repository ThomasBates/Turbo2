#pragma once

#include <pch.h>

#include <ITurboSceneNavigationControl.h>

namespace Turbo
{
    namespace Scene
    {
        struct NavigationInfo {
            double	Time;
            double	DeltaTime;

            std::vector<std::shared_ptr<ITurboSceneNavigationControl>> Controls;

            bool	Start;
            bool	Select;
            bool	Terminate;
        };
    }
}