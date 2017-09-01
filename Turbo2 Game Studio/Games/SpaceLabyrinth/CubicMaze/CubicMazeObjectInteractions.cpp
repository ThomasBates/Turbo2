#include <pch.h>
#include <CubicMazeObjectInteractions.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;


void CubicMazeObjectInteractions::ProcessObjectInteractions(
	NavigationInfo navInfo,
	std::shared_ptr<CubicMaze> maze,
	std::shared_ptr<ITurboSceneObject> sceneObject,
	bool isPlayer,
	int *pPortalIndex)
{
	*pPortalIndex = 0;

	double deltaTime = navInfo.DeltaTime;

	//  Player-Maze Interactions
	std::shared_ptr<ITurboScenePlacement> placement = sceneObject->Placement();
	TurboVector3D velocity = placement->Velocity();
	TurboVector3D angularVelocity = placement->AngularVelocity();

	InputArguments input = {};
	input.maze = maze;
	input.sceneObject = sceneObject;
	input.isPlayer = isPlayer;
	input.fromPoint = placement->Position();;
	input.toPoint = input.fromPoint + velocity * deltaTime;
	input.location = CubicMazeLocation(
		(int)round(input.fromPoint.X / CELLSIZE),
		(int)round(-input.fromPoint.Y / CELLSIZE),
		(int)round(-input.fromPoint.Z / CELLSIZE));

	OutputArguments output = {};

	_debug->Send(debugInformation, debugInteractions) << "Current From " << input.fromPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Current To   " << input.toPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Current Cell " << input.location << "\n";

	IsTouchingCellWall(input, &output);

	//	If we've entered a door, none of the rest matters.
	if (output.portalIndex > 0)
	{
		*pPortalIndex = output.portalIndex;
		return;
	}

	if ((!output.isTouching) && (output.isLeaving))
	{
		CubicMazeCell *cell = maze->Cell(input.location);

		//	Test the adjacent cells that are in the direction of travel.
		int w1 = 0, w2 = 0;
		int h1 = 0, h2 = 0;
		int d1 = 0, d2 = 0;

		if (velocity.X > 0) w2 = 1;
		if (velocity.X < 0) w1 = -1;
		if (velocity.Y > 0) h1 = -1;
		if (velocity.Y < 0) h2 = 1;
		if (velocity.Z > 0) d1 = -1;
		if (velocity.Z < 0) d2 = 1;

		for (int dw = w1; (dw <= w2) && !output.isTouching; dw++)
		for (int dh = h1; (dh <= h2) && !output.isTouching; dh++)
		for (int dd = d1; (dd <= d2) && !output.isTouching; dd++)
		{
			if ((dw < 0) && (cell->LeftWall.Type	!= None)) continue;
			if ((dw > 0) && (cell->RightWall.Type	!= None)) continue;
			if ((dh < 0) && (cell->TopWall.Type		!= None)) continue;
			if ((dh > 0) && (cell->BottomWall.Type	!= None)) continue;
			if ((dd < 0) && (cell->BackWall.Type	!= None)) continue;
			if ((dd > 0) && (cell->FrontWall.Type	!= None)) continue;

			//	Already did 0,0,0 above.
			if ((dw != 0) || (dh != 0) || (dd != 0))
			{
				InputArguments testInput = input;
				testInput.location = CubicMazeLocation(
					input.location.W + dw, 
					input.location.H + dh, 
					input.location.D + dd);

				_debug->Send(debugInformation, debugInteractions) << "Testing Cell " << testInput.location << "\n";

				IsTouchingCellWall(input, &output);
			}
		}
	}

	if (output.isTouching)
	{
		//	bounciness constant?
		TurboVector3D v = velocity * 0.75;
		TurboVector3D n = -output.normal * (-output.normal * v);
		TurboVector3D t = v - n;
		velocity = t - n * 0.5;

		input.toPoint = output.contact + velocity * deltaTime;

		//sceneObject->PlaySound(1);
	}

	placement->Velocity(velocity);
	placement->GoTo(input.toPoint);
	placement->Rotate(angularVelocity * deltaTime);
}

void CubicMazeObjectInteractions::IsTouchingCellWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	CubicMazeCell *cell = input.maze->Cell(input.location);

	if (cell == NULL)
	{
		return;
	}

	_debug->Send(debugInformation, debugInteractions) << "Testing Cell " << input.location << "\n";

	TurboVector3D cellCenter = TurboVector3D(
		input.location.W * CELLSIZE,
		-input.location.H * CELLSIZE,
		-input.location.D * CELLSIZE);

	InputArguments cellInput = input;

	cellInput.fromPoint = input.fromPoint - cellCenter;
	cellInput.toPoint = input.toPoint - cellCenter;

	_debug->Send(debugInformation, debugInteractions) << "Testing From " << cellInput.fromPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Testing To   " << cellInput.toPoint << "\n";

	double nearestDistance = 0.0;
	TurboVector3D nearestContact;
	TurboVector3D nearestNormal;

	OutputArguments cellOutput = {};
	ITurboSceneObject* touchingSceneObject = NULL;


	//	Left Wall --------------------------------------------------------------
	cellInput.cellWall = cell->LeftWall;
	IsTouchingLeftWall(cellInput, &cellOutput);

	if ((input.isPlayer) && (cellOutput.portalIndex > 0))
	{
		pOutput->portalIndex = cellOutput.portalIndex;
		pOutput->isTouching = true;
		input.sceneObject->HitSound(cell->LeftWall.SceneObject->HitSound());
		input.sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		pOutput->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!pOutput->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->LeftWall.SceneObject;
		}
		pOutput->isTouching = true;
	}

	//	Right Wall -------------------------------------------------------------
	cellInput.cellWall = cell->RightWall;
	IsTouchingRightWall(cellInput, &cellOutput);

	if ((input.isPlayer) && (cellOutput.portalIndex > 0))
	{
		pOutput->portalIndex = cellOutput.portalIndex;
		pOutput->isTouching = true;
		input.sceneObject->HitSound(cell->RightWall.SceneObject->HitSound());
		input.sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		pOutput->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!pOutput->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->RightWall.SceneObject;
		}
		pOutput->isTouching = true;
	}

	//	Top Wall ---------------------------------------------------------------
	cellInput.cellWall = cell->TopWall;
	IsTouchingTopWall(cellInput, &cellOutput);

	if ((input.isPlayer) && (cellOutput.portalIndex > 0))
	{
		pOutput->portalIndex = cellOutput.portalIndex;
		pOutput->isTouching = true;
		input.sceneObject->HitSound(cell->TopWall.SceneObject->HitSound());
		input.sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		pOutput->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!pOutput->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->TopWall.SceneObject;
		}
		pOutput->isTouching = true;
	}

	//	Bottom Wall ----------------------------------------------------------------
	cellInput.cellWall = cell->BottomWall;
	IsTouchingBottomWall(cellInput, &cellOutput);

	if ((input.isPlayer) && (cellOutput.portalIndex > 0))
	{
		pOutput->portalIndex = cellOutput.portalIndex;
		pOutput->isTouching = true;
		input.sceneObject->HitSound(cell->BottomWall.SceneObject->HitSound());
		input.sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		pOutput->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!pOutput->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->BottomWall.SceneObject;
		}
		pOutput->isTouching = true;
	}

	//	Front Wall -----------------------------------------------------------------
	cellInput.cellWall = cell->FrontWall;
	IsTouchingFrontWall(cellInput, &cellOutput);

	if ((input.isPlayer) && (cellOutput.portalIndex > 0))
	{
		pOutput->portalIndex = cellOutput.portalIndex;
		pOutput->isTouching = true;
		input.sceneObject->HitSound(cell->FrontWall.SceneObject->HitSound());
		input.sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		pOutput->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!pOutput->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->FrontWall.SceneObject;
		}
		pOutput->isTouching = true;
	}

	//	Back Wall ------------------------------------------------------------------
	cellInput.cellWall = cell->BackWall;
	IsTouchingBackWall(cellInput, &cellOutput);

	if ((input.isPlayer) && (cellOutput.portalIndex > 0))
	{
		pOutput->portalIndex = cellOutput.portalIndex;
		pOutput->isTouching = true;
		input.sceneObject->HitSound(cell->BackWall.SceneObject->HitSound());
		input.sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		pOutput->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!pOutput->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->BackWall.SceneObject;
		}
		pOutput->isTouching = true;
	}

	//	----------------------------------------------------------------------------
	if (pOutput->isTouching)
	{
		pOutput->contact = nearestContact + cellCenter;
		pOutput->normal = nearestNormal;

		if ((input.isPlayer) && (touchingSceneObject != NULL))
		{
			input.sceneObject->HitSound(touchingSceneObject->HitSound());
			input.sceneObject->PlaySound(1);
		}

		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: touching\n";
	}
	else if (pOutput->isLeaving)
	{
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: leaving\n";
	}
	else
	{
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: not touching\n";
	}

	return;
}

void CubicMazeObjectInteractions::IsTouchingRightWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(+CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Right:  min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Right:  max " << max << '\n';

	if ((input.fromPoint.X <= input.toPoint.X) &&	//	moving in +X direction
		(input.fromPoint.X <= min.X) &&				//	fromPoint has not entered wall
		(input.toPoint.X + _hBuffer >= min.X))		//	toPoint has entered buffer zone
	{
		double factor = (min.X - _hBuffer - input.fromPoint.X) / (input.toPoint.X - input.fromPoint.X);

		pOutput->contact = input.fromPoint + (input.toPoint - input.fromPoint) * factor;
		pOutput->normal = TurboVector3D(-1.0, 0.0, 0.0);

		if ((input.isPlayer) && (input.cellWall.PortalIndex > 0))
		{
			if ((pOutput->contact.Y >= -PORTALHALF) && (pOutput->contact.Y <= PORTALHALF) &&
				(pOutput->contact.Z >= -PORTALHALF) && (pOutput->contact.Z <= PORTALHALF))
			{
				pOutput->portalIndex = input.cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Right:  portal " << pOutput->portalIndex << '\n';
				return;
			}
		}

		if ((pOutput->contact.Y >= min.Y - _vBuffer) && (pOutput->contact.Y <= max.Y + _vBuffer) &&
			(pOutput->contact.Z >= min.Z - _hBuffer) && (pOutput->contact.Z <= max.Z + _hBuffer))
		{
			if (input.cellWall.Type == None)
			{
				pOutput->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Right:  leaving\n";
			}
			else
			{
				pOutput->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Right:  touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Right:  not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingLeftWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(-CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Left:   min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Left:   max " << max << '\n';

	if ((input.fromPoint.X >= input.toPoint.X) &&	//	moving in -X direction
		(input.fromPoint.X >= max.X) &&				//	fromPoint has not entered wall
		(input.toPoint.X - _hBuffer <= max.X))		//	toPoint has entered buffer zone
	{
		double factor = (max.X + _hBuffer - input.fromPoint.X) / (input.toPoint.X - input.fromPoint.X);

		pOutput->contact = input.fromPoint + (input.toPoint - input.fromPoint) * factor;
		pOutput->normal = TurboVector3D(1.0, 0.0, 0.0);

		if ((input.isPlayer) && (input.cellWall.PortalIndex > 0))
		{
			if ((pOutput->contact.Y >= -PORTALHALF) && (pOutput->contact.Y <= PORTALHALF) &&
				(pOutput->contact.Z >= -PORTALHALF) && (pOutput->contact.Z <= PORTALHALF))
			{
				pOutput->portalIndex = input.cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Left:   portal " << pOutput->portalIndex << '\n';
				return;
			}
		}

		if ((pOutput->contact.Y >= min.Y - _vBuffer) && (pOutput->contact.Y <= max.Y + _vBuffer) &&
			(pOutput->contact.Z >= min.Z - _hBuffer) && (pOutput->contact.Z <= max.Z + _hBuffer))
		{
			if (input.cellWall.Type == None)
			{
				pOutput->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Left:   leaving\n";
			}
			else
			{
				pOutput->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Left:   touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Left:   not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingBottomWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, -CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Bottom: min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Bottom: max " << max << '\n';

	if ((input.fromPoint.Y >= input.toPoint.Y) &&	//	moving in -Y direction
		(input.fromPoint.Y >= max.Y) &&				//	fromPoint has not entered wall
		(input.toPoint.Y - _vBuffer <= max.Y))		//	toPoint has entered buffer zone
	{
		double factor = (max.Y + _vBuffer - input.fromPoint.Y) / (input.toPoint.Y - input.fromPoint.Y);

		pOutput->contact = input.fromPoint + (input.toPoint - input.fromPoint) * factor;
		pOutput->normal = TurboVector3D(0.0, 1.0, 0.0);

		if ((input.isPlayer) && (input.cellWall.PortalIndex > 0))
		{
			if ((pOutput->contact.Z >= -PORTALHALF) && (pOutput->contact.Z <= PORTALHALF) &&
				(pOutput->contact.X >= -PORTALHALF) && (pOutput->contact.X <= PORTALHALF))
			{
				pOutput->portalIndex = input.cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Bottom: portal " << pOutput->portalIndex << '\n';
				return;
			}
		}

		if ((pOutput->contact.Z >= min.Z - _hBuffer) && (pOutput->contact.Z <= max.Z + _hBuffer) &&
			(pOutput->contact.X >= min.X - _hBuffer) && (pOutput->contact.X <= max.X + _hBuffer))
		{
			if (input.cellWall.Type == None)
			{
				pOutput->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Bottom: leaving\n";
			}
			else
			{
				pOutput->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Bottom: touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Bottom: not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingTopWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, +CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Top:    min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Top:    max " << max << '\n';

	if ((input.fromPoint.Y <= input.toPoint.Y) &&	//	moving in +Y direction
		(input.fromPoint.Y <= min.Y) &&				//	fromPoint has not entered wall
		(input.toPoint.Y + _vBuffer >= min.Y))		//	toPoint has entered buffer zone
	{
		double factor = (min.Y - _vBuffer - input.fromPoint.Y) / (input.toPoint.Y - input.fromPoint.Y);

		pOutput->contact = input.fromPoint + (input.toPoint - input.fromPoint) * factor;
		pOutput->normal = TurboVector3D(0.0, -1.0, 0.0);

		if ((input.isPlayer) && (input.cellWall.PortalIndex > 0))
		{
			if ((pOutput->contact.Z >= -PORTALHALF) && (pOutput->contact.Z <= PORTALHALF) &&
				(pOutput->contact.X >= -PORTALHALF) && (pOutput->contact.X <= PORTALHALF))
			{
				pOutput->portalIndex = input.cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Top:    portal " << pOutput->portalIndex << '\n';
				return;
			}
		}

		if ((pOutput->contact.Z >= min.Z - _hBuffer) && (pOutput->contact.Z <= max.Z + _hBuffer) &&
			(pOutput->contact.X >= min.X - _hBuffer) && (pOutput->contact.X <= max.X + _hBuffer))
		{
			if (input.cellWall.Type == None)
			{
				pOutput->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Top:    leaving\n";
			}
			else
			{
				pOutput->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Top:    touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Top:    not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingFrontWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, -CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Front:  min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Front:  max " << max << '\n';

	if ((input.fromPoint.Z >= input.toPoint.Z) &&	//	moving in -Z direction
		(input.fromPoint.Z >= max.Z) &&				//	fromPoint has not entered wall
		(input.toPoint.Z - _hBuffer <= max.Z))		//	toPoint has entered buffer zone
	{
		double factor = (max.Z + _hBuffer - input.fromPoint.Z) / (input.toPoint.Z - input.fromPoint.Z);

		pOutput->contact = input.fromPoint + (input.toPoint - input.fromPoint) * factor;
		pOutput->normal = TurboVector3D(0.0, 0.0, 1.0);

		if ((input.isPlayer) && (input.cellWall.PortalIndex > 0))
		{
			if ((pOutput->contact.X >= -PORTALHALF) && (pOutput->contact.X <= PORTALHALF) &&
				(pOutput->contact.Y >= -PORTALHALF) && (pOutput->contact.Y <= PORTALHALF))
			{
				pOutput->portalIndex = input.cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Front:  portal " << pOutput->portalIndex << '\n';
				return;
			}
		}

		if ((pOutput->contact.X >= min.X - _hBuffer) && (pOutput->contact.X <= max.X + _hBuffer) &&
			(pOutput->contact.Y >= min.Y - _vBuffer) && (pOutput->contact.Y <= max.Y + _vBuffer))
		{
			if (input.cellWall.Type == None)
			{
				pOutput->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Front:  leaving\n";
			}
			else
			{
				pOutput->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Front:  touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Front:  not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingBackWall(InputArguments input, OutputArguments* pOutput)
{
	pOutput->portalIndex = 0;
	pOutput->isLeaving = false;
	pOutput->isTouching = false;
	pOutput->contact = TurboVector3D();
	pOutput->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, +CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Back:   min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Back:   max " << max << '\n';

	if ((input.fromPoint.Z <= input.toPoint.Z) &&	//	moving in +Z direction
		(input.fromPoint.Z <= min.Z) &&				//	fromPoint has not entered wall
		(input.toPoint.Z + _hBuffer >= min.Z))		//	toPoint has entered buffer zone
	{
		double factor = (min.Z - _hBuffer - input.fromPoint.Z) / (input.toPoint.Z - input.fromPoint.Z);

		pOutput->contact = input.fromPoint + (input.toPoint - input.fromPoint) * factor;
		pOutput->normal = TurboVector3D(0.0, 0.0, -1.0);

		if ((input.isPlayer) && (input.cellWall.PortalIndex > 0))
		{
			if ((pOutput->contact.X >= -PORTALHALF) && (pOutput->contact.X <= PORTALHALF) &&
				(pOutput->contact.Y >= -PORTALHALF) && (pOutput->contact.Y <= PORTALHALF))
			{
				pOutput->portalIndex = input.cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Back:   portal " << pOutput->portalIndex << '\n';
				return;
			}
		}

		if ((pOutput->contact.X >= min.X - _hBuffer) && (pOutput->contact.X <= max.X + _hBuffer) &&
			(pOutput->contact.Y >= min.Y - _vBuffer) && (pOutput->contact.Y <= max.Y + _vBuffer))
		{
			if (input.cellWall.Type == None)
			{
				pOutput->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Back:   leaving\n";
			}
			else
			{
				pOutput->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Back:   touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Back:   not touching\n";
}
