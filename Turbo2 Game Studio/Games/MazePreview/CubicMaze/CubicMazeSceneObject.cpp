
#include <pch.h>

#include <CubicMaze.h>
#include <CubicMazeSceneObject.h>
#include <TurboSceneMesh.h>

using namespace Turbo::Math;
using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

CubicMazeSceneObject::CubicMazeSceneObject(std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material)
{
	Mesh(mesh);
	Material(material);
	PrimitiveType(SceneObjectPrimitiveType::Mesh);
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboSceneObject Methods ------------------------------------------------------------------------------------------

bool CubicMazeSceneObject::IsTouching(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	if (mesh == nullptr)
	{
		return false;
	}

	TurboMatrix4x4 inverse = Placement()->Inverse();

	//  transform fromPoint and toPoint into model space.
	TurboVector3D modelFromPoint = fromPoint * inverse;
	TurboVector3D modelToPoint = toPoint * inverse;

	/*
	double nearestDistance = 1e+10;
	TurboVector3D nearestIntersection = TurboVector3D(1e+10, 1e+10, 1e+10);

	for (auto& triangle : mesh->Triangles())
	{
		TurboVector3D p1 = mesh->Vertices()[triangle.Vertex1].Position;
		TurboVector3D p2 = mesh->Vertices()[triangle.Vertex2].Position;
		TurboVector3D p3 = mesh->Vertices()[triangle.Vertex3].Position;

		//	From http://geomalgorithms.com/a05-_intersect-1.html

		// normal from cross product of clockwise points
		TurboVector3D n = ((p1 - p2) % (p3 - p2)).Normalize();
		TurboVector3D v = p2 + n * buffer;
		TurboVector3D u = modelToPoint - modelFromPoint;
		TurboVector3D w = modelFromPoint - v;

		double n_dot_u = n * u;
		double n_dot_w = n * w;

		if (n_dot_u == 0.0)
		{
			//	The line is parallel to the plane.
			continue;
		}

		double sI = -n_dot_w / n_dot_u;

		TurboVector3D pI = modelFromPoint + u * sI;	//	intersection point.
	}
	*/

	bool result = false;

	double nearestDistance = 0.0;
	TurboVector3D nearestContact;
	TurboVector3D nearestNormal;

	double distance;
	TurboVector3D contact;
	TurboVector3D normal;

	if (IsTouchingLeftSideOfWall(modelFromPoint, modelToPoint, buffer, &contact, &normal))
	{
		distance = (contact - modelFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingFrontSideOfWall(modelFromPoint, modelToPoint, buffer, &contact, &normal))
	{
		distance = (contact - modelFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingTopSideOfWall(modelFromPoint, modelToPoint, buffer, &contact, &normal))
	{
		distance = (contact - modelFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingBottomSideOfWall(modelFromPoint, modelToPoint, buffer, &contact, &normal))
	{
		distance = (contact - modelFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingBackSideOfWall(modelFromPoint, modelToPoint, buffer, &contact, &normal))
	{
		distance = (contact - modelFromPoint).Length();

		if ((!result) || (distance < nearestDistance))
		{
			nearestDistance = distance;
			nearestContact = contact;
			nearestNormal = normal;
			result = true;
		}
	}

	if (IsTouchingFrontSideOfWall(modelFromPoint, modelToPoint, buffer, &contact, &normal))
	{
		distance = (contact - modelFromPoint).Length();

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
		TurboMatrix4x4 transform = Placement()->Transform();

		*pContact = nearestContact * transform;

		transform = transform.Translate(-(Placement()->Position()));

		*pNormal = nearestNormal * transform;
	}
	
	return result;
}

//  ITurboSceneObject Methods ------------------------------------------------------------------------------------------
//  Local Support Methods ----------------------------------------------------------------------------------------------

bool CubicMazeSceneObject::IsTouchingLeftSideOfWall(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	TurboVector3D min = mesh->MinExtent();
	TurboVector3D max = mesh->MaxExtent();

	if ((abs(toPoint.X - min.X) > CELLSIZE) ||
		(abs(toPoint.X - max.X) > CELLSIZE))
	{
		return false;
	}

	if ((fromPoint.X <= toPoint.X) &&	//	moving in +X direction
		(fromPoint.X <= min.X) &&		//	fromPoint has not entered wall
		(toPoint.X + buffer >= min.X))	//	toPoint has entered buffer zone
	{
		double factor = (toPoint.X - fromPoint.X) / (min.X - buffer - fromPoint.X);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(-1.0, 0.0, 0.0);

		if ((pContact->Y + WALLSIZE >= min.Y) && (pContact->Y - WALLSIZE <= max.Y) &&
			(pContact->Z + WALLSIZE >= min.Z) && (pContact->Z - WALLSIZE <= max.Z))
		{
			return true;
		}

		//if ((pContact->Y + buffer >= min.Y) && (pContact->Y - buffer <= max.Y) &&
		//	(pContact->Z + buffer >= min.Z) && (pContact->Z - buffer <= max.Z))
		//{
		//	return true;
		//}
	}

	return false;
}

bool CubicMazeSceneObject::IsTouchingRightSideOfWall(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	TurboVector3D min = mesh->MinExtent();
	TurboVector3D max = mesh->MaxExtent();

	if ((abs(toPoint.X - min.X) > CELLSIZE) ||
		(abs(toPoint.X - max.X) > CELLSIZE))
	{
		return false;
	}

	if ((fromPoint.X >= toPoint.X) &&	//	moving in -X direction
		(fromPoint.X >= max.X) &&		//	fromPoint has not entered wall
		(toPoint.X - buffer <= max.X))	//	toPoint has entered buffer zone
	{
		double factor = (toPoint.X - fromPoint.X) / (max.X - buffer - fromPoint.X);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(1.0, 0.0, 0.0);

		if ((pContact->Y + WALLSIZE >= min.Y) && (pContact->Y - WALLSIZE <= max.Y) &&
			(pContact->Z + WALLSIZE >= min.Z) && (pContact->Z - WALLSIZE <= max.Z))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeSceneObject::IsTouchingTopSideOfWall(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	TurboVector3D min = mesh->MinExtent();
	TurboVector3D max = mesh->MaxExtent();

	if ((abs(toPoint.Y - min.Y) > CELLSIZE) ||
		(abs(toPoint.Y - max.Y) > CELLSIZE))
	{
		return false;
	}

	if ((fromPoint.Y >= toPoint.Y) &&	//	moving in -Y direction
		(fromPoint.Y >= max.Y) &&		//	fromPoint has not entered wall
		(toPoint.Y - buffer <= max.Y))	//	toPoint has entered buffer zone
	{
		double factor = (toPoint.Y - fromPoint.Y) / (max.Y - fromPoint.Y);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, 1.0, 0.0);

		if ((pContact->Z + WALLSIZE >= min.Z) && (pContact->Z - WALLSIZE <= max.Z) &&
			(pContact->X + WALLSIZE >= min.X) && (pContact->X - WALLSIZE <= max.X))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeSceneObject::IsTouchingBottomSideOfWall(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	TurboVector3D min = mesh->MinExtent();
	TurboVector3D max = mesh->MaxExtent();

	if ((abs(toPoint.Y - min.Y) > CELLSIZE) ||
		(abs(toPoint.Y - max.Y) > CELLSIZE))
	{
		return false;
	}

	if ((fromPoint.Y <= toPoint.Y) &&	//	moving in +Y direction
		(fromPoint.Y <= min.Y) &&		//	fromPoint has not entered wall
		(toPoint.Y + buffer >= min.Y))	//	toPoint has entered buffer zone
	{
		double factor = (toPoint.Y - fromPoint.Y) / (min.Y - buffer - fromPoint.Y);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, -1.0, 0.0);

		if ((pContact->Z + WALLSIZE >= min.Z) && (pContact->Z - WALLSIZE <= max.Z) &&
			(pContact->X + WALLSIZE >= min.X) && (pContact->X - WALLSIZE <= max.X))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeSceneObject::IsTouchingBackSideOfWall(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	TurboVector3D min = mesh->MinExtent();
	TurboVector3D max = mesh->MaxExtent();

	if ((abs(toPoint.Z - min.Z) > CELLSIZE) ||
		(abs(toPoint.Z - max.Z) > CELLSIZE))
	{
		return false;
	}

	if ((fromPoint.Z >= toPoint.Z) &&	//	moving in -Z direction
		(fromPoint.Z >= max.Z) &&		//	fromPoint has not entered wall
		(toPoint.Z - buffer <= max.Z))	//	toPoint has entered buffer zone
	{
		double factor = (toPoint.Z - fromPoint.Z) / (min.Z - buffer - fromPoint.Z);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, 0.0, 1.0);

		if ((pContact->X + WALLSIZE >= min.X) && (pContact->X - WALLSIZE <= max.X) &&
			(pContact->Y + WALLSIZE >= min.Y) && (pContact->Y - WALLSIZE <= max.Y))
		{
			return true;
		}
	}

	return false;
}

bool CubicMazeSceneObject::IsTouchingFrontSideOfWall(TurboVector3D fromPoint, TurboVector3D toPoint, double buffer, TurboVector3D *pContact, TurboVector3D *pNormal)
{
	std::shared_ptr<ITurboSceneMesh> mesh = Mesh();

	TurboVector3D min = mesh->MinExtent();
	TurboVector3D max = mesh->MaxExtent();

	if ((abs(toPoint.Z - min.Z) > CELLSIZE) ||
		(abs(toPoint.Z - max.Z) > CELLSIZE))
	{
		return false;
	}

	if ((fromPoint.Z <= toPoint.Z) &&	//	moving in +Z direction
		(fromPoint.Z <= min.Z) &&		//	fromPoint has not entered wall
		(toPoint.Z + buffer >= min.Z))	//	toPoint has entered buffer zone
	{
		double factor = (toPoint.Z - fromPoint.Z) / (min.Z - buffer - fromPoint.Z);

		*pContact = fromPoint + (toPoint - fromPoint) * factor;
		*pNormal = TurboVector3D(0.0, 0.0, -1.0);

		if ((pContact->X + WALLSIZE >= min.X) && (pContact->X - WALLSIZE <= max.X) &&
			(pContact->Y + WALLSIZE >= min.Y) && (pContact->Y - WALLSIZE <= max.Y))
		{
			return true;
		}
	}

	return false;
}

//  Local Support Methods ----------------------------------------------------------------------------------------------
