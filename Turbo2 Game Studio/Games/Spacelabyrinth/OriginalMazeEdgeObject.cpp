
#include "pch.h"

#include <OriginalMazeEdgeObject.h>
#include <TurboSceneObjectMesh.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeEdgeObject::OriginalMazeEdgeObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectMaterial> material)
{
	std::shared_ptr<ITurboSceneObjectMesh> mesh = std::shared_ptr<ITurboSceneObjectMesh>(new TurboSceneObjectMesh());

	if (mazeObject.Right - mazeObject.Left > CELLHALF)
	{
		Vector3D normal = Vector3D(0.0f, 0.0f, 1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 0.0f	//	1.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 1.0f	//	0.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));		//	1.0f, 1.0f	//	0.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 0.0f	//	1.0f, 1.0f	//	X,X

		normal = Vector3D(0.0f, 1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	0.0f, 0.0f	//	X,X			//	0.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 1.0f	//	X,X			//	1.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	1.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));		//	1.0f, 0.0f	//	X,X			//	0.0f, 0.0f

		normal = Vector3D(0.0f, 0.0f, -1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 0.0f	//	0.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 1.0f	//	1.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 1.0f	//	1.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 0.0f	//	0.0f, 0.0f	//	X,X

		normal = Vector3D(0.0f, -1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 0.0f	//	X,X			//	1.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));	//	0.0f, 1.0f	//	X,X			//	0.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	0.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 0.0f	//	X,X			//	1.0f, 1.0f


		normal = Vector3D(-1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));		//	X,X			//	0.0f, 0.0f	//	1.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	X,X			//	0.0f, 1.0f	//	0.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	X,X			//	1.0f, 1.0f	//	0.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	1.0f, 1.0f	

		normal = Vector3D(1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));		//	X,X			//	0.0f, 0.0f	//	0.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));	//	X,X			//	0.0f, 1.0f	//	1.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	X,X			//	1.0f, 1.0f	//	1.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	0.0f, 0.0f	
	}
	else if (mazeObject.Top - mazeObject.Bottom > CELLHALF)
	{
		Vector3D normal = Vector3D(-1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));		//	X,X			//	0.0f, 0.0f	//	1.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	X,X			//	0.0f, 1.0f	//	0.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));	//	X,X			//	1.0f, 1.0f	//	0.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	1.0f, 1.0f	

		normal = Vector3D(0.0f, 0.0f, 1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	0.0f, 1.0f	//	0.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	1.0f, 1.0f	//	0.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 0.0f	//	1.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	0.0f, 0.0f	//	1.0f, 0.0f	//	X,X

		normal = Vector3D(1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	X,X			//	0.0f, 0.0f	//	0.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));	//	X,X			//	0.0f, 1.0f	//	1.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	X,X			//	1.0f, 1.0f	//	1.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	0.0f, 0.0f	

		normal = Vector3D(0.0f, 0.0f, -1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	1.0f, 0.0f	//	0.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 0.0f	//	0.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	0.0f, 1.0f	//	1.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 1.0f	//	1.0f, 0.0f	//	X,X


		normal = Vector3D(0.0f, 1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));		//	1.0f, 0.0f	//	X,X			//	0.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	0.0f, 0.0f	//	X,X			//	0.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 1.0f	//	X,X			//	1.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	1.0f, 0.0f

		normal = Vector3D(0.0f, -1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));	//	0.0f, 1.0f	//	X,X			//	0.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	0.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 0.0f	//	X,X			//	1.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 0.0f	//	X,X			//	1.0f, 0.0f

	}
	else if (mazeObject.Back - mazeObject.Front > CELLHALF)
	{
		Vector3D normal = Vector3D(0.0f, 1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	1.0f, 0.0f	//	X,X			//	0.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 0.0f	//	X,X			//	0.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));		//	0.0f, 1.0f	//	X,X			//	1.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 1.0f	//	X,X			//	1.0f, 0.0f

		normal = Vector3D(-1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	X,X			//	0.0f, 1.0f	//	0.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));	//	X,X			//	1.0f, 1.0f	//	0.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	X,X			//	1.0f, 0.0f	//	1.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));		//	X,X			//	0.0f, 0.0f	//	1.0f, 0.0f	

		normal = Vector3D(0.0f, -1.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 1.0f	//	X,X			//	0.0f, 0.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	0.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 0.0f	//	X,X			//	1.0f, 1.0f
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	0.0f, 0.0f	//	X,X			//	1.0f, 0.0f

		normal = Vector3D(1.0f, 0.0f, 0.0f);
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	0.0f, 0.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	X,X			//	0.0f, 0.0f	//	0.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	X,X			//	0.0f, 1.0f	//	1.0f, 1.0f	
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 1.0f	//	1.0f, 0.0f	


		normal = Vector3D(0.0f, 0.0f, 1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 0.0f	//	1.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 1.0f	//	0.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));		//	1.0f, 1.0f	//	0.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 0.0f	//	1.0f, 1.0f	//	X,X

		normal = Vector3D(0.0f, 0.0f, -1.0f);
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 0.0f	//	0.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 1.0f	//	1.0f, 1.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 1.0f	//	1.0f, 0.0f	//	X,X
		mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 0.0f	//	0.0f, 0.0f	//	X,X
	}

	/*
	Vector3D normal = Vector3D(0.0f, 0.0f, 1.0f);
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 0.0f	//	1.0f, 0.0f	//	X,X
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 1.0f	//	0.0f, 0.0f	//	X,X
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));		//	1.0f, 1.0f	//	0.0f, 1.0f	//	X,X
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 0.0f	//	1.0f, 1.0f	//	X,X

	normal = Vector3D(0.0f, 0.0f, -1.0f);
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 0.0f	//	0.0f, 1.0f	//	X,X
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 1.0f	//	1.0f, 1.0f	//	X,X
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 1.0f	//	1.0f, 0.0f	//	X,X
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 0.0f	//	0.0f, 0.0f	//	X,X


	normal = Vector3D(0.0f, 1.0f, 0.0f);
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));		//	1.0f, 0.0f	//	X,X			//	0.0f, 0.0f
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));		//	0.0f, 0.0f	//	X,X			//	0.0f, 1.0f
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));		//	0.0f, 1.0f	//	X,X			//	1.0f, 1.0f
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	1.0f, 0.0f

	normal = Vector3D(0.0f, -1.0f, 0.0f);
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));	//	0.0f, 1.0f	//	X,X			//	0.0f, 0.0f
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));	//	1.0f, 1.0f	//	X,X			//	0.0f, 1.0f
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	1.0f, 0.0f	//	X,X			//	1.0f, 1.0f
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	0.0f, 0.0f	//	X,X			//	1.0f, 0.0f


	normal = Vector3D(-1.0f, 0.0f, 0.0f);
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));		//	X,X			//	0.0f, 0.0f	//	1.0f, 0.0f	
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));		//	X,X			//	0.0f, 1.0f	//	0.0f, 0.0f	
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));	//	X,X			//	1.0f, 1.0f	//	0.0f, 1.0f	
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	1.0f, 1.0f	

	normal = Vector3D(1.0f, 0.0f, 0.0f);
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));		//	X,X			//	0.0f, 0.0f	//	0.0f, 1.0f	
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));	//	X,X			//	0.0f, 1.0f	//	1.0f, 1.0f	
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));	//	X,X			//	1.0f, 1.0f	//	1.0f, 0.0f	
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));	//	X,X			//	1.0f, 0.0f	//	0.0f, 0.0f	
	*/

	mesh->AddTriangle(0, 1, 2);
	mesh->AddTriangle(2, 3, 0);

	mesh->AddTriangle(4, 5, 6);
	mesh->AddTriangle(6, 7, 4);

	mesh->AddTriangle(8, 9, 10);
	mesh->AddTriangle(10, 11, 8);

	mesh->AddTriangle(12, 13, 14);
	mesh->AddTriangle(14, 15, 12);

	mesh->AddTriangle(16, 17, 18);
	mesh->AddTriangle(18, 19, 16);

	mesh->AddTriangle(20, 21, 22);
	mesh->AddTriangle(22, 23, 20);

	Mesh(mesh);
	Material(material);
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------

//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------
