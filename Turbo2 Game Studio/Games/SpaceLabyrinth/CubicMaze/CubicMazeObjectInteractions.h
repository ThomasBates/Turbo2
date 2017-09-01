
#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <ICubicMazeObjectInteractions.h>
#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboSceneBuilder.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

struct InputArguments
{
	std::shared_ptr<CubicMaze> maze;
	std::shared_ptr<ITurboSceneObject> sceneObject;
	bool isPlayer;
	TurboVector3D fromPoint;
	TurboVector3D toPoint;
	CubicMazeLocation location;
	CubicMazeCellWall cellWall;
};

struct OutputArguments
{
	int portalIndex;
	bool isLeaving;
	bool isTouching;
	TurboVector3D contact;
	TurboVector3D normal;
};

class CubicMazeObjectInteractions : public ICubicMazeObjectInteractions
{
public:
	//	Constructors ---------------------------------------------------------------------------------------------------
	CubicMazeObjectInteractions(std::shared_ptr<ITurboDebug> debug) :
		_debug(debug)
	{
	}

	//	Properties -----------------------------------------------------------------------------------------------------


	//	Methods --------------------------------------------------------------------------------------------------------
	virtual void ProcessObjectInteractions(
		NavigationInfo navInfo, 
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		bool isPlayer,
		int *pPortalIndex);

private:
	void IsTouchingCellWall(	InputArguments in, OutputArguments *pOut);
	void IsTouchingRightWall(	InputArguments in, OutputArguments *pOut);
	void IsTouchingLeftWall(	InputArguments in, OutputArguments *pOut);
	void IsTouchingBottomWall(	InputArguments in, OutputArguments *pOut);
	void IsTouchingTopWall(		InputArguments in, OutputArguments *pOut);
	void IsTouchingFrontWall(	InputArguments in, OutputArguments *pOut);
	void IsTouchingBackWall(	InputArguments in, OutputArguments *pOut);

/*
	void IsTouchingCellWall(	CubicMazeLocation location, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal, std::shared_ptr<ITurboSceneObject> sceneObject);
	void IsTouchingRightWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingLeftWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingBottomWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingTopWall(		CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingFrontWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingBackWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, bool isPlayer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
*/

	std::shared_ptr<ITurboDebug>		_debug;

	const double	_hBuffer	= 0.25;
	const double	_vBuffer	= 0.25;
	const double	_bounciness	= 0.25;

};

