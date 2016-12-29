
#include "pch.h"

#include <OriginalMazeWallObject.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeWallObject::OriginalMazeWallObject(
	std::shared_ptr<ITurboSceneObjectMesh> mesh, 
	MazeObject mazeObject, 
	std::shared_ptr<ITurboSceneObjectTexture> texture1, 
	std::shared_ptr<ITurboSceneObjectTexture> texture2)
{
	_rightLeft = false;
	_topBottom = false;
	_backFront = false;

	//  Right/Left wall
	if (mazeObject.Right - mazeObject.Left < CELLHALF)
	{
		_rightLeft = true;

		Vector3D normal = Vector3D(-1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));

		normal = Vector3D(1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));
	}

	//  Top/Bottom wall (ceiling/floor)
	if (mazeObject.Top - mazeObject.Bottom < CELLHALF)
	{
		_topBottom = true;

		Vector3D normal = Vector3D(0.0f, 1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));

		normal = Vector3D(0.0f, -1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
	}

	//  Back/Front wall
	if (mazeObject.Back - mazeObject.Front < CELLHALF)
	{
		_backFront = true;

		Vector3D normal = Vector3D(0.0f, 0.0f, 1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));

		normal = Vector3D(0.0f, 0.0f, -1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));
	}

	mesh->AddTriangle(0, 1, 2, texture1);
	mesh->AddTriangle(2, 3, 0, texture1);

	mesh->AddTriangle(4, 5, 6, texture2);
	mesh->AddTriangle(6, 7, 4, texture2);

	Mesh(mesh);
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------

bool OriginalMazeWallObject::IsTouching(Vector3D fromPoint, Vector3D toPoint, float buffer, Vector3D *contact, Vector3D *normal)
{
	if (_rightLeft)
	{
		if (IsTouchingX(fromPoint, toPoint, buffer, contact, normal))
			return true;
		if (IsTouchingY(fromPoint, toPoint, buffer, contact, normal))
			return true;
		if (IsTouchingZ(fromPoint, toPoint, buffer, contact, normal))
			return true;
		return false;
	}

	if (_topBottom)
	{
		if (IsTouchingY(fromPoint, toPoint, buffer, contact, normal))
			return true;
		if (IsTouchingZ(fromPoint, toPoint, buffer, contact, normal))
			return true;
		if (IsTouchingX(fromPoint, toPoint, buffer, contact, normal))
			return true;
		return false;
	}

	if (_backFront)
	{
		if (IsTouchingZ(fromPoint, toPoint, buffer, contact, normal))
			return true;
		if (IsTouchingX(fromPoint, toPoint, buffer, contact, normal))
			return true;
		if (IsTouchingY(fromPoint, toPoint, buffer, contact, normal))
			return true;
		return false;
	}

	return false;
}

//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------
//  Local Support Methods  ---------------------------------------------------------------------------------------------

bool OriginalMazeWallObject::IsTouchingX(Vector3D fromPoint, Vector3D toPoint, float buffer, Vector3D *contact, Vector3D *normal)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = Mesh();

	Vector3D min = mesh->MinExtent();
	Vector3D max = mesh->MaxExtent();

	if ((abs(toPoint.X - min.X) > CELLSIZE) ||
		(abs(toPoint.X - max.X) > CELLSIZE))
		return false;

	if ((fromPoint.X <= toPoint.X) &&	//	moving in +X direction
		(fromPoint.X <= min.X) &&		//	fromPoint has not entered wall
		(toPoint.X + buffer >= min.X))	//	toPoint has entered buffer zone
	{
		float factor = (toPoint.X - fromPoint.X) / (min.X - buffer - fromPoint.X);
		*contact = fromPoint + (toPoint - fromPoint) * factor;

		if ((contact->Y + buffer >= min.Y) && (contact->Y - buffer <= max.Y) &&
			(contact->Z + buffer >= min.Z) && (contact->Z - buffer <= max.Z))
		{
			*normal = Vector3D(-1.0, 0.0, 0.0);
			return true;
		}
		return false;
	}

	if ((fromPoint.X >= toPoint.X) &&	//	moving in -X direction
		(fromPoint.X >= max.X) &&		//	fromPoint has not entered wall
		(toPoint.X - buffer <= max.X))	//	toPoint has entered buffer zone
	{
		float factor = (toPoint.X - fromPoint.X) / (max.X - buffer - fromPoint.X);
		*contact = fromPoint + (toPoint - fromPoint) * factor;

		if ((contact->Y + buffer >= min.Y) && (contact->Y - buffer <= max.Y) &&
			(contact->Z + buffer >= min.Z) && (contact->Z - buffer <= max.Z))
		{
			*normal = Vector3D(1.0, 0.0, 0.0);
			return true;
		}
		return false;
	}

	return false;
}


bool OriginalMazeWallObject::IsTouchingY(Vector3D fromPoint, Vector3D toPoint, float buffer, Vector3D *contact, Vector3D *normal)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = Mesh();

	Vector3D min = mesh->MinExtent();
	Vector3D max = mesh->MaxExtent();

	if ((abs(toPoint.Y - min.Y) > CELLSIZE) ||
		(abs(toPoint.Y - max.Y) > CELLSIZE))
		return false;

	if ((fromPoint.Y <= toPoint.Y) &&	//	moving in +Y direction
		(fromPoint.Y <= min.Y) &&		//	fromPoint has not entered wall
		(toPoint.Y + buffer >= min.Y))	//	toPoint has entered buffer zone
	{
		float factor = (toPoint.Y - fromPoint.Y) / (min.Y - buffer - fromPoint.Y);
		*contact = fromPoint + (toPoint - fromPoint) * factor;

		if ((contact->Z + buffer >= min.Z) && (contact->Z - buffer <= max.Z) &&
			(contact->X + buffer >= min.X) && (contact->X - buffer <= max.X))
		{
			*normal = Vector3D(0.0, -1.0, 0.0);
			return true;
		}
		return false;
	}

	if ((fromPoint.Y >= toPoint.Y) &&	//	moving in -Y direction
		(fromPoint.Y >= max.Y) &&		//	fromPoint has not entered wall
		(toPoint.Y - buffer <= max.Y))	//	toPoint has entered buffer zone
	{
		float factor = (toPoint.Y - fromPoint.Y) / (max.Y - fromPoint.Y);
		*contact = fromPoint + (toPoint - fromPoint) * factor;

		if ((contact->Z + buffer >= min.Z) && (contact->Z - buffer <= max.Z) &&
			(contact->X + buffer >= min.X) && (contact->X - buffer <= max.X))
		{
			*normal = Vector3D(0.0, 1.0, 0.0);
			return true;
		}
		return false;
	}

	return false;
}


bool OriginalMazeWallObject::IsTouchingZ(Vector3D fromPoint, Vector3D toPoint, float buffer, Vector3D *contact, Vector3D *normal)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = Mesh();

	Vector3D min = mesh->MinExtent();
	Vector3D max = mesh->MaxExtent();

	if ((abs(toPoint.Z - min.Z) > CELLSIZE) ||
		(abs(toPoint.Z - max.Z) > CELLSIZE))
		return false;

	if ((fromPoint.Z <= toPoint.Z) &&	//	moving in +Z direction
		(fromPoint.Z <= min.Z) &&		//	fromPoint has not entered wall
		(toPoint.Z + buffer >= min.Z))	//	toPoint has entered buffer zone
	{
		float factor = (toPoint.Z - fromPoint.Z) / (min.Z - buffer - fromPoint.Z);
		*contact = fromPoint + (toPoint - fromPoint) * factor;

		if ((contact->X + buffer >= min.X) && (contact->X - buffer <= max.X) &&
			(contact->Y + buffer >= min.Y) && (contact->Y - buffer <= max.Y))
		{
			*normal = Vector3D(0.0, 0.0, -1.0);
			return true;
		}
		return false;
	}

	if ((fromPoint.Z >= toPoint.Z) &&	//	moving in -Z direction
		(fromPoint.Z >= max.Z) &&		//	fromPoint has not entered wall
		(toPoint.Z - buffer <= max.Z))	//	toPoint has entered buffer zone
	{
		float factor = (toPoint.Z - fromPoint.Z) / (min.Z - buffer - fromPoint.Z);
		*contact = fromPoint + (toPoint - fromPoint) * factor;

		if ((contact->X + buffer >= min.X) && (contact->X - buffer <= max.X) &&
			(contact->Y + buffer >= min.Y) && (contact->Y - buffer <= max.Y))
		{
			*normal = Vector3D(0.0, 0.0, 1.0);
			return true;
		}
		return false;
	}

	return false;
}

//  Local Support Methods  ---------------------------------------------------------------------------------------------
