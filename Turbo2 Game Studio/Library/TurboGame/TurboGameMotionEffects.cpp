
#include <pch.h>

#include <TurboGameMotionEffects.h>

using namespace Turbo::Game;

//  Constructors & Destructors -----------------------------------------------------------------------------------------

//  ITurboGameState Properties -----------------------------------------------------------------------------------------

//  ITurboGameState Methods --------------------------------------------------------------------------------------------

void TurboGameMotionEffects::ProcessMotionEffects(NavigationInfo* navInfo, std::shared_ptr<ITurboSceneObject> sceneObject, bool isPlayer)
{

	double deltaTime = navInfo->DeltaTime;
	double time = navInfo->Time;

	//  double moveSpeed = _moveAccelleration * deltaTime;

	std::shared_ptr<ITurboScenePlacement> _placement = sceneObject->Placement();

	TurboVector3D velocity = _placement->Velocity();

	//	If no movement inputs, slow down, hover, and fall (if enabled).
	if (!(navInfo->MoveLeft ||
		navInfo->MoveRight ||
		navInfo->MoveDown ||
		navInfo->MoveUp ||
		navInfo->MoveFore ||
		navInfo->MoveBack))
	{
		//  air friction decay
		velocity -= velocity * _frictionFactor * deltaTime;

		//  hover
		velocity += _placement->Up() * cos(time * _hoverFrequency) * _hoverMagnitude * deltaTime;
	}

	//  gravity
	velocity.Y -= deltaTime * _gravityFactor;

/*
	//	Handle keyboard movement inputs.
	if (navInfo->MoveLeft)	velocity -= _placement->Right() * moveSpeed;
	if (navInfo->MoveRight)	velocity += _placement->Right() * moveSpeed;
	if (navInfo->MoveDown)	velocity -= _placement->Up()    * moveSpeed;
	if (navInfo->MoveUp)		velocity += _placement->Up()    * moveSpeed;
	if (navInfo->MoveFore)	velocity -= _placement->Back()  * moveSpeed;
	if (navInfo->MoveBack)	velocity += _placement->Back()  * moveSpeed;
*/
	_placement->Velocity(velocity);

	//  double rotateSpeed = _rotateAccelleration * deltaTime;

	TurboVector3D angularVelocity = _placement->AngularVelocity();

	//	If no direction inputs, slow down the spinning and stand upright (if enabled).
	if (!(navInfo->Pointer ||
		navInfo->PitchFore ||
		navInfo->PitchBack ||
		navInfo->YawRight ||
		navInfo->YawLeft ||
		navInfo->RollRight ||
		navInfo->RollLeft))
	{
		// Slow down spinning
		angularVelocity -= angularVelocity * 1.0f * deltaTime;

		//  self-righting
		angularVelocity.X = _placement->Back().Y * _selfRightingSpeed * deltaTime;
	}
	//	keeping this outside of the "if" keeps the player from turning on its side while looking around.
	angularVelocity.Z = -_placement->Right().Y * _selfRightingSpeed * deltaTime;

/*
	//	Handle mouse direction inputs
	if (navInfo->Pointer && _lastNavInfo.Pointer)
	{
		double dx = navInfo->PointerX - _lastNavInfo.PointerX;
		double dy = navInfo->PointerY - _lastNavInfo.PointerY;

		angularVelocity.X = -dy / deltaTime;
		angularVelocity.Y = -dx / deltaTime;
	}

	//	Handle keyboard direction inputs.
	if (navInfo->PitchFore)	angularVelocity.X -= rotateSpeed;
	if (navInfo->PitchBack)	angularVelocity.X += rotateSpeed;
	if (navInfo->YawRight)	angularVelocity.Y -= rotateSpeed;
	if (navInfo->YawLeft)	angularVelocity.Y += rotateSpeed;
	if (navInfo->RollRight)	angularVelocity.Z -= rotateSpeed;
	if (navInfo->RollLeft)	angularVelocity.Z += rotateSpeed;
*/

	_placement->AngularVelocity(angularVelocity);

	_lastNavInfo = *navInfo;
}
