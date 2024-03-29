
#include <pch.h>

#include <ITurboSceneObject.h>
#include <ITurboScenePlacement.h>
#include <TurboScenePlacement.h>

//#include <TeapotPlayer.h>
#include "TeapotPlayer.h"

using namespace Turbo::Math;
using namespace Turbo::Scene;

//TeapotPlayer::TeapotPlayer(TeapotUserOptions* userOptions) :
//        _userOptions(userOptions)
//{
//    _placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
//}

TeapotPlayer::TeapotPlayer()
{
    _placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

void TeapotPlayer::Update(NavigationInfo* navInfo)
{
    const double cMoveAccelleration = 2.0f;
//    const double cRotateAccelleration = 45.0f;
//    const double cFrictionFactor = 2.0f;

//	const double cHoverFrequency = 2.0f;
//	const double cHoverMagnitude = 0.05f;
//	const double cGravityFactor = 0.0f;
//	const double cSelfRightingSpeed = 0.0f;

//    const double cHoverFrequency = 0.0f;
//    const double cHoverMagnitude = 0.0f;
//    const double cGravityFactor = 9.8f;
//    const double cSelfRightingSpeed = 3000.0f;

    double deltaTime = navInfo->DeltaTime;

    double moveSpeed = cMoveAccelleration * deltaTime;

    TurboVector3D velocity = _placement->Velocity();

/*
	//	If no movement inputs, slow down, hover, and fall (if enabled).
	if (!(navInfo->MoveLeft ||
		navInfo->MoveRight ||
		navInfo->MoveDown ||
		navInfo->MoveUp ||
		navInfo->MoveFore ||
		navInfo->MoveBack))
	{
		//  air friction decay
		velocity -= velocity * cFrictionFactor * deltaTime;

		//  hover
		velocity += _placement->Up() * cos(time * cHoverFrequency) * cHoverMagnitude * deltaTime;
	}

	//  gravity
	velocity.Y -= deltaTime * cGravityFactor;
*/

//    //	Handle keyboard movement inputs.
//    if (navInfo->MoveLeft)	velocity -= _placement->Right() * moveSpeed;
//    if (navInfo->MoveRight)	velocity += _placement->Right() * moveSpeed;
//    if (navInfo->MoveDown)	velocity -= _placement->Up()    * moveSpeed;
//    if (navInfo->MoveUp)	velocity += _placement->Up()    * moveSpeed;
//    if (navInfo->MoveFore)	velocity -= _placement->Back()  * moveSpeed;
//    if (navInfo->MoveBack)	velocity += _placement->Back()  * moveSpeed;

    for (auto& control: navInfo->Controls)
    {
        if ((control->Type() == TurboGameControlType::Move) &&
            (control->IsActive()))
        {
            velocity -= _placement->Back()  * moveSpeed * control->YValue();
            velocity += _placement->Right() * moveSpeed * control->XValue();
        }
    }

    _placement->Velocity(velocity);

//    double rotateSpeed = cRotateAccelleration * deltaTime;

    TurboVector3D angularVelocity = _placement->AngularVelocity();

    for (auto& control: navInfo->Controls)
    {
        if ((control->Type() == TurboGameControlType::Look) &&
            (control->IsActive()))
        {
            double dx = control->XValue();
            double dy = control->YValue();

            angularVelocity.X = dy / deltaTime;
            angularVelocity.Y = dx / deltaTime;
        }
    }

/*
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
		angularVelocity.X = _placement->Back().Y * cSelfRightingSpeed * deltaTime;
		angularVelocity.Z = -_placement->Right().Y * cSelfRightingSpeed * deltaTime;
	}
*/

    //	Handle mouse direction inputs
//    if (navInfo->Pointer && _lastNavInfo.Pointer)
//    {
//        double dx = navInfo->PointerX - _lastNavInfo.PointerX;
//        double dy = navInfo->PointerY - _lastNavInfo.PointerY;
//
//        if (true)//(_userOptions->InvertedMouse)
//        {
//            angularVelocity.X = 0.01 * dy / deltaTime;
//            angularVelocity.Y = 0.01 * dx / deltaTime;
//        }
//        else
//        {
//            angularVelocity.X = -1.00 * dy / deltaTime;
//            angularVelocity.Y = -1.00 * dx / deltaTime;
//        }
//    }

//    //	Handle keyboard direction inputs.
//    if (navInfo->PitchFore)	angularVelocity.X -= rotateSpeed;
//    if (navInfo->PitchBack)	angularVelocity.X += rotateSpeed;
//    if (navInfo->YawRight)	angularVelocity.Y -= rotateSpeed;
//    if (navInfo->YawLeft)	angularVelocity.Y += rotateSpeed;
//    if (navInfo->RollRight)	angularVelocity.Z -= rotateSpeed;
//    if (navInfo->RollLeft)	angularVelocity.Z += rotateSpeed;

//    //	If no direction inputs, slow down the spinning and stand upright (if enabled).
//    if (!(navInfo->Pointer ||
//          navInfo->PitchFore ||
//          navInfo->PitchBack ||
//          navInfo->YawRight ||
//          navInfo->YawLeft ||
//          navInfo->RollRight ||
//          navInfo->RollLeft))
//    {
//        // Slow down spinning
//        angularVelocity -= angularVelocity * 1.0f * deltaTime;
//    }

    _placement->AngularVelocity(angularVelocity);

//    _lastNavInfo = *navInfo;
}

void TeapotPlayer::PlaySound(float volume)
{
    if (_hitSound == nullptr)
    {
        return;
    }

    if (true) // (!_userOptions->SoundEffectsOn)
    {
        return;
    }

    _hitSound->Volume(volume);
    _hitSound->PlaySound(true);
}
