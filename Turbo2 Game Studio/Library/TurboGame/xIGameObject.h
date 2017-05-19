#pragma once

#include <IAnimate.h>
#include <IMaterial.h>
#include <IMeshObject.h>
#include <IPlatformResources.h>
#include <ISoundEffect.h>

#include <Vector3D.h>

class IGameObject
{
public:
	//  IGameObject Methods  -------------------------------------------------------------------------------------------
	virtual int		IsTouching() = 0;
	virtual void	Render(IPlatformResources *platformResources) = 0;
	virtual void	PlaySound(float impactSpeed, Vector3D eyePoint) = 0;

	//  IGameObject Properties  ----------------------------------------------------------------------------------------
	virtual void			SetMesh(IMeshObject *mesh) = 0;
	virtual IMaterial		*GetNormalMaterial() = 0;
	virtual void			SetNormalMaterial(IMaterial *material) = 0;
	virtual IMaterial		*GetHitMaterial() = 0;
	virtual void			SetHitMaterial(IMaterial *material) = 0;
	virtual IAnimate		*GetAnimatePosition() = 0;
	virtual void			SetAnimatePosition(IAnimate *animatePosition) = 0;
	virtual ISoundEffect	*GetHitSound() = 0;
	virtual void			SetHitSound(ISoundEffect *hitSound) = 0;

	virtual Vector3D		Position() = 0;
	virtual void			Position(Vector3D position) = 0;
	virtual Vector3D		Velocity() = 0;
	virtual void			Velocity(Vector3D velocity) = 0;
	//virtual Matrix			GetModelMatrix() = 0;

	virtual int				IsActive() = 0;
	virtual void			IsActive(int isActive) = 0;
	virtual int				IsTarget() = 0;
	virtual void			IsTarget(int isTarget) = 0;
	virtual int				IsHit() = 0;
	virtual void			IsHit(int isHit) = 0;
};