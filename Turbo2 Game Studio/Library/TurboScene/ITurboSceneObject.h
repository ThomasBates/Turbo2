#pragma once

#include <ITurboSceneMesh.h>
#include <ITurboSceneMaterial.h>
#include <ITurboScenePlacement.h>
#include <TurboSceneTypes.h>

namespace Turbo
{
	namespace Scene
	{
		class ITurboSceneObject
		{
		public:
			//  ITurboSceneObject Properties -----------------------------------------------------------------------------------
			virtual	std::shared_ptr<ITurboSceneMesh> Mesh() = 0;
			virtual void Mesh(std::shared_ptr<ITurboSceneMesh> mesh) = 0;

			virtual std::shared_ptr<ITurboSceneMaterial> Material() = 0;
			virtual void Material(std::shared_ptr<ITurboSceneMaterial> material) = 0;

			virtual	std::shared_ptr<ITurboScenePlacement> Placement() = 0;
			virtual void Placement(std::shared_ptr<ITurboScenePlacement> placement) = 0;

			virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() = 0;

			//  ITurboSceneObject Methods --------------------------------------------------------------------------------------
			virtual void Update(NavigationInfo navInfo) = 0;
			virtual bool IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *contact, TurboVector3D *normal) = 0;
		};
	}
}
