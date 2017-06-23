
#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboSceneBuilder.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class CubicMazeObjectInteractions
{
private:
	std::shared_ptr<ITurboDebug>		_debug;
	std::shared_ptr<CubicMaze>			_maze;
	std::shared_ptr<ITurboSceneObject>	_player;

public:
	//	Constructors ---------------------------------------------------------------------------------------------------
	CubicMazeObjectInteractions(
		std::shared_ptr<ITurboDebug> debug,
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboSceneObject> player) :
		_debug(debug),
		_maze(maze),
		_player(player)
	{
	}

	//	Properties -----------------------------------------------------------------------------------------------------


	//	Methods --------------------------------------------------------------------------------------------------------
	void ProcessObjectInteractions(NavigationInfo navInfo, int *pPortalIndex);

private:
	void IsTouchingCellWall(	CubicMazeLocation location, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingRightWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingLeftWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingBottomWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingTopWall(		CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingFrontWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
	void IsTouchingBackWall(	CubicMazeCellWall cellWall, TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, int *pPortalIndex, bool *pIsLeaving, bool *pIsTouching, TurboVector3D *pContact, TurboVector3D *pNormal);
};

