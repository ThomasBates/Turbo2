#pragma once

#include <pch.h>

#include <TurboSceneObject.h>

class OriginalMazeSceneObject : public TurboSceneObject
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	OriginalMazeSceneObject(std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);

	//  ITurboSceneObject Properties -----------------------------------------------------------------------------------

	//  ITurboSceneObject Methods --------------------------------------------------------------------------------------
	virtual bool IsTouching(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);

private:
	//  Local Support Methods ------------------------------------------------------------------------------------------
	bool IsTouchingLeftSideOfWall  (TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);
	bool IsTouchingRightSideOfWall (TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);
	bool IsTouchingTopSideOfWall   (TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);
	bool IsTouchingBottomSideOfWall(TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);
	bool IsTouchingBackSideOfWall  (TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);
	bool IsTouchingFrontSideOfWall (TurboVector3D oldPosition, TurboVector3D newPosition, double radius, TurboVector3D *pContact, TurboVector3D *pNormal);
};

