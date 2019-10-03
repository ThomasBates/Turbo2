#pragma once

#include <pch.h>

//#include <ITurboSceneNavigationControl.h>
#include "ITurboSceneNavigationControl.h"

namespace Turbo
{
    namespace Scene
    {
        struct NavigationInfo {
            double	Time;
            double	DeltaTime;
            bool	Restart;
            bool	Pause;
            bool	Terminate;
            std::vector<std::shared_ptr<ITurboSceneNavigationControl>> Controls;

            TurboVector3D Look {};
            TurboVector3D Move {};
//			bool	Pointer;
//			float	PointerX;
//			float	PointerY;
//			bool 	MovePadActive;
//			float 	MovePadX;
//			float 	MovePadY;
//			bool 	LookPadActive;
//			float 	LookPadX;
//			float 	LookPadY;
//			bool	MoveLeft;
//			bool	MoveRight;
//			bool	MoveDown;
//			bool	MoveUp;
//			bool	MoveFore;
//			bool	MoveBack;
//			bool	PitchFore;
//			bool	PitchBack;
//			bool	YawRight;
//			bool	YawLeft;
//			bool	RollLeft;
//			bool	RollRight;
        };
    }
}