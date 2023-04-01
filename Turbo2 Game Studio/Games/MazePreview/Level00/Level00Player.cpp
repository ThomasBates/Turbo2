
#include <pch.h>

#include <ITurboSceneObject.h>
#include <ITurboScenePlacement.h>
#include <TurboScenePlacement.h>

#include <Level00Player.h>

using namespace Turbo::Math;
using namespace Turbo::Scene;

Level00Player::Level00Player(std::shared_ptr<UserOptions>  userOptions) :
	_userOptions(std::move(userOptions))
{
	_placement = std::shared_ptr<ITurboScenePlacement>(new TurboScenePlacement());
}

void Level00Player::Update(NavigationInfo* navInfo)
{
	const double cMoveAcceleration = 2.0f;

	double deltaTime = navInfo->DeltaTime;
	double moveSpeed = cMoveAcceleration * deltaTime;

	TurboVector3D velocity = _placement->Velocity();

	for (auto& control: navInfo->Controls)
	{
		if ((control->Type() == TurboGameControlType::Move) &&
			(control->IsActive()))
		{
			velocity += _placement->Up()    * (float)moveSpeed * control->ZValue();
			velocity += _placement->Back()  * (float)moveSpeed * control->YValue();
			velocity += _placement->Right() * (float)moveSpeed * control->XValue();
		}
	}


	_placement->Velocity(velocity);

	TurboVector3D angularVelocity = _placement->AngularVelocity();


	for (auto& control: navInfo->Controls)
	{
		if ((control->Type() == TurboGameControlType::Look) &&
			(control->IsActive()))
		{
			float dx = control->XValue();
			float dy = control->YValue();
			float dz = control->ZValue();

			angularVelocity.X = dy / (float)deltaTime;
			angularVelocity.Y = dx / (float)deltaTime;
			angularVelocity.Z = dz / (float)deltaTime;
		}
	}

	_placement->AngularVelocity(angularVelocity);
}

void Level00Player::PlaySound(float volume)
{
	if (_hitSound == nullptr)
	{
		return;
	}

	if (!_userOptions->SoundEffectsOn()->GetValue())
	{
		return;
	}

	_hitSound->Volume(volume);
	_hitSound->PlaySound(true);
}
