#pragma once

#include <pch.h>

#include <ITurboSceneMesh.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneMesh : public ITurboSceneMesh
		{
		private:
			std::vector<TurboSceneVertex>	_vertices;
			std::vector<TurboSceneTriangle>	_triangles;

			TurboVector3D _minExtent = TurboVector3D( 10000.0F,  10000.0F,  10000.0F);
			TurboVector3D _maxExtent = TurboVector3D(-10000.0F, -10000.0F, -10000.0F);

		public:
			//  Constructors and Destructors ---------------------------------------------------------------------------
			TurboSceneMesh() {}
			virtual ~TurboSceneMesh() {}

			//  ITurboSceneMesh Methods --------------------------------------------------------------------------------
			virtual void AddVertex(TurboVector3D position, TurboVector3D normal, TurboColor	color, TurboVector2D textureUV);
			virtual void AddVertex(TurboVector3D position, TurboVector3D normal, TurboColor	color);
			virtual void AddVertex(TurboVector3D position, TurboVector3D normal, TurboVector2D textureUV);
			virtual void AddTriangle(int vertex1, int vertex2, int vertex3);

			//  ITurboSceneMesh Properties -----------------------------------------------------------------------------
			virtual std::vector<TurboSceneVertex> Vertices() { return _vertices; };
			virtual std::vector<TurboSceneTriangle> Triangles() { return _triangles; };

			virtual TurboVector3D MinExtent() { return _minExtent; };
			virtual TurboVector3D MaxExtent() { return _maxExtent; };
		};
	}
}