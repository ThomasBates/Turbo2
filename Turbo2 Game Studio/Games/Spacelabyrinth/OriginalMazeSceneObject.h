#pragma once

#include <pch.h>

#include <TurboSceneObject.h>

class OriginalMazeSceneObject : public TurboSceneObject
{
public:
	//  Constructors and Destructors  -------------------------------------------------------------------------------------
	OriginalMazeSceneObject(std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);

	//  ITurboSceneObject Properties  -------------------------------------------------------------------------------------

	//  ITurboSceneObject Methods  ----------------------------------------------------------------------------------------
	virtual bool IsTouching(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);

private:
	//  Local Support Methods  --------------------------------------------------------------------------------------------
	bool IsTouchingLeftSideOfWall  (Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);
	bool IsTouchingRightSideOfWall (Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);
	bool IsTouchingTopSideOfWall   (Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);
	bool IsTouchingBottomSideOfWall(Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);
	bool IsTouchingBackSideOfWall  (Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);
	bool IsTouchingFrontSideOfWall (Vector3D oldPosition, Vector3D newPosition, double radius, Vector3D *pContact, Vector3D *pNormal);
};

