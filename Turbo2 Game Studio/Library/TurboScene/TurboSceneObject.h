#pragma once

#include <pch.h>

#include <ITurboSceneObject.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneObject : public ITurboSceneObject
		{
		public:
			//  Constructors and Destructors -----------------------------------------------------------------------------------
			TurboSceneObject();
			TurboSceneObject(std::shared_ptr<ITurboSceneMesh> mesh);

			//  ITurboSceneObject Properties -----------------------------------------------------------------------------------
			virtual SceneObjectPrimitiveType PrimitiveType() { return _primitiveType; }
			virtual void PrimitiveType(SceneObjectPrimitiveType primitiveType) { _primitiveType = primitiveType; }

			virtual	std::shared_ptr<ITurboSceneMesh> Mesh() { return _mesh; }
			virtual void Mesh(std::shared_ptr<ITurboSceneMesh> mesh) { _mesh = mesh; }

			virtual std::shared_ptr<ITurboSceneMaterial> Material() { return _material; }
			virtual void Material(std::shared_ptr<ITurboSceneMaterial> material) { _material = material; }

			virtual std::shared_ptr<ITurboSceneSoundEffect> HitSound() { return _hitSound; }
			virtual void HitSound(std::shared_ptr<ITurboSceneSoundEffect> hitSound) { _hitSound = hitSound; }

			virtual std::shared_ptr<ITurboSceneLight> Light() { return _light; }
			virtual void Light(std::shared_ptr<ITurboSceneLight> light) { _light = light; }

			virtual	std::shared_ptr<ITurboScenePlacement> Placement() { return _placement; }
			virtual void Placement(std::shared_ptr<ITurboScenePlacement> placement) { _placement = placement; }

			virtual std::vector<std::shared_ptr<ITurboSceneObject>> ChildSceneObjects() { return _childSceneObjects; }

			//  ITurboSceneObject Methods --------------------------------------------------------------------------------------
			virtual void Update(NavigationInfo navInfo) {}
			virtual bool IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *contact, TurboVector3D *normal);
			virtual void PlaySound(float volume);

		private:
			SceneObjectPrimitiveType						_primitiveType;
			std::shared_ptr<ITurboSceneMesh>				_mesh;
			std::shared_ptr<ITurboSceneMaterial>			_material;
			std::shared_ptr<ITurboSceneSoundEffect>			_hitSound;
			std::shared_ptr<ITurboSceneLight>				_light;
			std::shared_ptr<ITurboScenePlacement>			_placement;
			std::vector<std::shared_ptr<ITurboSceneObject>>	_childSceneObjects;
		};
	}
}
