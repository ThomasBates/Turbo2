
#pragma once

#include <TurboVector2D.h>
#include <TurboVector3D.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		struct NavigationInfo {
			double	Time;
			double	DeltaTime;
			bool	Pointer;
			float	PointerX;
			float	PointerY;
			bool 	MovePadActive;
			float 	MovePadX;
			float 	MovePadY;
			bool 	LookPadActive;
			float 	LookPadX;
			float 	LookPadY;
			bool	MoveLeft;
			bool	MoveRight;
			bool	MoveDown;
			bool	MoveUp;
			bool	MoveFore;
			bool	MoveBack;
			bool	PitchFore;
			bool	PitchBack;
			bool	YawRight;
			bool	YawLeft;
			bool	RollLeft;
			bool	RollRight;
			bool	Pause;
			bool	Restart;
			bool	Terminate;
		};

		struct TurboSceneVertex
		{
			TurboVector3D Position;
			TurboVector3D Normal;
			TurboVector2D TextureUV;
		};

		struct TurboSceneTriangle
		{
			unsigned short Vertex1;
			unsigned short Vertex2;
			unsigned short Vertex3;
		};

		enum SceneObjectPrimitiveType
		{
			None,
			Mesh,
			Model,
			Cube,
			Sphere
		};
	}
}