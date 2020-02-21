#include <pch.h>
#include <CubicMazeObjectInteractions.h>

using namespace Turbo::Scene;


void CubicMazeObjectInteractions::ProcessKeyHazardInteractions(
	NavigationInfo* navInfo, 
	std::shared_ptr<ITurboSceneObject> sceneObject, 
	std::vector<std::shared_ptr<ITurboSceneObject>>* keys, 
	std::vector<std::shared_ptr<ITurboSceneObject>>* hazards)
{
	std::shared_ptr<ITurboScenePlacement> placement = sceneObject->Placement();
	TurboVector3D objectPosition = placement->Position();
	TurboVector3D velocity = placement->Velocity();

	for (auto& key : *keys)
	{
		TurboVector3D keyPosition = key->Placement()->Position();
		TurboVector3D direction = objectPosition - keyPosition;

		if (direction.Length() >= 0.25)
		{
			continue;
		}

		TurboVector3D normal = direction.Normalize();
		TurboVector3D n = -normal * abs(-normal * velocity);
		velocity = velocity - n * 2;

		placement->Velocity(velocity);
	}
	
	for (auto& hazard : *hazards)
	{
		TurboVector3D hazardPosition = hazard->Placement()->Position();
		TurboVector3D direction = objectPosition - hazardPosition;

		if (direction.Length() >= 0.25)
		{
			continue;
		}

		TurboVector3D normal = direction.Normalize();
		TurboVector3D n = -normal * abs(-normal * velocity);
		velocity = velocity - n * 2;

		placement->Velocity(velocity);
	}
}

void CubicMazeObjectInteractions::ProcessObjectInteractions(
	NavigationInfo* navInfo,
	std::shared_ptr<CubicMaze> maze,
	std::shared_ptr<ITurboSceneObject> sceneObject,
	bool isPlayer,
	int *pPortalIndex)
{
	*pPortalIndex = 0;

	double deltaTime = navInfo->DeltaTime;

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
	input.isNeighborCell = false;

	OutputArguments output = {};

	_debug->Send(debugInformation, debugInteractions) << "================================================================================\n";
	_debug->Send(debugInformation, debugInteractions) << "Current From " << input.fromPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Current To   " << input.toPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Current Cell " << input.location << "\n";

	IsTouchingCellWall(&input, &output);

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
			//	Already did 0,0,0 above.
			if ((dw == 0) && (dh == 0) && (dd == 0))	continue;

			//	Don't try a neighboring cell if there is a wall in that direction.
			if ((dw < 0) && (cell->LeftWall.Type	!= CubicMazeCellWallType::None)) continue;
			if ((dw > 0) && (cell->RightWall.Type	!= CubicMazeCellWallType::None)) continue;
			if ((dh < 0) && (cell->TopWall.Type		!= CubicMazeCellWallType::None)) continue;
			if ((dh > 0) && (cell->BottomWall.Type	!= CubicMazeCellWallType::None)) continue;
			if ((dd < 0) && (cell->BackWall.Type	!= CubicMazeCellWallType::None)) continue;
			if ((dd > 0) && (cell->FrontWall.Type	!= CubicMazeCellWallType::None)) continue;

			InputArguments testInput = input;
			testInput.location = CubicMazeLocation(
				input.location.W + dw, 
				input.location.H + dh, 
				input.location.D + dd);
			testInput.isNeighborCell = true;

			_debug->Send(debugInformation, debugInteractions) << "Testing Cell " << testInput.location << "\n";

			IsTouchingCellWall(&testInput, &output);
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

void CubicMazeObjectInteractions::IsTouchingCellWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	CubicMazeCell *cell = input->maze->Cell(input->location);

	if (cell == NULL)
	{
		return;
	}

	_debug->Send(debugInformation, debugInteractions) << "Testing Cell " << input->location << "\n";

	TurboVector3D cellCenter = TurboVector3D(
		input->location.W * CELLSIZE,
		-input->location.H * CELLSIZE,
		-input->location.D * CELLSIZE);

	InputArguments cellInput = *input;

	cellInput.fromPoint = input->fromPoint - cellCenter;
	cellInput.toPoint = input->toPoint - cellCenter;

	_debug->Send(debugInformation, debugInteractions) << "Testing From " << cellInput.fromPoint << "\n";
	_debug->Send(debugInformation, debugInteractions) << "Testing To   " << cellInput.toPoint << "\n";

	double nearestDistance = 0.0;
	TurboVector3D nearestContact;
	TurboVector3D nearestNormal;

	OutputArguments cellOutput = {};
	ITurboSceneObject* touchingSceneObject = NULL;


	//	Left Wall --------------------------------------------------------------
	cellInput.cellWall = cell->LeftWall;
	IsTouchingLeftWall(&cellInput, &cellOutput);

	if ((input->isPlayer) && (cellOutput.portalIndex > 0))
	{
		output->portalIndex = cellOutput.portalIndex;
		output->isTouching = true;
		input->sceneObject->HitSound(cell->LeftWall.SceneObject->HitSound());
		input->sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		output->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!output->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->LeftWall.SceneObject;
		}
		output->isTouching = true;
	}

	//	Right Wall -------------------------------------------------------------
	cellInput.cellWall = cell->RightWall;
	IsTouchingRightWall(&cellInput, &cellOutput);

	if ((input->isPlayer) && (cellOutput.portalIndex > 0))
	{
		output->portalIndex = cellOutput.portalIndex;
		output->isTouching = true;
		input->sceneObject->HitSound(cell->RightWall.SceneObject->HitSound());
		input->sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		output->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!output->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->RightWall.SceneObject;
		}
		output->isTouching = true;
	}

	//	Top Wall ---------------------------------------------------------------
	cellInput.cellWall = cell->TopWall;
	IsTouchingTopWall(&cellInput, &cellOutput);

	if ((input->isPlayer) && (cellOutput.portalIndex > 0))
	{
		output->portalIndex = cellOutput.portalIndex;
		output->isTouching = true;
		input->sceneObject->HitSound(cell->TopWall.SceneObject->HitSound());
		input->sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		output->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!output->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->TopWall.SceneObject;
		}
		output->isTouching = true;
	}

	//	Bottom Wall ----------------------------------------------------------------
	cellInput.cellWall = cell->BottomWall;
	IsTouchingBottomWall(&cellInput, &cellOutput);

	if ((input->isPlayer) && (cellOutput.portalIndex > 0))
	{
		output->portalIndex = cellOutput.portalIndex;
		output->isTouching = true;
		input->sceneObject->HitSound(cell->BottomWall.SceneObject->HitSound());
		input->sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		output->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!output->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->BottomWall.SceneObject;
		}
		output->isTouching = true;
	}

	//	Front Wall -----------------------------------------------------------------
	cellInput.cellWall = cell->FrontWall;
	IsTouchingFrontWall(&cellInput, &cellOutput);

	if ((input->isPlayer) && (cellOutput.portalIndex > 0))
	{
		output->portalIndex = cellOutput.portalIndex;
		output->isTouching = true;
		input->sceneObject->HitSound(cell->FrontWall.SceneObject->HitSound());
		input->sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		output->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!output->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->FrontWall.SceneObject;
		}
		output->isTouching = true;
	}

	//	Back Wall ------------------------------------------------------------------
	cellInput.cellWall = cell->BackWall;
	IsTouchingBackWall(&cellInput, &cellOutput);

	if ((input->isPlayer) && (cellOutput.portalIndex > 0))
	{
		output->portalIndex = cellOutput.portalIndex;
		output->isTouching = true;
		input->sceneObject->HitSound(cell->BackWall.SceneObject->HitSound());
		input->sceneObject->PlaySound(1);
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: portal " << cellOutput.portalIndex << '\n';
		return;
	}

	if (cellOutput.isLeaving)
	{
		output->isLeaving = true;
	}

	if (cellOutput.isTouching)
	{
		double distance = (cellOutput.contact - cellInput.fromPoint).Length();

		if ((!output->isTouching) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = cellOutput.contact;
			nearestNormal = cellOutput.normal;
			touchingSceneObject = cell->BackWall.SceneObject;
		}
		output->isTouching = true;
	}

	//	----------------------------------------------------------------------------
	if (output->isTouching)
	{
		output->contact = nearestContact + cellCenter;
		output->normal = nearestNormal;

		if ((input->isPlayer) && (touchingSceneObject != NULL))
		{
			input->sceneObject->HitSound(touchingSceneObject->HitSound());
			input->sceneObject->PlaySound(1);
		}

		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: touching\n";
	}
	else if (output->isLeaving)
	{
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: leaving\n";
	}
	else
	{
		_debug->Send(debugInformation, debugInteractions) << "Testing Cell: not touching\n";
	}

	return;
}

void CubicMazeObjectInteractions::IsTouchingRightWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(+CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer);

	_debug->Send(debugInformation, debugInteractions) << "IsTouchingRightWall: min = " << min << ", max = " << max << "\n"; 

	//	When the first of these functions returns true, then the method will return.
	bool isTouching =
		IsTouchingLeftSide(input, output, min, max, true) ||
		IsTouchingTopSide(input, output, min, max, false) ||
		IsTouchingBottomSide(input, output, min, max, false) ||
		IsTouchingFrontSide(input, output, min, max, false) ||
		IsTouchingBackSide(input, output, min, max, false);

	//	Avoid build warning/error "unused variable 'isTouching'"
	if (isTouching)
		return;
}

void CubicMazeObjectInteractions::IsTouchingLeftWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer);
	TurboVector3D max = TurboVector3D(-CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer);

	_debug->Send(debugInformation, debugInteractions) << "IsTouchingLeftWall: min = " << min << ", max = " << max << "\n";

	//	When the first of these functions returns true, then the method will return.
	bool isTouching =
		IsTouchingRightSide(input, output, min, max, true) ||
		IsTouchingTopSide(input, output, min, max, false) ||
		IsTouchingBottomSide(input, output, min, max, false) ||
		IsTouchingFrontSide(input, output, min, max, false) ||
		IsTouchingBackSide(input, output, min, max, false);

	//	Avoid build warning/error "unused variable 'isTouching'"
	if (isTouching)
		return;
}

void CubicMazeObjectInteractions::IsTouchingBottomWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF + _buffer, -CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer);

	_debug->Send(debugInformation, debugInteractions) << "IsTouchingBottomWall: min = " << min << ", max = " << max << "\n";

	//	When the first of these functions returns true, then the method will return.
	bool isTouching =
		IsTouchingTopSide(input, output, min, max, true) ||
		IsTouchingFrontSide(input, output, min, max, false) ||
		IsTouchingBackSide(input, output, min, max, false) ||
		IsTouchingLeftSide(input, output, min, max, false) ||
		IsTouchingRightSide(input, output, min, max, false);

	//	Avoid build warning/error "unused variable 'isTouching'"
	if (isTouching)
		return;
}

void CubicMazeObjectInteractions::IsTouchingTopWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF - _buffer, +CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer);

	_debug->Send(debugInformation, debugInteractions) << "IsTouchingTopWall: min = " << min << ", max = " << max << "\n";

	//	When the first of these functions returns true, then the method will return.
	bool isTouching =
		IsTouchingBottomSide(input, output, min, max, true) ||
		IsTouchingFrontSide(input, output, min, max, false) ||
		IsTouchingBackSide(input, output, min, max, false) ||
		IsTouchingLeftSide(input, output, min, max, false) ||
		IsTouchingRightSide(input, output, min, max, false);

	//	Avoid build warning/error "unused variable 'isTouching'"
	if (isTouching)
		return;
}

void CubicMazeObjectInteractions::IsTouchingFrontWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer, -CELLHALF + WALLHALF + _buffer);

	_debug->Send(debugInformation, debugInteractions) << "IsTouchingFrontWall: min = " << min << ", max = " << max << "\n";

	//	When the first of these functions returns true, then the method will return.
	bool isTouching =
		IsTouchingBackSide(input, output, min, max, true) ||
		IsTouchingLeftSide(input, output, min, max, false) ||
		IsTouchingRightSide(input, output, min, max, false) ||
		IsTouchingTopSide(input, output, min, max, false) ||
		IsTouchingBottomSide(input, output, min, max, false);

	//	Avoid build warning/error "unused variable 'isTouching'"
	if (isTouching)
		return;
}

void CubicMazeObjectInteractions::IsTouchingBackWall(InputArguments* input, OutputArguments* output)
{
	output->portalIndex = 0;
	output->isLeaving = false;
	output->isTouching = false;
	output->contact = TurboVector3D();
	output->normal = TurboVector3D();

	TurboVector3D min = TurboVector3D(-CELLHALF - WALLHALF - _buffer, -CELLHALF - WALLHALF - _buffer, +CELLHALF - WALLHALF - _buffer);
	TurboVector3D max = TurboVector3D(+CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer, +CELLHALF + WALLHALF + _buffer);

	_debug->Send(debugInformation, debugInteractions) << "IsTouchingBackWall: min = " << min << ", max = " << max << "\n";

	//	When the first of these functions returns true, then the method will return.
	bool isTouching =
		IsTouchingFrontSide(input, output, min, max, true) ||
		IsTouchingLeftSide(input, output, min, max, false) ||
		IsTouchingRightSide(input, output, min, max, false) ||
		IsTouchingTopSide(input, output, min, max, false) ||
		IsTouchingBottomSide(input, output, min, max, false);

	//	Avoid build warning/error "unused variable 'isTouching'"
	if (isTouching)
		return;
}

bool CubicMazeObjectInteractions::IsTouchingLeftSide(InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide)
{
	if ((input->fromPoint.X <= input->toPoint.X) &&		//	moving in +X direction
		(input->fromPoint.X <= min.X + _buffer) &&		//	fromPoint has not entered wall
		(input->toPoint.X >= min.X))					//	toPoint has entered buffer zone
	{
		double factor = (min.X - input->fromPoint.X) / (input->toPoint.X - input->fromPoint.X);

		output->contact = input->fromPoint + (input->toPoint - input->fromPoint) * factor;
		output->normal = TurboVector3D(-1.0, 0.0, 0.0);

		if ((isWallSide) && (input->isPlayer) && (input->cellWall.PortalIndex > 0))
		{
			if ((output->contact.Y >= -PORTALHALF) && (output->contact.Y <= PORTALHALF) &&
				(output->contact.Z >= -PORTALHALF) && (output->contact.Z <= PORTALHALF))
			{
				output->portalIndex = input->cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Left:  portal " << output->portalIndex << '\n';
				return true;
			}
		}

		if ((output->contact.Y >= min.Y) && (output->contact.Y <= max.Y) &&
			(output->contact.Z >= min.Z) && (output->contact.Z <= max.Z))
		{
			if (input->cellWall.Type == CubicMazeCellWallType::None)
			{
				output->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Left:  leaving\n";
			}
			else
			{
				output->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Left:  touching\n";
			}
			return true;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Left:  not touching\n";
	return false;
}

bool CubicMazeObjectInteractions::IsTouchingRightSide(InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide)
{
	if ((input->fromPoint.X >= input->toPoint.X) &&		//	moving in -X direction
		(input->fromPoint.X >= max.X - _buffer) &&		//	fromPoint has not entered wall
		(input->toPoint.X <= max.X))					//	toPoint has entered buffer zone
	{
		double factor = (max.X - input->fromPoint.X) / (input->toPoint.X - input->fromPoint.X);

		output->contact = input->fromPoint + (input->toPoint - input->fromPoint) * factor;
		output->normal = TurboVector3D(1.0, 0.0, 0.0);

		if ((isWallSide) && (input->isPlayer) && (input->cellWall.PortalIndex > 0))
		{
			if ((output->contact.Y >= -PORTALHALF) && (output->contact.Y <= PORTALHALF) &&
				(output->contact.Z >= -PORTALHALF) && (output->contact.Z <= PORTALHALF))
			{
				output->portalIndex = input->cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Right:  portal " << output->portalIndex << '\n';
				return true;
			}
		}

		if ((output->contact.Y >= min.Y) && (output->contact.Y <= max.Y) &&
			(output->contact.Z >= min.Z) && (output->contact.Z <= max.Z))
		{
			if (input->cellWall.Type == CubicMazeCellWallType::None)
			{
				output->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Right:  leaving\n";
			}
			else
			{
				output->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Right:  touching\n";
			}
			return true;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Right:  not touching\n";
	return false;
}

bool CubicMazeObjectInteractions::IsTouchingTopSide(InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide)
{
	if ((input->fromPoint.Y >= input->toPoint.Y) &&		//	moving in -Y direction
		(input->fromPoint.Y >= max.Y - _buffer) &&		//	fromPoint has not entered wall
		(input->toPoint.Y <= max.Y))					//	toPoint has entered buffer zone
	{
		double factor = (max.Y - input->fromPoint.Y) / (input->toPoint.Y - input->fromPoint.Y);

		output->contact = input->fromPoint + (input->toPoint - input->fromPoint) * factor;
		output->normal = TurboVector3D(0.0, 1.0, 0.0);

		if ((isWallSide) && (input->isPlayer) && (input->cellWall.PortalIndex > 0))
		{
			if ((output->contact.Z >= -PORTALHALF) && (output->contact.Z <= PORTALHALF) &&
				(output->contact.X >= -PORTALHALF) && (output->contact.X <= PORTALHALF))
			{
				output->portalIndex = input->cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Top:  portal " << output->portalIndex << '\n';
				return true;
			}
		}

		if ((output->contact.Z >= min.Z) && (output->contact.Z <= max.Z) &&
			(output->contact.X >= min.X) && (output->contact.X <= max.X))
		{
			if (input->cellWall.Type == CubicMazeCellWallType::None)
			{
				output->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Top:  leaving\n";
			}
			else
			{
				output->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Top:  touching\n";
			}
			return true;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Top:  not touching\n";
	return false;
}

bool CubicMazeObjectInteractions::IsTouchingBottomSide(InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide)
{

	if ((input->fromPoint.Y <= input->toPoint.Y) &&		//	moving in +Y direction
		(input->fromPoint.Y <= min.Y + _buffer) &&		//	fromPoint has not entered wall
		(input->toPoint.Y >= min.Y))					//	toPoint has entered buffer zone
	{
		double factor = (min.Y - input->fromPoint.Y) / (input->toPoint.Y - input->fromPoint.Y);

		output->contact = input->fromPoint + (input->toPoint - input->fromPoint) * factor;
		output->normal = TurboVector3D(0.0, -1.0, 0.0);

		if ((isWallSide) && (input->isPlayer) && (input->cellWall.PortalIndex > 0))
		{
			if ((output->contact.Z >= -PORTALHALF) && (output->contact.Z <= PORTALHALF) &&
				(output->contact.X >= -PORTALHALF) && (output->contact.X <= PORTALHALF))
			{
				output->portalIndex = input->cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Bottom:  portal " << output->portalIndex << '\n';
				return true;
			}
		}

		if ((output->contact.Z >= min.Z) && (output->contact.Z <= max.Z) &&
			(output->contact.X >= min.X) && (output->contact.X <= max.X))
		{
			if (input->cellWall.Type == CubicMazeCellWallType::None)
			{
				output->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Bottom:  leaving\n";
			}
			else
			{
				output->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Bottom:  touching\n";
			}
			return true;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Bottom:  not touching\n";
	return false;
}

bool CubicMazeObjectInteractions::IsTouchingBackSide(InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide)
{
	if ((input->fromPoint.Z >= input->toPoint.Z) &&		//	moving in -Z direction
		(input->fromPoint.Z >= max.Z - _buffer) &&		//	fromPoint has not entered wall
		(input->toPoint.Z <= max.Z))					//	toPoint has entered buffer zone
	{
		double factor = (max.Z - input->fromPoint.Z) / (input->toPoint.Z - input->fromPoint.Z);

		output->contact = input->fromPoint + (input->toPoint - input->fromPoint) * factor;
		output->normal = TurboVector3D(0.0, 0.0, 1.0);

		if ((isWallSide) && (input->isPlayer) && (input->cellWall.PortalIndex > 0))
		{
			if ((output->contact.X >= -PORTALHALF) && (output->contact.X <= PORTALHALF) &&
				(output->contact.Y >= -PORTALHALF) && (output->contact.Y <= PORTALHALF))
			{
				output->portalIndex = input->cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Back:  portal " << output->portalIndex << '\n';
				return true;
			}
		}

		if ((output->contact.X >= min.X) && (output->contact.X <= max.X) &&
			(output->contact.Y >= min.Y) && (output->contact.Y <= max.Y))
		{
			if (input->cellWall.Type == CubicMazeCellWallType::None)
			{
				output->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Back:  leaving\n";
			}
			else
			{
				output->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Back:  touching\n";
			}
			return true;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Back:  not touching\n";
	return false;
}

bool CubicMazeObjectInteractions::IsTouchingFrontSide(InputArguments* input, OutputArguments* output, TurboVector3D min, TurboVector3D max, bool isWallSide)
{
	if ((input->fromPoint.Z <= input->toPoint.Z) &&		//	moving in +Z direction
		(input->fromPoint.Z <= min.Z + _buffer) &&		//	fromPoint has not entered wall
		(input->toPoint.Z >= min.Z))					//	toPoint has entered buffer zone
	{
		double factor = (min.Z - input->fromPoint.Z) / (input->toPoint.Z - input->fromPoint.Z);

		output->contact = input->fromPoint + (input->toPoint - input->fromPoint) * factor;
		output->normal = TurboVector3D(0.0, 0.0, -1.0);

		if ((isWallSide) && (input->isPlayer) && (input->cellWall.PortalIndex > 0))
		{
			if ((output->contact.X >= -PORTALHALF) && (output->contact.X <= PORTALHALF) &&
				(output->contact.Y >= -PORTALHALF) && (output->contact.Y <= PORTALHALF))
			{
				output->portalIndex = input->cellWall.PortalIndex;
				_debug->Send(debugInformation, debugInteractions) << "Front:  portal " << output->portalIndex << '\n';
				return true;
			}
		}

		if ((output->contact.X >= min.X) && (output->contact.X <= max.X) &&
			(output->contact.Y >= min.Y) && (output->contact.Y <= max.Y))
		{
			if (input->cellWall.Type == CubicMazeCellWallType::None)
			{
				output->isLeaving = true;
				_debug->Send(debugInformation, debugInteractions) << "Front:  leaving\n";
			}
			else
			{
				output->isTouching = true;
				_debug->Send(debugInformation, debugInteractions) << "Front:  touching\n";
			}
			return true;
		}
	}

	_debug->Send(debugInformation, debugInteractions) << "Front:  not touching\n";
	return false;
}
