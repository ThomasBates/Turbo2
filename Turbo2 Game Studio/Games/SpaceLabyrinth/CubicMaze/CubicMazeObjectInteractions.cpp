#include <pch.h>
#include <CubicMazeObjectInteractions.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Scene;


void CubicMazeObjectInteractions::ProcessObjectInteractions(NavigationInfo navInfo, int *pPortalIndex)
{
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
	
	//CubicMazeLocation newLocation = CubicMazeLocation(
	//	(int)round(newPosition.X / CELLSIZE), 
	//	(int)round(-newPosition.Y / CELLSIZE), 
	//	(int)round(-newPosition.Z / CELLSIZE));

	//if (oldPosition.Z > 1.0)
	//{
	//	_debug->Send(Information, DebugLevel0) << "Outside\n";
	//}

	TurboVector3D contact;
	TurboVector3D normal;

	int portalIndex;

	bool isTouching = IsTouchingCellWall(
		oldLocation, 
		oldPosition, 
		newPosition, 
		buffer, 
		&contact, 
		&normal, 
		&portalIndex);

	if (portalIndex > 0)
	{
		*pPortalIndex = portalIndex;
		return;
	}

	if (!isTouching)
	{
		for (int dw = -1; (dw <= 1) && !isTouching; dw++)
		for (int dh = -1; (dh <= 1) && !isTouching; dh++)
		for (int dd = -1; (dd <= 1) && !isTouching; dd++)
		{
			//	Already did 0,0,0 above.
			if ((dw != 0) && (dh != 0) && (dd != 0))
			{
				CubicMazeLocation tempLocation = CubicMazeLocation(
					oldLocation.W + dw, 
					oldLocation.H + dh, 
					oldLocation.D + dd);

				isTouching = IsTouchingCellWall(
					oldLocation, 
					oldPosition, 
					newPosition, 
					buffer, 
					&contact, 
					&normal, 
					&portalIndex);
			}
		}
	}

	if (isTouching)
	{
		TurboVector3D v = velocity * 0.75;
		TurboVector3D n = -normal * (-normal * v);
		TurboVector3D t = v - n;
		velocity = t - n * 0.5;

		newPosition = oldPosition + velocity * deltaTime;
	}



	placement->Velocity(velocity);
	placement->GoTo(newPosition);
	placement->Rotate(angularVelocity * deltaTime);
}

bool CubicMazeObjectInteractions::IsTouchingCellWall(
	CubicMazeLocation location,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	TurboVector3D *pContact,
	TurboVector3D *pNormal,
	int *pPortalIndex)
{
	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	CubicMazeCell *cell = _maze->Cell(location);

	if (cell == NULL)
	{
		return false;
	}

	TurboVector3D cellCenter = TurboVector3D(
		location.W * CELLSIZE,
		-location.H * CELLSIZE,
		-location.D * CELLSIZE);

	TurboVector3D cellFromPoint = fromPoint - cellCenter;
	TurboVector3D celltoPoint = toPoint - cellCenter;

	bool result = false;

	double nearestDistance = 0.0;
	TurboVector3D nearestContact;
	TurboVector3D nearestNormal;

	TurboVector3D contact;
	TurboVector3D normal;
	int portalIndex;

	if (IsTouchingLeftWall(cell->LeftWall, cellFromPoint, celltoPoint, buffer, &contact, &normal, &portalIndex))
	{
		if (portalIndex > 0)
		{
			*pPortalIndex = portalIndex;
			return true;
		}

		double distance = (contact - cellFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingRightWall(cell->RightWall, cellFromPoint, celltoPoint, buffer, &contact, &normal, &portalIndex))
	{
		if (portalIndex > 0)
		{
			*pPortalIndex = portalIndex;
			return true;
		}

		double distance = (contact - cellFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingTopWall(cell->TopWall, cellFromPoint, celltoPoint, buffer, &contact, &normal, &portalIndex))
	{
		if (portalIndex > 0)
		{
			*pPortalIndex = portalIndex;
			return true;
		}

		double distance = (contact - cellFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingBottomWall(cell->BottomWall, cellFromPoint, celltoPoint, buffer, &contact, &normal, &portalIndex))
	{
		if (portalIndex > 0)
		{
			*pPortalIndex = portalIndex;
			return true;
		}

		double distance = (contact - cellFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingFrontWall(cell->FrontWall, cellFromPoint, celltoPoint, buffer, &contact, &normal, &portalIndex))
	{
		if (portalIndex > 0)
		{
			*pPortalIndex = portalIndex;
			return true;
		}

		double distance = (contact - cellFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingBackWall(cell->BackWall, cellFromPoint, celltoPoint, buffer, &contact, &normal, &portalIndex))
	{
		if (portalIndex > 0)
		{
			*pPortalIndex = portalIndex;
			return true;
		}

		double distance = (contact - cellFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (result)
	{
		*pContact = nearestContact + cellCenter;
		*pNormal = nearestNormal;
	}

	return result;
}

bool CubicMazeObjectInteractions::IsTouchingRightWall(
	CubicMazeCellWall cellWall, 
	TurboVector3D fromPoint, 
	TurboVector3D toPoint, 
	double buffer, 
	TurboVector3D *pContact, 
	TurboVector3D *pNormal, 
	int *pPortalIndex)
{
	if (cellWall.Type == None)
	{
		return false;
	}

	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	TurboVector3D min = TurboVector3D(+CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	//_debug->Send(Information, DebugLevel0) << "Right:  from " << fromPoint << ", to " << toPoint << ", min " << min << ", max " << max << '\n';

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
				return true;
			}
		}

		if ((pContact->Y >= min.Y) && (pContact->Y <= max.Y) &&
			(pContact->Z >= min.Z) && (pContact->Z <= max.Z))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeObjectInteractions::IsTouchingLeftWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	TurboVector3D *pContact,
	TurboVector3D *pNormal,
	int *pPortalIndex)
{
	if (cellWall.Type == None)
	{
		return false;
	}

	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(-CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	//_debug->Send(Information, DebugLevel0) << "Left:  from " << fromPoint << ", to " << toPoint << ", min " << min << ", max " << max << '\n';

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
				return true;
			}
		}

		if ((pContact->Y >= min.Y) && (pContact->Y <= max.Y) &&
			(pContact->Z >= min.Z) && (pContact->Z <= max.Z))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeObjectInteractions::IsTouchingBottomWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	TurboVector3D *pContact,
	TurboVector3D *pNormal,
	int *pPortalIndex)
{
	if (cellWall.Type == None)
	{
		return false;
	}

	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, -CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	//_debug->Send(Information, DebugLevel0) << "Bottom:  from " << fromPoint << ", to " << toPoint << ", min " << min << ", max " << max << '\n';

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
				return true;
			}
		}

		if ((pContact->Z >= min.Z) && (pContact->Z <= max.Z) &&
			(pContact->X >= min.X) && (pContact->X <= max.X))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeObjectInteractions::IsTouchingTopWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	TurboVector3D *pContact,
	TurboVector3D *pNormal,
	int *pPortalIndex)
{
	if (cellWall.Type == None)
	{
		return false;
	}

	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, +CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	//_debug->Send(Information, DebugLevel0) << "Top:  from " << fromPoint << ", to " << toPoint << ", min " << min << ", max " << max << '\n';

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
				return true;
			}
		}

		if ((pContact->Z >= min.Z) && (pContact->Z <= max.Z) &&
			(pContact->X >= min.X) && (pContact->X <= max.X))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeObjectInteractions::IsTouchingFrontWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	TurboVector3D *pContact,
	TurboVector3D *pNormal,
	int *pPortalIndex)
{
	if (cellWall.Type == None)
	{
		return false;
	}

	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, -CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, -CELLHALF + WALLHALF);

	//_debug->Send(Information, DebugLevel0) << "Front:  from " << fromPoint << ", to " << toPoint << ", min " << min << ", max " << max << '\n';

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
				return true;
			}
		}

		if ((pContact->X >= min.X) && (pContact->X <= max.X) &&
			(pContact->Y >= min.Y) && (pContact->Y <= max.Y))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeObjectInteractions::IsTouchingBackWall(
	CubicMazeCellWall cellWall,
	TurboVector3D fromPoint,
	TurboVector3D toPoint,
	double buffer,
	TurboVector3D *pContact,
	TurboVector3D *pNormal,
	int *pPortalIndex)
{
	if (cellWall.Type == None)
	{
		return false;
	}

	*pContact = TurboVector3D();
	*pNormal = TurboVector3D();
	*pPortalIndex = 0;

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF, -CELLHALF - WALLHALF, +CELLHALF - WALLHALF);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF, +CELLHALF + WALLHALF, +CELLHALF + WALLHALF);

	//_debug->Send(Information, DebugLevel0) << "Back:  from " << fromPoint << ", to " << toPoint << ", min " << min << ", max " << max << '\n';

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
				return true;
			}
		}

		if ((pContact->X >= min.X) && (pContact->X <= max.X) &&
			(pContact->Y >= min.Y) && (pContact->Y <= max.Y))
		{
			return true;
		}
	}

	return false;
}
