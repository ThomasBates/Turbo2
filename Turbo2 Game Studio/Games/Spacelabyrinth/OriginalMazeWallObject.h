#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <TurboSceneObject.h>

class OriginalMazeWallObject: public TurboSceneObject
{
private:
	bool	_rightLeft;
	bool	_topBottom;
	bool	_backFront;

	bool IsTouchingX(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal);
	bool IsTouchingY(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal);
	bool IsTouchingZ(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal);

public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeWallObject(
		std::shared_ptr<ITurboSceneObjectMesh> mesh, 
		MazeObject mazeObject, 
		std::shared_ptr<ITurboSceneObjectTexture> texture1, 
		std::shared_ptr<ITurboSceneObjectTexture> texture2);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------
	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, float radius, Vector3D *contact, Vector3D *normal);

};

