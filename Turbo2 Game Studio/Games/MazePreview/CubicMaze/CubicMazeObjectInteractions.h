
#pragma once

#include <pch.h>

#include <CubicMaze.h>
#include <ICubicMazeObjectInteractions.h>
#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboSceneBuilder.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Math;
using namespace Turbo::Scene;

struct InputArguments
{
	std::shared_ptr<CubicMaze> maze;
	std::shared_ptr<ITurboSceneObject> sceneObject;
	bool isPlayer;
	bool isNeighborCell;
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
	virtual ~CubicMazeObjectInteractions(){}

	//	Properties -----------------------------------------------------------------------------------------------------


	//	Methods --------------------------------------------------------------------------------------------------------
	virtual void ProcessKeyHazardInteractions(
		NavigationInfo* navInfo,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		std::vector<std::shared_ptr<ITurboSceneObject>>* keys,
		std::vector<std::shared_ptr<ITurboSceneObject>>* hazards);

	virtual void ProcessObjectInteractions(
		NavigationInfo* navInfo, 
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboSceneObject> sceneObject,
		bool isPlayer,
		int *pPortalIndex);

private:
	void IsTouchingCellWall(	InputArguments* input, OutputArguments* output);
	void IsTouchingRightWall(	InputArguments* input, OutputArguments* output);
	void IsTouchingLeftWall(	InputArguments* input, OutputArguments* output);
	void IsTouchingBottomWall(	InputArguments* input, OutputArguments* output);
	void IsTouchingTopWall(		InputArguments* input, OutputArguments* output);
	void IsTouchingFrontWall(	InputArguments* input, OutputArguments* output);
	void IsTouchingBackWall(	InputArguments* input, OutputArguments* output);

	bool IsTouchingLeftSide(	InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide);
	bool IsTouchingRightSide(	InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide);
	bool IsTouchingTopSide(		InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide);
	bool IsTouchingBottomSide(	InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide);
	bool IsTouchingBackSide(	InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide);
	bool IsTouchingFrontSide(	InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide);


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

	const double	_buffer		= 0.25;
//	const double	_bounciness	= 0.25;

};

