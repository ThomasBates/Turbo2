
#include "pch.h"

#include <ITurboSceneObject.h>
#include <ITurboSceneObjectPlacement.h>
#include <TurboSceneObjectPlacement.h>

#include <SpaceLabyrinthPlayer.h>

SpaceLabyrinthPlayer::SpaceLabyrinthPlayer(std::shared_ptr<ITurboApplicationPlatform> platform)
{
	_platform = platform;
	_placement = std::shared_ptr<ITurboSceneObjectPlacement>(new TurboSceneObjectPlacement());
}

void SpaceLabyrinthPlayer::Navigate()
{
	const float cMoveAccelleration = 2.0f;
	const float cRotateAccelleration = 45.0f;
	const float cFrictionFactor = 2.0f;
	const float cHoverFrequency = 2.0f;
	const float cHoverMagnitude = 0.05f;
	const float cGravityFactor = 0.0f; // 9.8f;
	const float cSelfRightingSpeed = 0.0f; // 30.0f;

	NavigationInfo navInfo = _platform->GetNavigationInfo();

	float deltaTime = navInfo.DeltaTime;
	float time = navInfo.Time;

	float moveSpeed = cMoveAccelleration * deltaTime;

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

	float rotateSpeed = cRotateAccelleration * deltaTime;

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
		int dx = navInfo.PointerX - _lastNavInfo.PointerX;
		int dy = navInfo.PointerY - _lastNavInfo.PointerY;

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

void SpaceLabyrinthPlayer::Update()
{
	_platform->CameraPlacement(_placement);
}

void SpaceLabyrinthPlayer::Render()
{
	//_platform->RenderSceneObject(std::shared_ptr<ITurboSceneObject>(this));
}
