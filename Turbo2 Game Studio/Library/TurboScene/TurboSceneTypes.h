
#pragma once

#include <TurboVector2D.h>
#include <TurboVector3D.h>

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
			TurboVector3D Color;
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