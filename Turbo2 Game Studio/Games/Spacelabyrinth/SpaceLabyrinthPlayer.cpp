
#include "pch.h"

#include <ITurboSceneObject.h>
#include <ITurboSceneObjectPlacement.h>
#include <TurboSceneObjectPlacement.h>

#include <SpaceLabyrinthPlayer.h>

SpaceLabyrinthPlayer::SpaceLabyrinthPlayer()
{
	_placement = std::shared_ptr<ITurboSceneObjectPlacement>(new TurboSceneObjectPlacement());
}

void SpaceLabyrinthPlayer::Update(NavigationInfo navInfo)
{
	const double cMoveAccelleration = 2.0f;
	const double cRotateAccelleration = 45.0f;
	const double cFrictionFactor = 2.0f;
	const double cHoverFrequency = 2.0f;
	const double cHoverMagnitude = 0.05f;
	const double cGravityFactor = 0.0f; // 9.8f;
	const double cSelfRightingSpeed = 0.0f; // 30.0f;

	double deltaTime = navInfo.DeltaTime;
	double time = navInfo.Time;

	double moveSpeed = cMoveAccelleration * deltaTime;

	Vector3D velocity = _placement->Velocity();

	if (!(navInfo.MoveLeft ||
		navInfo.MoveRight ||
		navInfo.MoveDown ||
		navInfo.MoveUp ||
		navInfo.MoveFore ||
		navInfo.MoveBack))
	{
		//  air friction decay
		velocity -= velocity * cFrictionFactor * deltaTime;

		//  hover
		velocity += _placement->Up() * cos(time * cHoverFrequency) * cHoverMagnitude * deltaTime;

		//  gravity
		velocity.Y -= deltaTime * cGravityFactor;
	}


	if (navInfo.MoveLeft)	velocity -= _placement->Right() * moveSpeed;
	if (navInfo.MoveRight)	velocity += _placement->Right() * moveSpeed;
	if (navInfo.MoveDown)	velocity -= _placement->Up()    * moveSpeed;
	if (navInfo.MoveUp)		velocity += _placement->Up()    * moveSpeed;
	if (navInfo.MoveFore)	velocity -= _placement->Back()  * moveSpeed;
	if (navInfo.MoveBack)	velocity += _placement->Back()  * moveSpeed;

	_placement->Velocity(velocity);

	double rotateSpeed = cRotateAccelleration * deltaTime;

	Vector3D angularVelocity = _placement->AngularVelocity();

	if (!(navInfo.Pointer ||
		navInfo.PitchFore ||
		navInfo.PitchBack ||
		navInfo.YawRight ||
		navInfo.YawLeft ||
		navInfo.RollLeft ||
		navInfo.RollRight))
	{
		angularVelocity -= angularVelocity * 1.0f * deltaTime;

		//  self-righting
		angularVelocity.Z -= _placement->Right().Y * cSelfRightingSpeed * deltaTime;
		angularVelocity.X -= _placement->Back().Y * cSelfRightingSpeed * deltaTime;
	}

	if (navInfo.Pointer && _lastNavInfo.Pointer)
	{
		double dx = navInfo.PointerX - _lastNavInfo.PointerX;
		double dy = navInfo.PointerY - _lastNavInfo.PointerY;

		angularVelocity.X = dy / deltaTime;
		angularVelocity.Y = -dx / deltaTime;
	}

	_lastNavInfo = navInfo;


	if (navInfo.PitchFore)	angularVelocity.X -= rotateSpeed;
	if (navInfo.PitchBack)	angularVelocity.X += rotateSpeed;
	if (navInfo.YawRight)	angularVelocity.Y -= rotateSpeed;
	if (navInfo.YawLeft)	angularVelocity.Y += rotateSpeed;
	if (navInfo.RollLeft)	angularVelocity.Z -= rotateSpeed;
	if (navInfo.RollRight)	angularVelocity.Z += rotateSpeed;

	_placement->AngularVelocity(angularVelocity);
}
