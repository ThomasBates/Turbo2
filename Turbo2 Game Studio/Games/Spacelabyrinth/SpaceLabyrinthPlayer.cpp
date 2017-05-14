
#include "pch.h"

#include <ITurboSceneObject.h>
#include <ITurboScenePlacement.h>
#include <TurboScenePlacement.h>

#include <SpaceLabyrinthPlayer.h>

SpaceLabyrinthPlayer::SpaceLabyrinthPlayer()
{
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
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

	//	If no movement inputs, slow down, hover, and fall (if enabled).
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

	//	Handle keyboard movement inputs.
	if (navInfo.MoveLeft)	velocity -= _placement->Right() * moveSpeed;
	if (navInfo.MoveRight)	velocity += _placement->Right() * moveSpeed;
	if (navInfo.MoveDown)	velocity -= _placement->Up()    * moveSpeed;
	if (navInfo.MoveUp)		velocity += _placement->Up()    * moveSpeed;
	if (navInfo.MoveFore)	velocity -= _placement->Back()  * moveSpeed;
	if (navInfo.MoveBack)	velocity += _placement->Back()  * moveSpeed;

	_placement->Velocity(velocity);

	double rotateSpeed = cRotateAccelleration * deltaTime;

	Vector3D angularVelocity = _placement->AngularVelocity();

	//	If no direction inputs, slow down the spinning and stand upright (if enabled).
	if (!(navInfo.Pointer ||
		navInfo.PitchFore ||
		navInfo.PitchBack ||
		navInfo.YawRight ||
		navInfo.YawLeft ||
		navInfo.RollRight ||
		navInfo.RollLeft))
	{
		angularVelocity -= angularVelocity * 1.0f * deltaTime;

		//  self-righting
		angularVelocity.Z -= _placement->Right().Y * cSelfRightingSpeed * deltaTime;
		angularVelocity.X -= _placement->Back().Y * cSelfRightingSpeed * deltaTime;
	}

	//	Handle mouse direction inputs
	if (navInfo.Pointer && _lastNavInfo.Pointer)
	{
		double dx = navInfo.PointerX - _lastNavInfo.PointerX;
		double dy = navInfo.PointerY - _lastNavInfo.PointerY;

		angularVelocity.X = -dy / deltaTime;
		angularVelocity.Y = -dx / deltaTime;
	}

	_lastNavInfo = navInfo;

	//	Handle keyboard direction inputs.
	if (navInfo.PitchFore)	angularVelocity.X -= rotateSpeed;
	if (navInfo.PitchBack)	angularVelocity.X += rotateSpeed;
	if (navInfo.YawRight)	angularVelocity.Y -= rotateSpeed;
	if (navInfo.YawLeft)	angularVelocity.Y += rotateSpeed;
	if (navInfo.RollRight)	angularVelocity.Z -= rotateSpeed;
	if (navInfo.RollLeft)	angularVelocity.Z += rotateSpeed;

	_placement->AngularVelocity(angularVelocity);
}
