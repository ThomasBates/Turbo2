
#pragma once

#include <TurboVector2D.h>
#include <TurboVector3D.h>
#include <TurboColor.h>

using namespace Turbo::Graphics;
using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		enum TurboGameControlType
		{
			Look,
			Move,
			Action
		};

		struct TurboSceneVertex
		{
			TurboVector3D Position;
			TurboVector3D Normal;
			TurboColor Color;
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

		enum SceneTextHorizontalAlignment
		{
			horizontalLeft,
            horizontalCenter,
            horizontalRight,
            horizontalStretched,
            horizontalJustified
		};

		enum SceneTextVerticalAlignment
		{
			verticalTop,
            verticalCenter,
            verticalBottom,
            verticalStretched,
            verticalJustified
		};
	}
}