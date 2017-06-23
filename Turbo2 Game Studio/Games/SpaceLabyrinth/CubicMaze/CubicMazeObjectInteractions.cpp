#include <pch.h>
#include <CubicMazeObjectInteractions.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;


void CubicMazeObjectInteractions::ProcessObjectInteractions(NavigationInfo navInfo, int *pPortalIndex)
{
	*pPortalIndex = 0;

	const double buffer = 0.25;
	const double bounciness = 0.25;

	double deltaTime = navInfo.DeltaTime;

	//  Player-Maze Interactions
	std::shared_ptr<ITurboScenePlacement> placement = _player->Placement();
	TurboVector3D velocity = placement->Velocity();
	TurboVector3D angularVelocity = placement->AngularVelocity();
	TurboVector3D oldPosition = placement->Position();
	TurboVector3D newPosition = oldPosition + velocity * deltaTime;

	CubicMazeLocation oldLocation = CubicMazeLocation(
		(int)round(oldPosition.X / CELLSIZE), 
		(int)round(-oldPosition.Y / CELLSIZE), 
		(int)round(-oldPosition.Z / CELLSIZE));

	TurboVector3D contact;
	TurboVector3D normal;

	int portalIndex;
	bool isLeaving;
	bool isTouching;

	_debug->Send(debugInformation, debugInteractions) << "Current From " << oldPosition << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Current To   " << newPosition << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Current Cell " << oldLocation << "\n";

	IsTouchingCellWall(
		oldLocation, 
		oldPosition, 
		newPosition, 
		buffer,
		&portalIndex,
		&isLeaving,
		&isTouching,
		&contact, 
		&normal);

	//	If we've entered a door, none of the rest matters.
	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		return;
	}

	if (!isTouching && isLeaving)
	{
		CubicMazeCell *cell = _maze->Cell(oldLocation);

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

		for (int dw = w1; (dw <= w2) && !isTouching; dw++)
		for (int dh = h1; (dh <= h2) && !isTouching; dh++)
		for (int dd = d1; (dd <= d2) && !isTouching; dd++)
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
				CubicMazeLocation testLocation = CubicMazeLocation(
					oldLocation.W + dw, 
					oldLocation.H + dh, 
					oldLocation.D + dd);

				_debug->Send(debugInformation, debugInteractions) << "Testing Cell " << testLocation << "\n";

				IsTouchingCellWall(
					testLocation,
					oldPosition, 
					newPosition, 
					buffer,
					&portalIndex,
					&isLeaving,
					&isTouching,
					&contact, 
					&normal);
			}
		}
	}

	if (isTouching)
	{
		TurboVector3D v = velocity * 0.75;
		TurboVector3D n = -normal * (-normal * v);
		TurboVector3D t = v - n;
		velocity = t - n * 0.5;

		newPosition = contact + velocity * deltaTime;
	}

	placement->Velocity(velocity);
	placement->GoTo(newPosition);
	placement->Rotate(angularVelocity * deltaTime);
}

void CubicMazeObjectInteractions::IsTouchingCellWall(
	CubicMazeLocation location,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	CubicMazeCell *cell = _maze->Cell(location);

	if (cell == NULL)
	{
		return;
	}

	_debug->Send(debugInformation, debugInteractions) << "Testing Cell " << location << "\n";

	TurboVector3D cellCenter = TurboVector3D(
		location.W * CELLSIZE,
		-location.H * CELLSIZE,
		-location.D * CELLSIZE);

	TurboVector3D cellFromPoint = fromPoint - cellCenter;
	TurboVector3D celltoPoint = toPoint - cellCenter;

	_debug->Send(debugInformation, debugInteractions) << "Testing From " << cellFromPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Testing To   " << celltoPoint << "\n";

	double nearestDistance = 0.0;
	TurboVector3D nearestContact;
	TurboVector3D nearestNormal;

	int portalIndex;
	bool isLeaving;
	bool isTouching;
	TurboVector3D contact;
	TurboVector3D normal;


	//	Left Wall --------------------------------------------------------------
	IsTouchingLeftWall(
		cell->LeftWall, cellFromPoint, celltoPoint, buffer, 
		&portalIndex, &isLeaving, &isTouching, &contact, &normal);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		*pIsTouching = true;
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << portalIndex << '\n';
		return;
	}

	if (isLeaving)
	{
		*pIsLeaving = true;
	}

	if (isTouching)
	{
		double distance = (contact - cellFromPoint).Length();

		if ((!*pIsTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
		}
		*pIsTouching = true;
	}

	//	Right Wall -------------------------------------------------------------
	IsTouchingRightWall(
		cell->RightWall, cellFromPoint, celltoPoint, buffer,
		&portalIndex, &isLeaving, &isTouching, &contact, &normal);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		*pIsTouching = true;
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << portalIndex << '\n';
		return;
	}

	if (isLeaving)
	{
		*pIsLeaving = true;
	}

	if (isTouching)
	{
		double distance = (contact - cellFromPoint).Length();

		if ((!*pIsTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
		}
		*pIsTouching = true;
	}

	//	Top Wall ---------------------------------------------------------------
	IsTouchingTopWall(
		cell->TopWall, cellFromPoint, celltoPoint, buffer,
		&portalIndex, &isLeaving, &isTouching, &contact, &normal);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		*pIsTouching = true;
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << portalIndex << '\n';
		return;
	}

	if (isLeaving)
	{
		*pIsLeaving = true;
	}

	if (isTouching)
	{
		double distance = (contact - cellFromPoint).Length();

		if ((!*pIsTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
		}
		*pIsTouching = true;
	}

	//	Bottom Wall ----------------------------------------------------------------
	IsTouchingBottomWall(
		cell->BottomWall, cellFromPoint, celltoPoint, buffer,
		&portalIndex, &isLeaving, &isTouching, &contact, &normal);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		*pIsTouching = true;
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << portalIndex << '\n';
		return;
	}

	if (isLeaving)
	{
		*pIsLeaving = true;
	}

	if (isTouching)
	{
		double distance = (contact - cellFromPoint).Length();

		if ((!*pIsTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
		}
		*pIsTouching = true;
	}

	//	Front Wall -----------------------------------------------------------------
	IsTouchingFrontWall(
		cell->FrontWall, cellFromPoint, celltoPoint, buffer,
		&portalIndex, &isLeaving, &isTouching, &contact, &normal);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		*pIsTouching = true;
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << portalIndex << '\n';
		return;
	}

	if (isLeaving)
	{
		*pIsLeaving = true;
	}

	if (isTouching)
	{
		double distance = (contact - cellFromPoint).Length();

		if ((!*pIsTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
		}
		*pIsTouching = true;
	}

	//	Back Wall ------------------------------------------------------------------
	IsTouchingBackWall(
		cell->BackWall, cellFromPoint, celltoPoint, buffer,
		&portalIndex, &isLeaving, &isTouching, &contact, &normal);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		*pIsTouching = true;
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << portalIndex << '\n';
		return;
	}

	if (isLeaving)
	{
		*pIsLeaving = true;
	}

	if (isTouching)
	{
		double distance = (contact - cellFromPoint).Length();

		if ((!*pIsTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
		}
		*pIsTouching = true;
	}

	if (*pIsTouching)
	{
		*pContact = nearestContact + cellCenter;
		*pNormal = nearestNormal;

		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: touching\n";
	}
	else if (*pIsLeaving)
	{
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: leaving\n";
	}
	else
	{
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: not touching\n";
	}

	return;
}

void CubicMazeObjectInteractions::IsTouchingRightWall(
	CubicMazeCellWall cellWall, 
	TurboVector3D fromPoint, 
	TurboVector3D toPoint, 
	double buffer, 
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	TurboVector3D min = TurboVector3D(+CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Right:  min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Right:  max " << max << '\n';

	if ((fromPoint.X <= toPoint.X) &&	//	moving in +X direction
		(fromPoint.X <= min.X) &&		//	fromPoint has not entered wall
		(toPoint.X + buffer >= min.X))	//	toPoint has entered buffer zone
	{
		double factor = (min.X - buffer - fromPoint.X) / (toPoint.X - fromPoint.X);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(-1.0, 0.0, 0.0);

		if (cellWall.PortalIndex > 0)
		{
			if ((pContact->Y >= -PORTALHALF) && (pContact->Y <= PORTALHALF) &&
				(pContact->Z >= -PORTALHALF) && (pContact->Z <= PORTALHALF))
			{
				*pPortalIndex = cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Right:  portal " << *pPortalIndex << '\n';
				return;
			}
		}

		if ((pContact->Y >= min.Y - buffer) && (pContact->Y <= max.Y + buffer) &&
			(pContact->Z >= min.Z - buffer) && (pContact->Z <= max.Z + buffer))
		{
			if (cellWall.Type == None)
			{
				*pIsLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Right:  leaving\n";
			}
			else
			{
				*pIsTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Right:  touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Right:  not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingLeftWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(-CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Left:   min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Left:   max " << max << '\n';

	if ((fromPoint.X >= toPoint.X) &&	//	moving in -X direction
		(fromPoint.X >= max.X) &&		//	fromPoint has not entered wall
		(toPoint.X - buffer <= max.X))	//	toPoint has entered buffer zone
	{
		double factor = (max.X + buffer - fromPoint.X) / (toPoint.X - fromPoint.X);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(1.0, 0.0, 0.0);

		if (cellWall.PortalIndex > 0)
		{
			if ((pContact->Y >= -PORTALHALF) && (pContact->Y <= PORTALHALF) &&
				(pContact->Z >= -PORTALHALF) && (pContact->Z <= PORTALHALF))
			{
				*pPortalIndex = cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Left:   portal " << *pPortalIndex << '\n';
				return;
			}
		}

		if ((pContact->Y >= min.Y - buffer) && (pContact->Y <= max.Y + buffer) &&
			(pContact->Z >= min.Z - buffer) && (pContact->Z <= max.Z + buffer))
		{
			if (cellWall.Type == None)
			{
				*pIsLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Left:   leaving\n";
			}
			else
			{
				*pIsTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Left:   touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Left:   not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingBottomWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, -CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Bottom: min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Bottom: max " << max << '\n';

	if ((fromPoint.Y >= toPoint.Y) &&	//	moving in -Y direction
		(fromPoint.Y >= max.Y) &&		//	fromPoint has not entered wall
		(toPoint.Y - buffer <= max.Y))	//	toPoint has entered buffer zone
	{
		double factor = (max.Y + buffer - fromPoint.Y) / (toPoint.Y - fromPoint.Y);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, 1.0, 0.0);

		if (cellWall.PortalIndex > 0)
		{
			if ((pContact->Z >= -PORTALHALF) && (pContact->Z <= PORTALHALF) &&
				(pContact->X >= -PORTALHALF) && (pContact->X <= PORTALHALF))
			{
				*pPortalIndex = cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Bottom: portal " << *pPortalIndex << '\n';
				return;
			}
		}

		if ((pContact->Z >= min.Z - buffer) && (pContact->Z <= max.Z + buffer) &&
			(pContact->X >= min.X - buffer) && (pContact->X <= max.X + buffer))
		{
			if (cellWall.Type == None)
			{
				*pIsLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Bottom: leaving\n";
			}
			else
			{
				*pIsTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Bottom: touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Bottom: not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingTopWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, +CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Top:    min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Top:    max " << max << '\n';

	if ((fromPoint.Y <= toPoint.Y) &&	//	moving in +Y direction
		(fromPoint.Y <= min.Y) &&		//	fromPoint has not entered wall
		(toPoint.Y + buffer >= min.Y))	//	toPoint has entered buffer zone
	{
		double factor = (min.Y - buffer - fromPoint.Y) / (toPoint.Y - fromPoint.Y);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, -1.0, 0.0);

		if (cellWall.PortalIndex > 0)
		{
			if ((pContact->Z >= -PORTALHALF) && (pContact->Z <= PORTALHALF) &&
				(pContact->X >= -PORTALHALF) && (pContact->X <= PORTALHALF))
			{
				*pPortalIndex = cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Top:    portal " << *pPortalIndex << '\n';
				return;
			}
		}

		if ((pContact->Z >= min.Z - buffer) && (pContact->Z <= max.Z + buffer) &&
			(pContact->X >= min.X - buffer) && (pContact->X <= max.X + buffer))
		{
			if (cellWall.Type == None)
			{
				*pIsLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Top:    leaving\n";
			}
			else
			{
				*pIsTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Top:    touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Top:    not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingFrontWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, -CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Front:  min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Front:  max " << max << '\n';

	if ((fromPoint.Z >= toPoint.Z) &&	//	moving in -Z direction
		(fromPoint.Z >= max.Z) &&		//	fromPoint has not entered wall
		(toPoint.Z - buffer <= max.Z))	//	toPoint has entered buffer zone
	{
		double factor = (max.Z + buffer - fromPoint.Z) / (toPoint.Z - fromPoint.Z);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, 0.0, 1.0);

		if (cellWall.PortalIndex > 0)
		{
			if ((pContact->X >= -PORTALHALF) && (pContact->X <= PORTALHALF) &&
				(pContact->Y >= -PORTALHALF) && (pContact->Y <= PORTALHALF))
			{
				*pPortalIndex = cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Front:  portal " << *pPortalIndex << '\n';
				return;
			}
		}

		if ((pContact->X >= min.X - buffer) && (pContact->X <= max.X + buffer) &&
			(pContact->Y >= min.Y - buffer) && (pContact->Y <= max.Y + buffer))
		{
			if (cellWall.Type == None)
			{
				*pIsLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Front:  leaving\n";
			}
			else
			{
				*pIsTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Front:  touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Front:  not touching\n";
}

void CubicMazeObjectInteractions::IsTouchingBackWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	int *pPortalIndex,
	bool *pIsLeaving,
	bool *pIsTouching,
	TurboVector3D *pContact,
	TurboVector3D *pNormal)
{
	*pPortalIndex = 0;
	*pIsLeaving = false;
	*pIsTouching = false;
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, +CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	_debug->Send(debugInformation, debugInteractions) << "Back:   min " << min << '\n';
	_debug->Send(debugInformation, debugInteractions) << "Back:   max " << max << '\n';

	if ((fromPoint.Z <= toPoint.Z) &&	//	moving in +Z direction
		(fromPoint.Z <= min.Z) &&		//	fromPoint has not entered wall
		(toPoint.Z + buffer >= min.Z))	//	toPoint has entered buffer zone
	{
		double factor = (min.Z - buffer - fromPoint.Z) / (toPoint.Z - fromPoint.Z);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, 0.0, -1.0);

		if (cellWall.PortalIndex > 0)
		{
			if ((pContact->X >= -PORTALHALF) && (pContact->X <= PORTALHALF) &&
				(pContact->Y >= -PORTALHALF) && (pContact->Y <= PORTALHALF))
			{
				*pPortalIndex = cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Back:   portal " << *pPortalIndex << '\n';
				return;
			}
		}

		if ((pContact->X >= min.X - buffer) && (pContact->X <= max.X + buffer) &&
			(pContact->Y >= min.Y - buffer) && (pContact->Y <= max.Y + buffer))
		{
			if (cellWall.Type == None)
			{
				*pIsLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Back:   leaving\n";
			}
			else
			{
				*pIsTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Back:   touching\n";
			}
			return;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Back:   not touching\n";
}
