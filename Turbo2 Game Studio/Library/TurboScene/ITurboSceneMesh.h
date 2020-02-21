#pragma once

#include <pch.h>

#include <TurboVector2D.h>
#include <TurboVector3D.h>
#include <TurboSceneTypes.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneMesh
		{
		public:
			//  ITurboSceneMesh Methods --------------------------------------------------------------------------------
			virtual void AddVertex(TurboVector3D position, TurboVector3D normal, TurboVector2D textureUV) = 0;
			virtual void AddTriangle(int vertex1, int vertex2, int vertex3) = 0;

			//  ITurboSceneMesh Properties -----------------------------------------------------------------------------
			virtual std::vector<TurboSceneVertex> Vertices() = 0;
			virtual std::vector<TurboSceneTriangle> Triangles() = 0;

			virtual TurboVector3D MinExtent() = 0;
			virtual TurboVector3D MaxExtent() = 0;
		};
	}
}