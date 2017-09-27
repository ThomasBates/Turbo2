
#pragma once

#include <pch.h>
#include <CubicMazeMotionEffects_WithGravity.h>
#include <TurboGameMotionEffects.h>

using namespace Turbo::Game;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

CubicMazeMotionEffects_WithGravity::CubicMazeMotionEffects_WithGravity(std::shared_ptr<CubicMaze> maze) :
	_maze(maze)
{
	_baseMotionEffects = std::shared_ptr<ITurboGameMotionEffects>(new TurboGameMotionEffects(
		cMoveAccelleration,
		cRotateAccelleration,
		cFrictionFactor,
		cHoverFrequency,
		cHoverMagnitude,
		cGravityFactor,
		cSelfRightingSpeed));
}

//  ITurboGameState Properties -----------------------------------------------------------------------------------------

//  ITurboGameState Methods --------------------------------------------------------------------------------------------

void CubicMazeMotionEffects_WithGravity::ProcessMotionEffects(NavigationInfo navInfo, std::shared_ptr<ITurboSceneObject> sceneObject, bool isPlayer)
{
	_baseMotionEffects->ProcessMotionEffects(navInfo, sceneObject, isPlayer);

	//	See if we're in a cell with no bottom.
	TurboVector3D position = sceneObject->Placement()->Position();
	CubicMazeLocation location = CubicMazeLocation(
		(int)round(position.X / CELLSIZE),
		(int)round(-position.Y / CELLSIZE),
		(int)round(-position.Z / CELLSIZE));
	CubicMazeCell *cell = _maze->Cell(location);

	//	If there's no bottom, or the player is above the middle of the cell, allow gravity.
	//	Otherwise, stay in the middle of the cell.
	double middleY = round(position.Y * 0.5) * 2.0;
	if ((cell != NULL) && (cell->BottomWall.Type != CubicMazeCellWallType::None) && (position.Y <= middleY))
	{
		//  Limit motion to horizontal plane for this level
		TurboVector3D velocity = sceneObject->Placement()->Velocity();
		velocity.Y = 0;
		sceneObject->Placement()->Velocity(velocity);
	}
}

