#pragma once

#include <pch.h>

#include <TurboVector3D.h>
#include <TurboMatrix4x4.h>

using namespace Turbo::Math;

namespace Turbo
{
	namespace Scene
	{
		class ITurboScenePlacement
		{
		public:
			//  ITurboScenePlacement Properties --------------------------------------------------------------------------------
			virtual TurboMatrix4x4	Transform() = 0;
			virtual TurboMatrix4x4	Inverse() = 0;

			virtual TurboVector3D	Right() = 0;
			//virtual void		Right(TurboVector3D value) = 0;
			virtual TurboVector3D	Up() = 0;
			//virtual void		Up(TurboVector3D value) = 0;
			virtual TurboVector3D	Back() = 0;
			//virtual void		Back(TurboVector3D value) = 0;
			virtual TurboVector3D	Position() = 0;
			//virtual void		Position(TurboVector3D value) = 0;
			virtual TurboVector3D	Target() = 0;
			//virtual void		Target(TurboVector3D value) = 0;
			virtual TurboVector3D	Velocity() = 0;
			virtual void		Velocity(TurboVector3D value) = 0;
			virtual TurboVector3D	AngularVelocity() = 0;
			virtual void		AngularVelocity(TurboVector3D value) = 0;

			//  ITurboScenePlacement Methods -----------------------------------------------------------------------------------
			virtual void	Reset() = 0;
			virtual void	GoTo(double x, double y, double z) = 0;
			virtual void	GoTo(TurboVector3D position) = 0;
			virtual void	Move(double x, double y, double z) = 0;
			virtual void	Move(TurboVector3D velocity) = 0;
			virtual void	Rotate(double x, double y, double z) = 0;
			virtual void	Rotate(TurboVector3D angularVelocity) = 0;
			virtual void	RotateX(double x) = 0;
			virtual void	RotateY(double y) = 0;
			virtual void	RotateZ(double z) = 0;
			//virtual void	Transform(TurboMatrix4x4 transform) = 0;
			//virtual std::shared_ptr<ITurboScenePlacement> Clone() = 0;
		};
	}
}