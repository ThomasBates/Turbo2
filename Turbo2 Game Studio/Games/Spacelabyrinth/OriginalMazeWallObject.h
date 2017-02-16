#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <TurboSceneObject.h>

class OriginalMazeWallObject: public TurboSceneObject
{
public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeWallObject(
		MazeObject mazeObject,
		std::shared_ptr<ITurboSceneObjectMaterial> material,
		int wallSide);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------
	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal);

private:
	bool	_rightLeft;
	bool	_topBottom;
	bool	_backFront;
	int		_wallSide;

	//  Local Support Methods  --------------------------------------------------------------------------------------------
	bool IsTouchingX(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal);
	bool IsTouchingY(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal);
	bool IsTouchingZ(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *contact, Vector3D *normal);
};

